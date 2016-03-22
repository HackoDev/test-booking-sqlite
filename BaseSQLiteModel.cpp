#include "BaseSQLiteModel.h"
#include <vector>
#include <typeinfo>
#include "sqlite3.h"
#include "User.h"


BaseSQLiteModel::BaseSQLiteModel()
{

};
BaseSQLiteModel::~BaseSQLiteModel()
{

};

int BaseSQLiteModel::get_id()
{
	return id;
}