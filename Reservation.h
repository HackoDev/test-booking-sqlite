#pragma once
#include <ctime>
#include "BaseSQLiteModel.h"
#include "Country.h"
#include "Hotel.h"
#include "User.h"
#include "Room.h"
#include "City.h"

class Reservation :
	public BaseSQLiteModel
{
private:
	// db fields
	int city_id;
	int user_id;
	int room_id;
	std::string datetime;
public:
	Reservation();
	~Reservation();

	// getters
	std::string get_datetime();
	int get_country_id();
	int get_city_id();
	City* get_city();
	Country* get_country();
	Room* get_room();
	Hotel* get_hotel();

	// setters
	void set_datetime(std::string);
	void set_datetime(time_t);
	void set_user(User*);
	void set_city(int);
	void set_city(City*);
	void set_room(int);
	void set_room(Room*);

	// managers
	static Reservation* create(int, int, std::string);
	static std::string get_table_name();
	static Reservation* get_by_id(int);
	static Reservation* choice(std::vector<std::string>);
	static void show_by_user(User*);
	static void check_free_by_date(std::string);
	void prepare_params();
	void load_from_stmt(sqlite3_stmt*);
	static std::vector<std::string> db_sorted_fields;
};

