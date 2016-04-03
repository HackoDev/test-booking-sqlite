// bookinng2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "globals.h"
#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include "User.h"
#include <windows.h>
#include <cstdlib>
#include "Country.h"
#include "City.h"
#include "BaseSQLiteModel.h"
#include "Reservation.h"
#include "Hotel.h"
#include "Router.h"
#include <locale>

using namespace std;

char* zErrMsg = NULL;

void show_admin_route()
{
	std::cout << "Admin route";
}

void login_user()
{
	if (User::global_auth_uther == NULL)
	{
		std::cout << "Login" << std::endl;
		std::string phone, password;
		std::cout << "Enter your phone: ";
		std::cin >> phone;
		std::cout << std::endl;
		std::cout << "Enter your password: ";
		std::cin >> password;
		std::cout << std::endl;
		User* user = User::authenticate(phone, password);
		User::global_auth_uther = user;
		if (user == NULL)
			std::cout << "Incorrect phone or password user..." << std::endl;
		else
			std::cout << "Welcome, " + user->get_full_name() + "!" << std::endl;
	}
	else
		std::cout << "Welcome, " + User::global_auth_uther->get_full_name() + "!" << std::endl;
}

void logout_user()
{
	std::cout << "Logout user" << std::endl;
	if (User::global_auth_uther == NULL)
		return;
	std::cout << "Buy, " << User::global_auth_uther->get_full_name() << "!" << std::endl;
	User::global_auth_uther = NULL;
}

void registration_user()
{
	if (User::global_auth_uther != NULL)
		logout_user();
	std::string first_name, second_name, middle_name, phone, password1 = "", password2 = "";
	bool exists_phone = true;
	std::cout << "Registration" << std::endl;
	std::cout << "Enter your first name: ";
	std::cin >> first_name;
	std::cout << std::endl;
	std::cout << "Enter your second name: ";
	std::cin >> second_name;
	std::cout << std::endl;
	std::cout << "Enter your middle name: ";
	std::cin >> middle_name;
	std::cout << std::endl;
	// check phone as login
	while (exists_phone)
	{
		std::cout << "Enter your phone number: ";
		std::cin >> phone;
		std::cout << std::endl;
		exists_phone = User::check_phone(phone);
		if (exists_phone)
			std::cout << "Phone already exists. Retype phone number..." << std::endl;
	}
	while ((password1 == "") || (password2 == "") || (password1 != password2))
	{
		std::cout << "Enter your password: ";
		std::cin >> password1;
		std::cout << std::endl;
		std::cout << "Retype password: ";
		std::cin >> password2;
		std::cout << std::endl;
		if (password1 != password2)
			std::cout << "Error set password. Please retype yor password..." << std::endl;
	}
	User::global_auth_uther = User::create(first_name, second_name, middle_name, phone, password1);
	std::cout << "We successfully registrated." << std::endl;
}

void admin_add_country()
{
	std::string title;
	std::cout << "Admin: add country" << std::endl;
	std::cout << "Enter country title: ";
	std::getline(std::cin, title);
	std::cout << std::endl;
	Country::create(title);
	std::cout << std::endl;
}

void admin_add_city()
{
	std::cout << "Admin: add city" << std::endl;
	std::string title;
	std::cout << "Enter city title: ";
	std::getline(std::cin, title);
	Country* country = Country::choice({ "title" });
	if (country == NULL)
	{
		std::cout << "This city has not added" << std::endl;
		return;
	}
	City* city = City::create(country->get_id(), title);
	std::cout << std::endl;
}

void admin_add_hotel()
{
	std::cout << "Admin: add hotel" << std::endl;
	std::string title, phone, situated, description;
	int position;
	City* city;
	std::cout << "Enter tiitle: ";
	std::getline(std::cin, title);
	std::cout << std::endl;
	std::cout << "Enter phone: ";
	std::getline(std::cin, phone);
	std::cout << std::endl;
	std::cout << "Enter situated (example 2016-10-01): ";
	std::getline(std::cin, situated);
	std::cout << std::endl;
	std::cout << "Enter description: ";
	std::getline(std::cin, description);
	std::cout << std::endl;
	std::cout << "Position: ";
	std::cin >> position;
	std::cout << std::endl;
	city = City::choice({ "title" });
	Hotel::create(title, phone, situated, city->get_id(), description, position);
	std::cout << "Hotel successfully added" << std::endl;
}

void admin_add_room()
{
	std::cout << "Admin: add room" << std::endl;
	std::string title, description;
	int level = 5;
	Hotel* hotel;
	float price;
	std::cout << "Enter title: ";
	std::getline(std::cin, title);
	std::cout << "Enter description: ";
	std::getline(std::cin, description);
	std::cout << "Enter price: ";
	std::cin >> price;
	std::cin.ignore();
	hotel = Hotel::choice({ "title", "description" });
	if (hotel == NULL)
	{
		std::cout << "No hotels" << std::endl;
		return;
	}
	Room::create(title, description, hotel->get_id(), level, price);
}

void add_reservations()
{
	std::cout << "Admin: add reservations" << std::endl;
	std::string start_date, end_date, note;
	std::cout << "Enter date start reservation: ";
	std::getline(std::cin, start_date);
	std::cout << "Enter date end reservation: ";
	std::getline(std::cin, end_date);
	std::cout << "Enter note: ";
	std::getline(std::cin, note);
	Room* room = Room::choice({ "title", "position" });
	if (room == NULL)
	{
		std::cout << "No room" << std::endl;
		return;
	}
	if (User::global_auth_uther == NULL)
	{
		int i;
		std::cout << "User not authenticated" << std::endl;
		std::cout << "1- login, 2- registration;" << std::endl;
		std::cin >> i;
		std::cin.ignore();
		if (i == 1)
			login_user();
		else
			registration_user();
		if (User::global_auth_uther == NULL)
		{
			std::cout << "Reservation not added" << std::endl;
			return;
		}
	}
	Reservation* reservation = Reservation::create(User::global_auth_uther->get_id(), room->get_id(), start_date, end_date, note);
	std::cout << "User: " << User::global_auth_uther->get_full_name() << ". Add reservation to \"" << reservation->get_hotel()->get_title() << ". Date range: " << reservation->get_start_date() << " to " << reservation->get_end_date() << "." << std::endl;
}

void show_hotels_by_country()
{	
	std::cout << "Show hotels by country" << std::endl;
	Country* country = Country::choice({ "title" });
	if (country == NULL)
	{
		std::cout << "No such country" << std::endl;
		return;
	}
	std::string fields = "`" + Hotel::get_table_name() + "`.`id`, `" + Hotel::get_table_name() + "`.`title`";
	std::string sql_raw = "SELECT " + fields + " FROM `" + Hotel::get_table_name() + "` INNER JOIN `" + City::get_table_name() + "` ON (`" + Hotel::get_table_name() + "`.`city_id`=`" + City::get_table_name() + "`.`id`) WHERE `" + City::get_table_name() + "`.`country_id`=? ORDER BY `" + Hotel::get_table_name() + "`.`title`";
	if (DEBUG)
		std::cout << sql_raw << std::endl;
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db_link, sql_raw.c_str(), -1, &stmt, 0) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		return ;
	};
	if (sqlite3_bind_int(stmt, 1, country->get_id()) != SQLITE_OK)
	{
		std::cout << "SQL error: " << sqlite3_errmsg(db_link) << std::endl;
		return;
	}
	std::vector<City*> cities;
	int i = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		if (i == 0)
		{
			printf("%-10s %-20s %-10s", "ID", "Title", "City");
		}
		i++;
		std::cout << "hotel id = " << sqlite3_column_name(stmt, 0) << std::endl;
		std::cout << "hotel id = " << sqlite3_column_int(stmt, 0) << std::endl;
		Hotel* hotel = Hotel::get_by_id(sqlite3_column_int(stmt, 0));
		printf("%-10s %-20s %-10s\n", hotel->get_id(), hotel->get_title(), hotel->get_city()->get_title());
	}

	sqlite3_finalize(stmt);

	if (i == 0)
	{
		std::cout << "No such hotels..." << std::endl;
		return;
	}
	int number;
	std::cin >> number;
	std::cin.ignore();
	Hotel::get_by_id(number);
}

void show_city_by_city()
{
	std::cout << "Show hotels by city" << std::endl;
}

void user_reservation()
{
	std::cout << "Show user reservations" << std::endl;

}

void add_reservation(int id)
{
	std::cout << "Add reservation" << std::endl;
}

void check_state(int result_code, char* query) {
	if (result_code == SQLITE_OK)
		return;
	cout << "Error query: ``" << query << "``" << endl;
}

void initial_structure(sqlite3* db) {
	char* table_countries = "CREATE TABLE Countries( id INTEGER PRIMARY KEY AUTOINCREMENT, title CHAR(256) );";
	char* table_cities = "CREATE TABLE Cities( id INTEGER PRIMARY KEY AUTOINCREMENT, country_id INT, title CHAR(256), FOREIGN KEY(country_id) REFERENCES Countries(id) );";
	char* table_users = "CREATE TABLE Users( id INTEGER PRIMARY KEY AUTOINCREMENT, first_name CHAR(256), second_name CHAR(256), middle_name CHAR(256), phone CHAR(32), password CHAR(1024), date_joined CHAR(32));";
	char* table_hotels = "CREATE TABLE Hotels( id INTEGER PRIMARY KEY AUTOINCREMENT, title CHAR(1024) NOT NULL, phone CHAR(32), city_id INT NOT NULL, position INT NOT NULL, description VARCHAR(2048) NOT NULL, situated DATE NOT NULL, FOREIGN KEY(city_id) REFERENCES Cities(id));";
	char* table_rooms = "CREATE TABLE Rooms( id INTEGER PRIMARY KEY AUTOINCREMENT, title CHAR(512), hotel_id INT NOT NULL, price FLOAT NOT NULL, position int INT NOT NULL, description VARCHAR(2048) NOT NULL, FOREIGN KEY(hotel_id) REFERENCES Hotels(id) );";
	char* table_reservations = "CREATE TABLE Reservations( id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INT NOT NULL, room_id INT NOT NULL, start_date DATE NOT NULL, end_date DATE NOT NULL, note VARCHAR(2048) NOT NULL, FOREIGN KEY(user_id) REFERENCES Users(id), FOREIGN KEY(room_id) REFERENCES Rooms(id) );";
	int rc;
	rc = sqlite3_exec(db, table_countries, NULL, 0, &zErrMsg);
	check_state(rc, table_countries);
	rc = sqlite3_exec(db, table_cities, NULL, 0, &zErrMsg);
	check_state(rc, table_cities);
	rc = sqlite3_exec(db, table_users, NULL, 0, &zErrMsg);
	check_state(rc, table_users);
	rc = sqlite3_exec(db, table_hotels, NULL, 0, &zErrMsg);
	check_state(rc, table_hotels);
	rc = sqlite3_exec(db, table_rooms, NULL, 0, &zErrMsg);
	check_state(rc, table_rooms);
	rc = sqlite3_exec(db, table_reservations, NULL, 0, &zErrMsg);
	check_state(rc, table_reservations);
}

int _tmain(int argc, _TCHAR* argv[])
{
//	setlocale(LC_ALL, "Russian");
	bool need_initial = !(bool)ifstream("bookings.sqlite");
	int rc = sqlite3_open("bookings.sqlite", &db_link);
	if (need_initial)
		initial_structure(db_link);
	
	// admin module
	Router* admin_main = new Router("Admin module");
	admin_main->addRoute(new Router(admin_add_city, "Admin: Add city"));
	admin_main->addRoute(new Router(admin_add_country, "Admin: Add country"));
	admin_main->addRoute(new Router(admin_add_hotel, "Admin: Add hotel"));
	admin_main->addRoute(new Router(admin_add_room, "Admin: Add room"));

	// user module
	Router* user_module = new Router("User module");
	user_module->addRoute(new Router(login_user, "Login"));
	user_module->addRoute(new Router(logout_user, "Logout"));
	user_module->addRoute(new Router(registration_user, "Registration"));
	user_module->addRoute(new Router(add_reservations, "Add reservation"));
	user_module->addRoute(new Router(show_hotels_by_country, "Show by city"));
	

	// main module
	Router* main = new Router("BOOKING-COM");
	main->addRoute(admin_main);
	main->addRoute(user_module);
	
	main->navigate();
	//show_hotels_by_country();
	system("pause");
	return 0;
}