#include <iostream>
#include "room.h"
#include "connection.h"

Connection::Connection(const char* name, const char* opposite_name, const char* description, Room* origin, Room* destination, bool one_way) :
Entity(name, description, (Entity*)origin),
closed(false), locked(false), key(NULL), blocked(false), resistance(0), one_way(one_way), destination(destination), opposite_name(opposite_name)
{
	type = EXIT;

	if(one_way == false)
		destination->container.push_back(this);
}

Connection::~Connection()
{
}


// We check what the connection connects
void Connection::Look() const
{
	cout << "\n" << name << " to " << opposite_name << "\n";
	cout << description << "\n";
	if (blocked == true) {
		cout << Find(ITEM)->description << "\n";
	}
	if (locked == true) {
		cout << "You might need something to continue.\n";
	}
}

// Get the name of the path when looking at it from a room
const string& Connection::GetNameFrom(const Room* room) const
{
	if(room == parent)
		return name;
	if(room == destination)
		return opposite_name;

	return name;
}

/*
	Get the destination when looking at it from a room
*/
Room* Connection::GetDestinationFrom(const Room* room) const
{
	if(room == parent)
		return destination;
	if(room == destination)
		return (Room*) parent;

	return NULL;
}