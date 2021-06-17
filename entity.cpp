#include <iostream>
#include "globals.h"
#include "entity.h"

Entity::Entity(const char* name, const char* description, Entity* parent = NULL) :
name(name), description(description), parent(parent)
{
	type = ENTITY;

	if(parent != NULL)
		parent->container.push_back(this);
}

Entity::~Entity()
{}

// We get the basic information of the entity
void Entity::Look() const
{
	cout << name << "\n";
	cout << description << "\n";
}

// Action performed at every tick
void Entity::Tick()
{
}

// Everytime an entity changes its location (inventory, room, etc) its new parent becomes the actual parent
void Entity::ChangeParentTo(Entity* new_parent)
{
	if(parent != NULL)
		parent->container.remove(this);

	parent = new_parent;

	if(parent != NULL)
		parent->container.push_back(this);
}

// Looks for an entity in particular
bool Entity::Find(Entity* entity) const
{
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it) == entity)
			return true;
	}

	return false;
}

// Looks for a type of entity (ITEM, ROOM, CONNECTOR...)
Entity* Entity::Find(EntityType type) const
{
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == type)
			return *it;
	}

	return NULL;
}

// Looks for an entity thats from a certain type
Entity* Entity::Find(const string& name, EntityType type) const
{
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == type)
		{
			if(Same((*it)->name, name))
				return *it;
		}
	}

	return NULL;
}

// Gets all entities
void Entity::FindAll(EntityType type, list<Entity*>& list_to_fill) const
{
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == type)
			list_to_fill.push_back(*it);
	}
}