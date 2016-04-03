#include "Room.h"
#include "Hotel.h"

Room::Room()
{
}


Room::~Room()
{
}

// getters
int Room::get_hotel_id()
{
	return hotel_id;
};

Hotel* Room::get_hotel()
{
	return Hotel::get_by_id(hotel_id);
};

int Room::get_position()
{
	return position;
};

std::string Room::get_title()
{
	return title;
};

std::string Room::get_description()
{
	return description;
};

float Room::get_price()
{
	return price;
}

// setters
void Room::set_title(std::string value)
{
	title = value;
};

void Room::set_hotel_id(int value)
{
	hotel_id = value;
};

void Room::set_hotel_id(Hotel* value)
{
	hotel_id = value->get_id();
};

void Room::set_position(int value)
{
	position = position;
};

void Room::set_price(float value)
{
	price = value;
}

// managers
Room* Room::create(std::string title, std::string description, int hotel_id, int position, float price)
{
	Room* room = new Room();
	room->title = title;
	room->description = description;
	room->hotel_id = hotel_id;
	room->position = position;
	room->price = price;
	room->save();
	room->bind_id(Room::get_table_name());
	return room;
};

std::string Room::get_table_name()
{
	return BaseSQLiteModel::get_table_name<Room>();
};

Room* Room::get_by_id(int value)
{
	return BaseSQLiteModel::get_by_id<Room>(value);
};

void Room::check_free_by_date(std::string)
{

};

void Room::prepare_params()
{
	sqlite3_stmt *stmp;
	std::string sql_raw;
	if (id)
		sql_raw = "UPDATE `" + get_table_name() + "`\
				   SET `title`=? SET `description`=? SET `hotel_id`=? SET `position`=? SET `price`=? \
				   FROM id=?;";
	else
		sql_raw = "INSERT INTO `" + get_table_name() + "`\
				   (`title`, `description`, `hotel_id`, `position`, `price`) \
				    VALUES(?, ?, ?, ?, ?);";
	std::cout << sql_raw << std::endl;
	int rc = sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmp, 0);
	// data binding
	if (sqlite3_bind_text(stmp, 1, title.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_text(stmp, 2, description.c_str(), -1, 0) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_int(stmp, 3, hotel_id) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_int(stmp, 4, position) != SQLITE_OK)
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
	if (sqlite3_bind_int(stmp, 5, price) != SQLITE_OK)
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
void Room::load_from_stmt(sqlite3_stmt* stmt)
{
	this->id = sqlite3_column_int(stmt, 0);
	this->title = std::string((char*)sqlite3_column_text(stmt, 1));
	this->description = std::string((char*)sqlite3_column_text(stmt, 2));
	this->hotel_id = sqlite3_column_int(stmt, 3);
	this->position = sqlite3_column_int(stmt, 4);
	this->price = sqlite3_column_int(stmt, 5);
};

std::vector<std::string> Room::db_sorted_fields = {"id", "title", "description", "hotel_id", "position", "price"};