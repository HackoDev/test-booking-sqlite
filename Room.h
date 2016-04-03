#pragma once
#include "BaseSQLiteModel.h"
#include "Hotel.h"

class Room :
	public BaseSQLiteModel
{
private:
	// db fields
	std::string title;
	std::string description;
	int hotel_id;
	int position;
	float price;
public:
	Room();
	~Room();

	// getters
	int get_hotel_id();
	Hotel* get_hotel();
	int get_position();
	float get_price();
	std::string get_title();
	std::string get_description();

	// setters
	void set_title(std::string);
	void set_hotel_id(int);
	void set_hotel_id(Hotel*);
	void set_position(int value);
	void set_price(float value);
	// managers
	static Room* create(std::string title, std::string description, int hotel_id, int position, float price);
	static std::string get_table_name();
	static Room* get_by_id(int);
	static Room* choice(std::vector<std::string>);
	static void check_free_by_date(std::string);
	void prepare_params();
	void load_from_stmt(sqlite3_stmt*);
	static std::vector<std::string> db_sorted_fields;
};

