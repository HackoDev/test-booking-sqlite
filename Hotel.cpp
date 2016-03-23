#include "Hotel.h"
#include <iostream>
#include <string.h>
#include <string>


Hotel::Hotel()
{
}


Hotel::~Hotel()
{
}

std::string Hotel::get_title()
{
	return title;
}

std::string Hotel::get_description()
{
	return description;
};

std::string Hotel::get_situated()
{
	return situated;
};

std::string Hotel::get_director()
{
	return director;
};

int Hotel::get_city_id()
{
	return city_id;
};

City* Hotel::get_city()
{
	return BaseSQLiteModel::get_by_id<City>(city_id);
};

Country* Hotel::get_country()
{
	return BaseSQLiteModel::get_by_id<Country>(this->get_city()->get_country_id());
};

// setters
void Hotel::set_title(std::string value)
{
	this->title = value;
};

void Hotel::set_description(std::string value)
{
	this->director = value;
};

void Hotel::set_situated(std::string value)
{
	this->situated = value;
};

void Hotel::set_director(std::string value)
{
	this->director = value;
};

void Hotel::set_city_id(int value)
{
	this->city_id = value;
};

void Hotel::set_city(City* value)
{
	this->city_id = value->get_id();
};

// managers
Hotel* Hotel::create(std::string title, std::string director, std::string situated, int city_id, std::string description)
{
	Hotel* hotel = new Hotel();
	hotel->title = title;
	hotel->director = director;
	hotel->situated = situated;
	hotel->city_id = city_id;
	hotel->description = description;
	return hotel;
};

std::string Hotel::get_table_name()
{
	return BaseSQLiteModel::get_table_name<Hotel>();
};

Hotel* Hotel::get_by_id(int value)
{
	return BaseSQLiteModel::get_by_id<Hotel>(value);
};

Hotel* Hotel::choice(std::vector<std::string> fiedls)
{
	return BaseSQLiteModel::choice<Hotel>(fiedls);
};

void Hotel::show_by_user(User* user)
{
	
};

bool Hotel::check_free_by_date(std::string)
{
	return true;
};

void Hotel::prepare_params()
{
	
};

void Hotel::load_from_stmt(sqlite3_stmt* stmt)
{
	Hotel* hotel = new Hotel();
	this->id = sqlite3_column_int(stmt, 0);
	this->title = std::string((char*)sqlite3_column_text(stmt, 1));
	this->director = std::string((char*)sqlite3_column_text(stmt, 2));
	this->description= std::string((char*)sqlite3_column_text(stmt, 3));
	this->situated = std::string((char*)sqlite3_column_text(stmt, 4));
	this->description = std::string((char*)sqlite3_column_text(stmt, 5));
	this->city_id = sqlite3_column_int(stmt, 6);
};

std::vector<std::string> Hotel::db_sorted_fields = { "id", "title", "director", "description", "situated", "description", "city_id"};
