#pragma once
#include <string.h>
#include <string>
#include "BaseSQLiteModel.h"
#include "City.h"
#include "Country.h"
#include "User.h"


class Hotel :
	public BaseSQLiteModel
{
private:
	// db fields
	int city_id;
	std::string title;
	std::string description;
	std::string situated;
	std::string director;
public:
	Hotel();
	~Hotel();

	// getters
	std::string get_title();
	std::string get_description();
	std::string get_situated();
	std::string get_director();
	int get_city_id();
	City* get_city();
	Country* get_country();

	// setters
	void set_title(std::string);
	void set_description(std::string);
	void set_situated(std::string);
	void set_director(std::string);
	void set_city_id(int);
	void set_city(City*);

	// managers
	static Hotel* create(std::string, std::string, std::string, int, std::string);
	static std::string get_table_name();
	static Hotel* get_by_id(int);
	static Hotel* choice(std::vector<std::string>);
	static void show_by_user(User*);
	static bool check_free_by_date(std::string);
	void prepare_params();
	void load_from_stmt(sqlite3_stmt*);
	static std::vector<std::string> db_sorted_fields;
};

