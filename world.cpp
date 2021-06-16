#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"

// ----------------------------------------------------
World::World()
{
	tick_timer = clock();

	// Rooms ----
	Room* forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room* house = new Room("House", "You are inside a beautiful but small white house.");
	Room* basement = new Room("Basement", "The basement features old furniture and dim light.");
		// New rooms
	Room* forest2 = new Room("Deep forest", "Your are deep inside the forest");
	Room* backyard = new Room("Backyard", "You are at the backyard. There is an old well, you might get down using something.");
	Room* park = new Room("Park", "A small but cozy park. Looks like someone has been here recently.");
	Room* shed = new Room("Shed", "You are inside a small shed filled with tools.");
	Room* cave1 = new Room("Cave I", "You are sorrounded by nothing but stones. Dim light reaches from the well.");
	Room* cave2 = new Room("Cave II", "You are sorrounded by nothing but stones.");
	Room* cave3 = new Room("Cave III", "You can hear some starnge noises ahead.");
	Room* cave4 = new Room("Cave IV", "You are in an open area inside the cave.");


	Exit* ex1 = new Exit("west", "east", "Little path", house, forest);
	Exit* ex2 = new Exit("down", "up", "Stairs", house, basement);
		//New exits
	Exit* ex3 = new Exit("up", "down", "Overgown path", forest, forest2);
	Exit* ex4 = new Exit("west", "east", "Back door", house, backyard);
	Exit* ex5 = new Exit("up", "down", "Paved road", backyard, park);
	Exit* ex6 = new Exit("east", "west", "Dirt path", forest2, shed);
	Exit* ex7 = new Exit("east", "west", "Dirt path", shed, park);
	Exit* ex8 = new Exit("up", "down", "Well", cave1, backyard);
	Exit* ex9 = new Exit("east", "west", "Underground corridor", cave1, cave2);
	Exit* ex10 = new Exit("down", "up", "Undergound corridor", cave2, cave3);
	Exit* ex11 = new Exit("west", "east", "Wide corridor", cave3, cave4);
	Exit* ex12 = new Exit("up", "down", "Steep slope", cave4, cave1, true);

	ex2->locked = true;
	ex8->locked = true;
	ex3->blocked = true;
	ex3->resistance = 1;

	entities.push_back(forest);
	entities.push_back(house);
	entities.push_back(basement);
	entities.push_back(forest2);
	entities.push_back(shed);
	entities.push_back(park);
	entities.push_back(backyard);
	entities.push_back(cave1);
	entities.push_back(cave2);
	entities.push_back(cave3);
	entities.push_back(cave4);

	entities.push_back(ex1);
	entities.push_back(ex2);
	entities.push_back(ex3);
	entities.push_back(ex4);
	entities.push_back(ex5);
	entities.push_back(ex6);
	entities.push_back(ex7);
	entities.push_back(ex8);
	entities.push_back(ex9);
	entities.push_back(ex10);
	entities.push_back(ex11);
	entities.push_back(ex12);

	// Creatures ----
	Creature* butler = new Creature("Butler", "It's James, the house Butler.", house);
	butler->hit_points = 10;
		//New creatures
	Creature* troll = new Creature("Troll", "A big and fearsome troll. It's holding something.", cave4);
	entities.push_back(butler);

	// Items -----
	Item* mailbox = new Item("Mailbox", "Looks like it might contain something.", house);
	Item* key = new Item("Key", "Old iron key.", mailbox);
	ex2->key = key;

	Item* sword = new Item("Sword", "A simple old and rusty sword.", forest, WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;

	Item* sword2(sword);
	sword2->parent = butler;

	Item* shield = new Item("Shield", "An old wooden shield.", butler, ARMOUR);
	shield->min_value = 1;
	shield->max_value = 3;
	butler->AutoEquip();
		
		//New items
	Item* bag = new Item("Bag", "An old and scratched bag.", forest2);
	Item* rope = new Item("Rope", "A sturdy rope", bag);
	Item* vines = new Item("Vines", "Overgown vines that block the path.", ex3);
	ex8->key = rope;

	entities.push_back(mailbox);
	entities.push_back(sword);
	entities.push_back(shield);

	// Player ----
	player = new Player("Hero", "You are an awesome adventurer!", forest);
	player->hit_points = 25;
	entities.push_back(player);
}

// ----------------------------------------------------
World::~World()
{
	for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Tick(vector<string>& args)
{
	bool ret = true;

	if(args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	clock_t now = clock();

	if((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
bool World::ParseCommand(vector<string>& args)
{
	bool ret = true;

	switch(args.size())
	{
		case 1: // commands with no arguments ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "north") || Same(args[0], "n"))
			{
				(args.size() == 1) ? args.push_back("north") : args[1] = "north";
				player->Go(args);
			}
			else if(Same(args[0], "south") || Same(args[0], "s"))
			{
				(args.size() == 1) ? args.push_back("south") : args[1] = "south";
				player->Go(args);
			}
			else if(Same(args[0], "east") || Same(args[0], "e"))
			{
				(args.size() == 1) ? args.push_back("east") : args[1] = "east";
				player->Go(args);
			}
			else if(Same(args[0], "west") || Same(args[0], "w"))
			{
				(args.size() == 1) ? args.push_back("west") : args[1] = "west";
				player->Go(args);
			}
			else if(Same(args[0], "up") || Same(args[0], "u"))
			{
				(args.size() == 1) ? args.push_back("up") : args[1] = "up";
				player->Go(args);
			}
			else if(Same(args[0], "down") || Same(args[0], "d"))
			{
				(args.size() == 1) ? args.push_back("down") : args[1] = "down";
				player->Go(args);
			}
			else if(Same(args[0], "stats") || Same(args[0], "st"))
			{
				player->Stats();
			}
			else if(Same(args[0], "inventory") || Same(args[0], "i"))
			{
				player->Inventory();
			}
			else
				ret = false;
			break;
		}
		case 2: // commands with one argument ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "go"))
			{
				player->Go(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else if(Same(args[0], "equip") || Same(args[0], "eq"))
			{
				player->Equip(args);
			}
			else if(Same(args[0], "unequip") || Same(args[0], "uneq"))
			{
				player->UnEquip(args);
			}
			else if(Same(args[0], "examine") || Same(args[0], "ex"))
			{
				player->Examine(args);
			}
			else if(Same(args[0], "attack") || Same(args[0], "at"))
			{
				player->Attack(args);
			}
			else if(Same(args[0], "loot") || Same(args[0], "lt"))
			{
				player->Loot(args);
			}
			else if (Same(args[0], "break") || Same(args[0], "bk"))
			{
				player->Break(args);
			}
			else
				ret = false;
			break;
		}
		case 3: // commands with two arguments ------------------------------
		{
			break;
		}
		case 4: // commands with three arguments ------------------------------
		{
			if(Same(args[0], "unlock") || Same(args[0], "unlk"))
			{
				player->UnLock(args);
			}
			else if(Same(args[0], "lock") || Same(args[0], "lk"))
			{
				player->Lock(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else
				ret = false;
			break;
		}
		default:
		ret =  false;
	}

	return ret;
}