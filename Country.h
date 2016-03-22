#pragma once
#include <vector>
#include <iostream>
#include "sqlite3.h"
#include "BaseSQLiteModel.h"

class Country:
	public BaseSQLiteModel
{
private:
	// db fields
	std::string title;
public:
	Country();
	~Country();

	// getters
	std::string get_title();

	// setters
	void set_title(std::string value);

	// managers
	static Country* create(std::string title);
	static std::string get_table_name();
	static Country* get_by_id(int id);
	static Country* choice(std::vector<std::string> fields);
	void prepare_params();
	void load_from_stmt(sqlite3_stmt *stmt);
	static std::vector<std::string> db_sorted_fields;
};

