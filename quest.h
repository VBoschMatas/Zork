#ifndef __Quest__
#define __Quest__

#include <string>
#include "entity.h"

class Item;

class Quest : public Entity
{
public:
	Quest(const char* name, const char* description, Item* parent, const bool main = false);
	~Quest();

	void Look() const;
	int GetValue() const;

public:
	Item* objective;
	Item* reward;
	const char* conversation;
	const char* completion;
};