#include <iostream>
#include "room.h"
#include "connection.h"
#include "globals.h"
#include "item.h"

Item::Item(const char* title, const char* description, Entity* parent, ItemType item_type) :
Entity(title, description, parent), item_type(item_type)
{
	type = ITEM;
	min_value = max_value = 0;
}

Item::~Item()
{}

/*
	Look the information about the item. If it has something inside show it
*/
void Item::Look() const
{
	cout << "\n" << name << "\n";
	cout << description << "\n";

	list<Entity*> stuff;
	FindAll(ITEM, stuff);

	if(stuff.size() > 0)
	{
		cout << "It contains: " << "\n";
		for(list<Entity*>::const_iterator it = stuff.begin(); it != stuff.cend(); ++it)
			cout << (*it)->name << "\n";
	}
}

/*
	Return a random value between max and min
*/
int Item::GetValue() const
{
	return Roll(min_value, max_value);
}