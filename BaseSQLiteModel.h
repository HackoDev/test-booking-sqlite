#pragma once
#include "globals.h"
#include <vector>
#include <typeinfo>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>
#include "sqlite3.h"


#define GET_NAME(n) #n

class BaseSQLiteModel
{
protected:
	int id = 0;
	// get object from db by the his id
	template<class T>
	static std::string get_joined_fiedls()
	{
		std::string joined_fields = "";
		std::string quote = "`";
		int size_count = T::db_sorted_fields.size();
		for (int i = 0; i < size_count; i++)
			joined_fields += "`" + T::db_sorted_fields[i] + "`" + (i == size_count - 1 ? "" : ",");
		return joined_fields;
	}

	template<class T>
	static T* get_by_id(int id)
	{
		sqlite3_stmt *stmp;
		T* instance = new T();
		std::string sql_raw = "SELECT " + T::get_joined_fiedls<T>() + " FROM `" + T::get_table_name() + "` WHERE id=?;";
		if (DEBUG)
			std::cout << sql_raw << std::endl;
		int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmp, 0);
		if (sqlite3_bind_int(stmp, 1, id) != SQLITE_OK)
			std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		if (sqlite3_step(stmp) == SQLITE_ROW)
		{
			instance->load_from_stmt(stmp);
			sqlite3_finalize(stmp);
			return instance;
		}
		sqlite3_finalize(stmp);
		return NULL;
	};
public:
	static std::vector<std::string> db_sorted_fields;
	BaseSQLiteModel();
	~BaseSQLiteModel();
	virtual void prepare_params() = 0;
	virtual void load_from_stmt(sqlite3_stmt* db) = 0;

	void bind_id(std::string tb_name)
	{
		std::string sql_raw = "SELECT `id` FROM `" + tb_name + "` ORDER BY `id` DESC LIMIT 1;";
		sqlite3_stmt* stmt;
		if (DEBUG)
			std::cout << sql_raw << std::endl;
		if (sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmt, 0) != SQLITE_OK)
		{
			std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
			return;
		}
		if (sqlite3_step(stmt) == SQLITE_ROW)
			this->id = sqlite3_column_int(stmt, 0);
	}
	void save(){
		prepare_params();
	};
	int get_id();
	void remove(){};
	template<class T>
	static std::string get_table_name()
	{
		std::string buf = typeid(T).name();
		buf.replace(0, 6, "");
		char last_char = buf[buf.length() - 1];
		char modify_sym = 'y';
		if (last_char == modify_sym)
			buf = buf.substr(0, buf.length() - 1) + "ie";
		return buf + "s";
	};
	template<class T>
	static T* get(...)
	{

		return new T();
	}
	
	template<class T>
	static T* choice(std::vector<std::string> fields, std::string sql_where="")
	{
		std::string select_fields = "`id`, ";
		if (std::find(fields.begin(), fields.end(), "id") != fields.end())
			select_fields = "";
		for (int i = 0; i < fields.size(); i++){
			select_fields += "`" + fields[i] + "`";
			if (fields.size() - 1 > i)
				select_fields += ", ";
		}

		std::string sql_raw = "SELECT " + select_fields + " FROM " + BaseSQLiteModel::get_table_name<T>();
		if (sql_where.length() > 0)
			sql_raw += " " + sql_where;
		sql_raw += ";";
		if (DEBUG)
			std::cout << sql_raw << std::endl;
		sqlite3_stmt* stmt;
		int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmt, 0), i;
		if (rc != SQLITE_OK){
			std::cout << sqlite3_errmsg(db_link) << std::endl; return new T();
		}
		rc = 0;
		int selected = 0, current_id, index = 0;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			rc++;
			index++;
			current_id = sqlite3_column_int(stmt, 0);
			printf("%-5d|", current_id);
			for (i = 0; i < fields.size(); i++)
				printf("  %-30s|", (char*)sqlite3_column_text(stmt, i+1));
				//std::cout << std::string((char*)sqlite3_column_text(stmt, i)) << " ";
			std::cout << std::endl;
			if (index % 10 == 0)
			{
				printf("------------------------------------------------------------\n");
				printf("  %-5d Next items.\n", 0);
				printf("------------------------------------------------------------\n");
				index = 0;
				std::cout << "Enter choice number: ";
				std::cin.ignore();
				std::cin >> selected;
				std::cout << std::endl;
			}
			if (selected){
				sqlite3_finalize(stmt);
				return T::get_by_id(selected);
				break;
			}
		}
		if (rc == 0)
			return NULL;
		if (!selected){
			if (index % 10 != 0)
			{
				printf("------------------------------------------------------------\n");
				printf("%-4s Go to start of list\n\n", "0");
				printf("------------------------------------------------------------\n");
				index = 1;
				printf("Enter choice number: ");
				std::cin >> selected;
				std::cin.ignore();
				std::cout << std::endl;
			}
			sqlite3_finalize(stmt);
			if (!selected)
			{
				return BaseSQLiteModel::choice<T>(fields);
			}
			return T::get_by_id(selected);

		}
		if (DEBUG)
			std::cout << "Current count: " << rc << std::endl;
		return new T();
	}
};