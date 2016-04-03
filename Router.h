#pragma once
#include <iostream>
#include <conio.h>
#include <vector>
#include <string.h>
#include <string>


typedef void(*handler)();


class Router
{
private:
	std::string title;
	std::string route;
	int level;
	handler view = NULL;
	std::vector <Router> items;
public:
	Router* parent = NULL;
	int step;
	Router(std::string value);
	Router(handler func, std::string title_value);

	std::string getTitle();

	int getLevel();

	bool is_exit(int position);

	bool is_back(int position);

	void setActive(int* position);

	void run();

	void setLevel(int value);

	void addRoute(Router* router);

	void displayMenu(int position);

	int getMenuSize();

	void navigate();
};

