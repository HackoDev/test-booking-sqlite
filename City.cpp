#include "globals.h"
#include "City.h"
#include "Country.h"

City::City()
{
}


City::~City()
{
}


std::string City::get_title()
{
	return title;
}

int City::get_country_id()
{
	return country_id;
}

Country* City::get_country()
{
	return Country::get_by_id(id);
}

void City::set_country(Country* country)
{
	id = country->get_id();
};

void City::set_title(std::string value)
{
	title = value;
}

std::string City::get_table_name()
{
	return BaseSQLiteModel::get_table_name<City>();
};

void City::load_from_stmt(sqlite3_stmt *stmt)
{
	this->id = sqlite3_column_int(stmt, 0);
	this->country_id = sqlite3_column_int(stmt, 1);
	this->title = std::string((char*)sqlite3_column_text(stmt, 2));
}
City* City::get_by_id(int id)
{
	return BaseSQLiteModel::get_by_id<City>(id);
};

void City::prepare_params()
{
	sqlite3_stmt *stmp;
	std::string sql_raw;
	if (id)
		sql_raw = "UPDATE `" + get_table_name() + "`\
				   SET `title`=? SET `country_id`=?  \
				   FROM id=?;";
	else
		sql_raw = "INSERT INTO `" + get_table_name() + "`\
				   (`title`, `country_id`) \
				   VALUES(?, ?);";
	if (DEBUG)
		std::cout << sql_raw << std::endl;
	int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmp, 0);
	// data binding
	if (sqlite3_bind_text(stmp, 1, title.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_int(stmp, 2, country_id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (id && sqlite3_bind_int(stmp, 3, id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	// exec sql raw
	if (sqlite3_step(stmp) != SQLITE_DONE)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	else
		std::cout << "Success execute" << std::endl;
	sqlite3_finalize(stmp);
};

City* City::create(int country_id, std::string title)
{
	City* city = new City();
	city->title = title;
	city->country_id = country_id;
	city->save();
	city->bind_id(City::get_table_name());
	return city;
};

City* City::choice(std::vector<std::string> fields)
{
	return BaseSQLiteModel::choice<City>(fields);
}

std::vector<std::string> City::db_sorted_fields = { "id", "country_id", "title" };