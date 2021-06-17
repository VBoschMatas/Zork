#include <iostream>
#include "globals.h"
#include "connection.h"
#include "item.h"
#include "creature.h"
#include "room.h"

Room::Room(const char* title, const char* description) :
Entity(title, description, NULL)
{
	type = ROOM;
}

Room::~Room()
{
}

/*
	Look whats inside the room
*/
void Room::Look() const
{
	cout << "\n" << name << "\n";
	cout << description;

	// List connections
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == EXIT)
		{
			Connection* ex = (Connection*)*it;
			cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see " << ex->GetDestinationFrom(this)->name;
		}
	}

	// List items
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == ITEM)
		{
			Item* item = (Item*)*it;
			cout << "\nThere is an item here: " << item->name;
		}
	}

	// List creatures
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == CREATURE)
		{
			Creature* cr = (Creature*)*it;
			cout << "\nThere is someone else here: " << cr->name;
			if(cr->IsAlive() == false)
				cout << " (dead)";
		}
	}

	cout << "\n";
}

/*
	Return the connections it has
*/
Connection* Room::GetConnection(const string& direction) const
{
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == EXIT)
		{
			Connection* ex = (Connection*) *it;
			if(Same(ex->GetNameFrom(this), direction))
				return ex;
		}
	}

	return NULL;
}


