#include "Router.h"

Router::Router(std::string value)
{
	this->title = value;
}

Router::Router(handler func, std::string title_value)
{
	title = title_value;
	view = func;
}
std::string Router::getTitle()
{
	return title;
}
int Router::getLevel()
{
	return level;
}
bool Router::is_exit(int position)
{
	return position == items.size() + (parent != NULL) ? 1 : 0;
}
bool Router::is_back(int position)
{
	return position == items.size() && parent != NULL;
}
void Router::setActive(int* position)
{
	if (is_exit(*position))
		exit(0);
	if (is_back(*position)){
		if (parent != NULL)
			parent->navigate();
		*position = 0;
		return;
	}

	Router* router = &(items[*position]);
	router->run();
	*position = 0;
	router->navigate();

}

void Router::run()
{
	system("CLS");
	if (view != NULL)
	{
		view();
		system("pause");
	}
	if (items.size() == 0)
		parent->navigate();
}

void Router::setLevel(int value)
{
	level = value;
}
void Router::addRoute(Router* router){
	std::cout << "Add new router: " << router->getTitle() << std::endl;
	router->parent = this;
	std::cout << "Set router parent: " << router->parent->getTitle() << std::endl;
	items.push_back(*router);
};

void Router::displayMenu(int position)
{
	std::cout << "----- " << this->getTitle() << "----- " << std::endl;
	int size = items.size();
	for (int i = 0; i < size; i++)
		std::cout << ((position == i) ? "> " : "  ") << items[i].getTitle() << std::endl;
	if (parent != NULL)
		std::cout << (is_back(position) ? "> " : "  ") << "Back." << std::endl;
	std::cout << (is_exit(position) ? "> " : "  ") << "Exit program." << std::endl;
}
int Router::getMenuSize() {
	return items.size();
}

void Router::navigate()
{
	system("CLS");
	displayMenu(step);
	int param = _getch();
	switch (param)
	{
	case 72:
		system("CLS");
		step--;
		if (step < 0)
			if (parent != NULL)
				step = getMenuSize() + 1;
			else
				step = getMenuSize();
		navigate();
		break;
	case 80:
		system("CLS");
		step++;
		if (step > getMenuSize() + (parent != NULL) ? 1 : 0)
			step = 0;
		navigate();
		break;
	case 224:
		//system("CLS");
		navigate();
		break;
	case 13:
		setActive(&step);
		break;
	default:
		navigate();
	}
}