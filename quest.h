#ifndef __Quest__
#define __Quest__

#include <string>
#include "entity.h"

class Item;

class Quest : public Entity
{
public:
	Quest(const char* name, const char* description, Entity* parent, const bool main = false);
	~Quest();


public:
	const char* objective; // It may be different items with same name
	Item* reward;
	const char* conversation;
	const char* completion;
	bool is_completed;
	bool is_main;
};
#endif