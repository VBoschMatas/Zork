#ifndef __Connection__
#define __Connection__

#include <string>
#include "entity.h"

class Room;

using namespace std;

class Connection : public Entity
{
public:
	Connection(const char* name, const char* opposite_name, const char* description, Room* origin, Room* destination, bool one_way = false);
	~Connection();

	void Look() const;

	const string& GetNameFrom(const Room* room) const;
	Room* GetDestinationFrom(const Room* room) const;

public :
	bool one_way;
	bool closed;
	bool blocked; //If the exit is blocked by an obstacle
	bool locked;
	int resistance; //Resistance of the obstacle
	string opposite_name;
	Room* destination;
	Entity* key;
};

#endif //__Connection__