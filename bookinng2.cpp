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
#include <locale>

using namespace std;

char* zErrMsg = NULL;

void show_admin_route()
{
	std::cout << "Admin route";
}

void admin_add_country()
{
	std::string title;
	std::cout << "Admin: add country" << std::endl;
	std::cout << "Enter country title: ";
	std::cin >> title;
	Country::create(title);
	std::cout << std::endl;
}

void admin_add_city()
{
	std::cout << "Admin: add city" << std::endl;
	std::string title;
	std::cout << "Enter city title: ";
	std::cin >> title;
	vector<string> fields = {"title"};
	Country* country = Country::choice(fields);
	if (country == NULL)
	{
		std::cout << "This city has not added" << std::endl;
		return;
	}
	country->get_id();
	City::create(country->get_id(), title);
	std::cout << std::endl;
}

void admin_add_hotel()
{
	std::cout << "Admin: add hotel" << std::endl;
}

void admin_add_room()
{
	std::cout << "Admin: add room" << std::endl;
}

void admin_show_reservations()
{
	std::cout << "Admin: show reservations" << std::endl;
}

void show_hotels_by_country()
{

	std::cout << "Show hotels by country" << std::endl;
}

void show_city_by_city()
{
	std::cout << "Show hotels by city" << std::endl;
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

void registration_user()
{
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
	User::create(first_name, second_name, middle_name, phone, password1);
	std::cout << "We successfully registrated." << std::endl;
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
	char* table_hotels = "CREATE TABLE Hotels( id INTEGER PRIMARY KEY AUTOINCREMENT, title CHAR(1024) NOT NULL, phone CHAR(32), city_id INT NOT NULL, position INT NOT NULL, description VARCHAR(2048) NOT NULL, FOREIGN KEY(city_id) REFERENCES Cities(id));";
	char* table_rooms = "CREATE TABLE Rooms( id INTEGER PRIMARY KEY AUTOINCREMENT, hotel_id INT NOT NULL, price FLOAT NOT NULL, position int INT NOT NULL, description VARCHAR(2048) NOT NULL, FOREIGN KEY(hotel_id) REFERENCES Hotels(id) );";
	char* table_reservations = "CREATE TABLE Reservations( id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INT NOT NULL, room_id INT NOT NULL, start_date DATETIME NOT NULL, end_date DATETIME NOT NULL, price FLOAT NOT NULL, note VARCHAR(2048) NOT NULL, FOREIGN KEY(user_id) REFERENCES Users(id), FOREIGN KEY(room_id) REFERENCES Rooms(id) );";
	int rc;
	rc = sqlite3_exec(db, table_countries, NULL, 0, &zErrMsg);
	check_state(rc, table_countries);
	rc = sqlite3_exec(db, table_cities, NULL, 0, &zErrMsg);
	check_state(rc, table_cities);
	cout << "Create table users" << std::endl;
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
	setlocale(LC_ALL, "Rusian");
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);

	bool need_initial = !(bool)ifstream("booking.sqlite");
	int rc = sqlite3_open("booking.sqlite", &db_link);
	if (need_initial)
		initial_structure(db_link);
	//registration_user();
	login_user();
	system("pause");
	return 0;
}