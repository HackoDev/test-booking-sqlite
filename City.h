#pragma once
#include "BaseSQLiteModel.h"
#include "Country.h"

class City :
	public BaseSQLiteModel
{
private:
	// db fields
	std::string title;
	int country_id;
public:
	City();
	~City();

	// getters
	std::string get_title();
	int get_country_id();
	Country* get_country();

	// setters
	void set_title(std::string value);
	void set_country(int country_id);
	void set_country(Country* country);

	// managers
	static City* create(int country_id, std::string title);
	static std::string get_table_name();
	static City* get_by_id(int id);
	static City* choice(std::vector<std::string> fields);
	void prepare_params();
	void load_from_stmt(sqlite3_stmt *stmt);
	static std::vector<std::string> db_sorted_fields;
};
