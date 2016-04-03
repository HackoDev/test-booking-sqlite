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
	int position;
	std::string title;
	std::string description;
	std::string situated;
	std::string phone;
public:
	Hotel();
	~Hotel();

	// getters
	int get_position();
	std::string get_phone();
	std::string get_title();
	std::string get_description();
	std::string get_situated();
	int get_city_id();
	City* get_city();
	Country* get_country();

	// setters
	void set_position(int position);
	void set_title(std::string value);
	void set_description(std::string value);
	void set_situated(std::string value);
	void set_phone(std::string value);
	void set_city_id(int value);
	void set_city(City* value);

	// managers
	static Hotel* create(std::string title, std::string phone, std::string situated, int city_id, std::string description, int position);
	static std::string get_table_name();
	static Hotel* get_by_id(int);
	static Hotel* choice(std::vector<std::string>);
	static void show_by_user(User*);
	static bool check_free_by_date(std::string);
	void prepare_params();
	void load_from_stmt(sqlite3_stmt*);
	static std::vector<std::string> db_sorted_fields;
	static std::vector<Hotel*> get_by_city(City* city);
};

