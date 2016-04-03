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

std::string Hotel::get_phone()
{
	return phone;
};

int Hotel::get_city_id()
{
	return city_id;
};

City* Hotel::get_city()
{
	return City::get_by_id(city_id);
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
	this->description = value;
};

void Hotel::set_situated(std::string value)
{
	this->situated = value;
};

void Hotel::set_phone(std::string value)
{
	this->phone = value;
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
Hotel* Hotel::create(std::string title, std::string phone, std::string situated, int city_id, std::string description, int position)
{
	Hotel* hotel = new Hotel();
	hotel->title = title;
	hotel->phone = phone;
	hotel->situated = situated;
	hotel->city_id = city_id;
	hotel->description = description;
	hotel->position = position;
	hotel->save();
	hotel->bind_id(Hotel::get_table_name());
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


std::vector<Hotel*> Hotel::get_by_city(City* city)
{
	std::vector<Hotel*> results;
	std::string sql_raw = "SELECT * FROM `" + Hotel::get_table_name() + "` WHERE `city_id`=? ORDER BY `title`;";
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmt, 0) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		return results;
	};
	if (sqlite3_bind_int(stmt, 1, city->get_id()) == SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		return results;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		Hotel* hotel = new Hotel();
		hotel->load_from_stmt(stmt);
		results.push_back(hotel);
	}
	sqlite3_finalize(stmt);
	return results;
};

void Hotel::prepare_params()
{

	sqlite3_stmt *stmp;
	std::string sql_raw;
	if (id)
		sql_raw = "UPDATE `" + get_table_name() + "`\
				   SET `title`=? SET `city_id`=? SET `situated`=? SET `description`=? SET `phone`=? SET `position`=? \
				   FROM id=?;";
	else
		sql_raw = "INSERT INTO `" + get_table_name() + "`\
				   (`title`, `city_id`, `situated`, `description`, `phone`, `position`) \
					VALUES(?, ?, ?, ?, ? ,?);";
	std::cout << sql_raw << std::endl;
	int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmp, 0);
	// data binding
	if (sqlite3_bind_text(stmp, 1, title.c_str(), -1, 0) != SQLITE_OK){
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		std::cout << "TITLE: " << title << std::endl;
	}
	if (sqlite3_bind_int(stmp, 2, city_id) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		std::cout << "city_id: " << city_id << std::endl;
	}
	if (sqlite3_bind_text(stmp, 3, situated.c_str(), -1, 0) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		std::cout << "situated: " << situated << std::endl;
	}
	if (sqlite3_bind_text(stmp, 4, description.c_str(), -1, 0) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		std::cout << "Descr: " << description << std::endl;
	}
	if (sqlite3_bind_text(stmp, 5, phone.c_str(), -1, 0) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		std::cout << "Phone: " << phone << std::endl;
	}
	if (sqlite3_bind_int(stmp, 6, position) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		std::cout << "position: " << position << std::endl;
	}
	if (id && sqlite3_bind_int(stmp, 7, id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	// exec sql raw
	if (sqlite3_step(stmp) != SQLITE_DONE)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	else
		std::cout << "Success execute" << std::endl;
	sqlite3_finalize(stmp);
};

void Hotel::load_from_stmt(sqlite3_stmt* stmt)
{
	this->id = sqlite3_column_int(stmt, 0);
	this->title = std::string((char*)sqlite3_column_text(stmt, 1));
	this->phone = std::string((char*)sqlite3_column_text(stmt, 2));
	this->description = std::string((char*)sqlite3_column_text(stmt, 3));
	this->position = sqlite3_column_int(stmt, 4);
	this->situated = std::string((char*)sqlite3_column_text(stmt, 5));
	this->description = std::string((char*)sqlite3_column_text(stmt, 6));
	this->city_id = sqlite3_column_int(stmt, 7);
};

std::vector<std::string> Hotel::db_sorted_fields = { "id", "title", "phone", "description", "position", "situated", "description", "city_id"};
