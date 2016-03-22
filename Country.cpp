#include "Country.h"
#include "BaseSQLiteModel.h"

Country::Country()
{
}


Country::~Country()
{
}

std::string Country::get_title()
{
	return title;
}

void Country::set_title(std::string value)
{
	title = value;
}

std::string Country::get_table_name()
{
	return BaseSQLiteModel::get_table_name<Country>();
};

void Country::load_from_stmt(sqlite3_stmt *stmt)
{
	this->id = sqlite3_column_int(stmt, 0);
	this->title = std::string((char*)sqlite3_column_text(stmt, 1));
}
Country* Country::get_by_id(int id)
{
	return BaseSQLiteModel::get_by_id<Country>(id);
};

void Country::prepare_params()
{
	sqlite3_stmt *stmp;
	std::string sql_raw;
	if (id)
		sql_raw = "UPDATE `" + get_table_name() + "`\
				   SET `title`=?  \
				   FROM id=?;";
	else
		sql_raw = "INSERT INTO `" + get_table_name() + "`\
				   (`title`) \
				   VALUES(?);";
	std::cout << sql_raw << std::endl;
	int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmp, 0);
	// data binding
	if (sqlite3_bind_text(stmp, 1, title.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (id && sqlite3_bind_int(stmp, 2, id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	// exec sql raw
	if (sqlite3_step(stmp) != SQLITE_DONE)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	else
		std::cout << "Success execute" << std::endl;
	sqlite3_finalize(stmp);
};

Country* Country::create(std::string title)
{
	Country *country = new Country();
	country->title = title;
	country->save();
	return country;
};

Country* Country::choice(std::vector<std::string> fields)
{
	return BaseSQLiteModel::choice<Country>(fields);
}

std::vector<std::string> Country::db_sorted_fields = { "id", "title" };