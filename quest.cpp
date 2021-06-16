#include <iostream>
#include "room.h"
#include "exit.h"
#include "creature.h"
#include "globals.h"
#include "quest.h"

// ----------------------------------------------------
Quest::Quest(const char* name, const char* description, Item* parent, const bool main) :
	Entity(name, description, parent)
{
	conversation = "";
	completion = "";
	reward = NULL;
	objective = NULL;
}

// ----------------------------------------------------
Quest::~Quest()
{}