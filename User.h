#pragma once
#include <string.h>
#include <string>
#include <vector>
#include <ctime>
#include "BaseSQLiteModel.h"

class User: 
	public BaseSQLiteModel
{
private:
	//db fiedls
	std::string first_name;
	std::string second_name;
	std::string middle_name;
	std::string phone;
	std::string date_joined;
	std::string password;
public:
	// constructors
	User();
	static User* global_auth_uther;
	// inheritance
	static User* create(std::string first_name, std::string second_name, std::string middle_name, std::string phone, std::string password);

	// destructors
	~User();
	
	// getters
	std::string get_first_name();
	std::string get_second_name();
	std::string get_middle_name();
	std::string get_phone();
	std::string get_full_name();
	std::string get_date_joined();

	// setters
	void set_first_name(std::string value);
	void set_second_name(std::string value);
	void set_middle_name(std::string value);
	void set_date_joined(std::string value); 
	void set_date_joined(time_t value);
	void set_phone(std::string value);
	
	// set password as hash
	void set_password(std::string password);
	static std::string make_password(std::string origin_password);

	// managers
	static const std::string username_field;
	void load_from_stmt(sqlite3_stmt* stmt);
	static std::string get_table_name();
	static User* get_by_id(int id);
	static User* choice(std::vector<std::string> fields);
	void prepare_params();
	static bool check_phone(std::string phone);
	static User* authenticate(std::string username, std::string password);
};

