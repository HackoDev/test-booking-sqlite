#include "Reservation.h"
#include "City.h"


Reservation::Reservation()
{
}


Reservation::~Reservation()
{
}

std::string Reservation::get_datetime()
{
	return datetime;
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
	return Country::get_by_id(get_city()->get_country_id);
};

Room* Reservation::get_room()
{
	return Room::get_by_id(room_id);
};

Hotel* Reservation::get_hotel()
{
	return Hotel::get_by_id(get_room()->get_hotel_id());
};

Reservation* Reservation::create(int user_id, int room_id, std::string datetime)
{
	Reservation* reservation = new Reservation();
	reservation->user_id = user_id;
	reservation->room_id = room_id;
	reservation->datetime = datetime;
	reservation->save();
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

void show_by_user(User* user)
{
	
};

void check_free_by_date(std::string)
{
	
};

void prepare_params()
{

};
void load_from_stmt(sqlite3_stmt*)
{

};

std::vector<std::string> db_sorted_fields = {"id", "user_id", "room_id", "datetime"};