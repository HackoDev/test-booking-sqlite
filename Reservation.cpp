#include <string.h>
#include <string>
#include "BaseSQLiteModel.h"
#include "Reservation.h"
#include "City.h"
#include "Room.h"


Reservation::Reservation()
{
}


Reservation::~Reservation()
{
}

std::string Reservation::get_start_date()
{
	return start_date;
};

std::string Reservation::get_end_date()
{
	return end_date;
};

int Reservation::get_country_id()
{
	return get_country()->get_id();
};

int Reservation::get_city_id()
{
	return city_id;
};

City* Reservation::get_city()
{
	return BaseSQLiteModel::get_by_id<City>(city_id); 
};

Country* Reservation::get_country()
{
	return Country::get_by_id(this->get_city()->get_country_id());
};

Room* Reservation::get_room()
{
	std::cout << "Room id: " << room_id << std::endl;
	return Room::get_by_id(room_id);
};

Hotel* Reservation::get_hotel()
{
	return this->get_room()->get_hotel();
};

void Reservation::set_start_date(std::string value)
{
	this->start_date = value;
}

void Reservation::set_start_date(std::time_t value)
{
	std::time_t now = std::time(NULL);
	std::tm * ptm = std::localtime(&now);
	char buffer[32];
	// Format: Mo, 2009-06-15 20:20:00
	std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);
	this->start_date = std::string(buffer);
}

void Reservation::set_end_date(std::string value)
{
	this->end_date = value;
}

void Reservation::set_end_date(std::time_t value)
{
	std::time_t now = std::time(NULL);
	std::tm * ptm = std::localtime(&now);
	char buffer[32];
	std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);
	this->end_date = std::string(buffer);
}

void Reservation::set_price(float value)
{
	this->price = value;
}

void Reservation::set_note(std::string value)
{
	this->note = value;
}

float Reservation::get_price()
{
	return price;
}

std::string Reservation::get_note()
{
	return note;
}

Reservation* Reservation::create(int user_id, int room_id, std::string start_date, std::string end_date, std::string note)
{
	Reservation* reservation = new Reservation();
	reservation->user_id = user_id;
	reservation->room_id = room_id;
	reservation->start_date = start_date;
	reservation->end_date = end_date;
	reservation->note = note;
	reservation->save();
	reservation->bind_id(Reservation::get_table_name());
	return reservation;
};

std::string Reservation::get_table_name()
{
	return BaseSQLiteModel::get_table_name<Reservation>();
};

Reservation* Reservation::get_by_id(int id)
{
	return BaseSQLiteModel::get_by_id<Reservation>(id);
};

Reservation* Reservation::choice(std::vector<std::string> fiedls)
{
	return BaseSQLiteModel::choice<Reservation>(fiedls);
};

void Reservation::show_by_user(User* user)
{
	
};

void Reservation::check_free_by_date(std::string)
{
	
};

void Reservation::prepare_params()
{
	sqlite3_stmt *stmp;
	std::string sql_raw;
	if (id)
		sql_raw = "UPDATE `" + get_table_name() + "`\
				   SET `user_id`=? `room_id`=? SET `start_date`=? SET `end_date`=? SET `note`=? \
				   FROM `id`=?;";
	else
		sql_raw = "INSERT INTO `" + get_table_name() + "`\
				   (`user_id`, `room_id`, `start_date`, `end_date`, `note`) \
				    VALUES(?, ?, ?, ?, ?);";
	std::cout << sql_raw << std::endl;
	int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmp, 0);
	// data binding
	if (sqlite3_bind_int(stmp, 1, user_id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_int(stmp, 2, room_id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 3, start_date.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 4, end_date.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 5, note.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (id && sqlite3_bind_int(stmp, 6, id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	// exec sql raw
	if (sqlite3_step(stmp) != SQLITE_DONE)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	else
		std::cout << "Success execute" << std::endl;
	sqlite3_finalize(stmp);
};
void Reservation::load_from_stmt(sqlite3_stmt* stmt)
{
	this->id = sqlite3_column_int(stmt, 0);
	this->user_id = sqlite3_column_int(stmt, 1);
	this->room_id = sqlite3_column_int(stmt, 2);
	this->start_date = std::string((char*)sqlite3_column_text(stmt, 3));
	this->end_date= std::string((char*)sqlite3_column_text(stmt, 4));
};

std::vector<std::string> Reservation::db_sorted_fields = { "id", "user_id", "room_id", "start_date", "end_date", "note"};
