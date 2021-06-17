#include <iostream>
#include "globals.h"
#include "creature.h"
#include "room.h"
#include "quest.h"


Quest::Quest(const char* title, const char* description, Entity* parent, const bool main) :
Entity(title, description, parent)
{
	conversation = "";
	completion = "";
	reward = NULL;
	objective = "";
	is_completed = false;
	is_main = main;
}

Quest::~Quest()
{}