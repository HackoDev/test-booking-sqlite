#include "globals.h"
#include "User.h"
#include <string.h>
#include <string>
#include <ctime>

User::User()
{
}


User::~User()
{
}

std::string User::get_first_name()
{
	return first_name;
};

std::string User::get_second_name()
{
	return second_name;
};

std::string User::get_middle_name()
{
	return middle_name;
};

std::string User::get_full_name()
{
	return second_name + " " + first_name + " " + middle_name;
};

std::string User::get_date_joined()
{
	return date_joined;
};

void User::set_first_name(std::string value)
{
	first_name = value;
};

void User::set_second_name(std::string value)
{
	second_name = value;
};

void User::set_middle_name(std::string value)
{
	middle_name = value;
};

void User::set_date_joined(std::string value)
{
	date_joined = value;
};

void User::set_date_joined(time_t value)
{
	std::time_t now = std::time(NULL);
	std::tm * ptm = std::localtime(&now);
	char buffer[32];
	// Format: Mo, 2009-06-15 20:20:00
	std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);
	date_joined = std::string(buffer);
};

void User::set_password(std::string value)
{
	// TODO: set password hash
	password = User::make_password(value);
}
void User::set_phone(std::string value)
{
	phone = value;
};

std::string User::get_table_name()
{
	return BaseSQLiteModel::get_table_name<User>();
}

User* User::choice(std::vector<std::string> fields)
{
	return BaseSQLiteModel::choice<User>(fields);
}

User* User::create(std::string first_name, std::string second_name, std::string middle_name, std::string phone, std::string password)
{
	User* user = new User();
	user->first_name = first_name;
	user->second_name = second_name;
	user->middle_name = middle_name;
	user->phone = phone;
	user->set_date_joined(time(0));
	std::cout << "Date joined: " << user->date_joined << std::endl;
	user->set_password(password);
	user->save();
	user->bind_id(User::get_table_name());
	return user;
};

void User::prepare_params()
{
	sqlite3_stmt *stmp;
	std::string sql_raw;
	if (id)
		sql_raw = "UPDATE `" + get_table_name() + "`\
				   SET `first_name`=? SET `second_name`=? SET `middle_name`=? SET `phone`=? SET `date_joined`=? SET `password`=? \
				   FROM id=?;";
	else
		sql_raw = "INSERT INTO `" + get_table_name() + "`\
			   (`first_name`, `second_name`, `middle_name`, `phone`, `date_joined`, `password`) \
				VALUES(?, ?, ?, ?, ?, ?);";
	std::cout << sql_raw << std::endl;
	int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmp, 0);
	if (sqlite3_bind_text(stmp, 1, first_name.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 2, second_name.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 3, middle_name.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 4, phone.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 5, date_joined.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 6, password.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (id && sqlite3_bind_int(stmp, 7, id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_step(stmp) != SQLITE_DONE)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	else
		if (DEBUG)
			std::cout << "Success execute" << std::endl;
	sqlite3_finalize(stmp);
}

void User::load_from_stmt(sqlite3_stmt *stmt)
{
	this->id = sqlite3_column_int(stmt, 0);
	this->first_name = std::string((char*)sqlite3_column_text(stmt, 1));
	this->second_name = std::string((char*)sqlite3_column_text(stmt, 2));
	this->middle_name = std::string((char*)sqlite3_column_text(stmt, 3));
	this->phone = std::string((char*)sqlite3_column_text(stmt, 4));
	this->date_joined = std::string((char*)sqlite3_column_text(stmt, 5));

}
User* User::get_by_id(int id)
{
	return BaseSQLiteModel::get_by_id<User>(id);
};

bool User::check_phone(std::string phone)
{
	sqlite3_stmt* stmt;
	std::string sql_raw = "SELECT id FROM `" + User::get_table_name() + "` WHERE phone=?;";
	if (sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmt, 0) == SQLITE_OK)
	{
		if (sqlite3_bind_text(stmt, 1, phone.c_str(), -1, 0) != SQLITE_OK){
			std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		}
		bool result = sqlite3_step(stmt) == SQLITE_ROW;
		sqlite3_finalize(stmt);
		return result;
	};
	return true;
};

std::string User::make_password(std::string origin_password)
{
	//TODO: create hash algorithm
	return origin_password;
};

User* User::authenticate(std::string username, std::string password)
{
	std::string sql_raw = "SELECT `id`, `" + User::username_field + "` FROM `" + User::get_table_name() + "` WHERE `" + User::username_field + "`=? AND `password`=?;";
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmt, 0) == SQLITE_OK)
	{
		std::cout << "eeee" << std::endl;
		if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, 0) != SQLITE_OK)
		{
			std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
			return NULL;
		};
		if (sqlite3_bind_text(stmt, 2, password.c_str(), -1, 0) != SQLITE_OK)
		{
			std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
			return NULL;
		};
		if (sqlite3_step(stmt) == SQLITE_ROW){
			int user_id = sqlite3_column_int(stmt, 0);
			sqlite3_finalize(stmt);
			return User::get_by_id(user_id);
		}
		return NULL;
	};
	return NULL;
};

const std::string User::username_field = "phone";

User* User::global_auth_uther = NULL;

std::vector<std::string> User::db_sorted_fields = {"id", "first_name", "second_name", "middle_name", "phone", "date_joined"};