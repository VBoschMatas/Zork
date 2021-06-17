#include <iostream>
#include "globals.h"
#include "room.h"
#include "connection.h"
#include "item.h"
#include "player.h"

// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
Creature(title, description, room)
{
	type = PLAYER;
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const vector<string>& args) const
{
	if(args.size() > 1)
	{
		for(list<Entity*>::const_iterator it = parent->container.begin(); it != parent->container.cend(); ++it)
		{
			if(Same((*it)->name, args[1]) || ((*it)->type == EXIT && Same(args[1], ((Connection*)(*it))->GetNameFrom((Room*)parent))))
			{
				(*it)->Look();
				return;
			}
		}

		if(Same(args[1], "me"))
		{
			cout << "\n" << name << "\n";
			cout << description << "\n";
		}
	}
	else
	{
		parent->Look();
	}
}

// ----------------------------------------------------
bool Player::Go(const vector<string>& args)
{
	Connection* exit = GetRoom()->GetConnection(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked)
	{
		cout << "\nThat exit is locked.\n";
		return false;
	}

	if (exit->blocked)
	{
		cout << "\nThat exit is blocked by " << exit->Find(ITEM)->name << ". You might need something to break through.\n";
		return false;
	}

	cout << "\nYou take direction " << exit->GetNameFrom((Room*) parent) << "...\n";
	ChangeParentTo(exit->GetDestinationFrom((Room*) parent));
	parent->Look();

	return true;
}


// ----------------------------------------------------
bool Player::Take(const vector<string>& args)
{
	if(args.size() == 4)
	{
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if(item == NULL)
			item = (Item*)Find(args[3], ITEM);

		if(item == NULL)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if(subitem == NULL)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
	}
	else if(args.size() == 2)
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nThere is no item here with that name.\n";
			return false;
		}

		cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	return false;
}

// ----------------------------------------------------
void Player::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if(items.size() == 0)
	{
		cout << "\nYou do not own any item.\n";
		return;
	}

	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			cout << "\n" << (*it)->name << " (as weapon)";
		else if(*it == armour)
			cout << "\n" << (*it)->name << " (as armour)";
		else
			cout << "\n" << (*it)->name;
	}

	cout << "\n";
}

// ----------------------------------------------------
bool Player::Drop(const vector<string>& args)
{
	if(args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);

		return true;
	}
	else if(args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if(container == NULL)
		{
			container = (Item*)Find(args[3], ITEM);
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		cout << "\nYou put " << item->name << " into " << container->name << ".\n";
		item->ChangeParentTo(container);

		return true;
	}
	
	return false;
}

// ----------------------------------------------------
bool Player::Equip(const vector<string>& args)
{
	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		cout << "\nCannot find '" << args[1] << "' is not in your inventory.\n";
		return false;
	}

	switch(item->item_type)
	{
		case WEAPON:
		weapon = item;
		break;

		case ARMOUR:
		armour = item;
		break;

		default:
		cout << "\n" << item->name << " cannot be equipped.\n";
		return false;
	}
		
	cout << "\nYou equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::UnEquip(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		cout << "\n" << item->name << " is not in your inventory.\n";
		return false;
	}

	if(item == weapon)
		weapon = NULL;
	else if(item == armour)
		armour = NULL;
	else
	{
		cout << "\n" << item->name << " is not equipped.\n";
		return false;
	}

	cout << "\nYou un-equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::Examine(const vector<string>& args) const
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	target->Inventory();
	target->Stats();

	return true;
}

// ----------------------------------------------------
bool Player::Attack(const vector<string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	combat_target = target;
	cout << "\nYou jump to attack " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
bool Player::Loot(const vector<string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if(target->IsAlive() == true)
	{
		cout << "\n" << target->name << " cannot be looted until it is killed.\n";
		return false;
	}

	list<Entity*> items;
	target->FindAll(ITEM, items);

	if(items.size() > 0)
	{
		cout << "\nYou loot " << target->name << "'s corpse:\n";

		for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
		{
			Item* i = (Item*)(*it);
			cout << "You find: " << i->name << "\n";
			i->ChangeParentTo(this);
		}
	}
	else
		cout << "\nYou loot " << target->name << "'s corpse, but find nothing there.\n";

	return true;
}

// ----------------------------------------------------
bool Player::Lock(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Connection* exit = GetRoom()->GetConnection(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked == true)
	{
		cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->key != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou lock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

// ----------------------------------------------------
bool Player::UnLock(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Connection* exit = GetRoom()->GetConnection(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked == false)
	{
		cout << "\nThat exit is not locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->key != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}

// The player will try to break what blocks the path
bool Player::Break(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Connection* exit = GetRoom()->GetConnection(args[1]);

	if (exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->blocked == false)
	{
		cout << "\nThat exit is not blocked.\n";
		return false;
	}

	//If the resistance of the blocking item is higher than the playe's attack it will fail
	if (exit->resistance > (((weapon) ? weapon->min_value : min_damage)) + min_damage)
	{
		cout << "\n You do not have enough strength to break through.\n";
		return false;
	}

	cout << "\nYou cleared " << exit->GetNameFrom((Room*)parent) << " from " << exit->Find(ITEM)->name << "...\n";

	exit->blocked = false;

	return true;
}

bool Player::Talk(const vector<string>& args)
{
	string name;
	Creature *target;

	if (args.size() == 2) {
		target = (Creature*)parent->Find(args[1], CREATURE);
		name = args[1];
	}
	else if (args.size() == 3) {
		target = (Creature*)parent->Find(args[2], CREATURE);
		name = args[2];
	}

	if (target == NULL)
	{
		cout << "\n" << name << " is not here.\n";
		return false;
	}

	if (target->quest == NULL)
	{
		cout << "\nYou can't talk to " << target->name;
		return false;
	}

	//combat_target = target;
	cout << "\n" << "!\n";
	return true;
}