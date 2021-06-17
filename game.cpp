#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "connection.h"
#include "room.h"
#include "player.h"
#include "game.h"
#include "quest.h"

/**
	We create all the entities that will be in the game and push them into the entities list
**/
Game::Game()
{
	tick_timer = clock();

	// Rooms
	Room* house = new Room("House", "You are inside a little house with a basement");
	Room* basement = new Room("Basement", "The basement features old furniture and dim light.");
	Room* forest1 = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room* forest2 = new Room("Deep forest", "Your are deep inside the forest");
	Room* backyard = new Room("Backyard", "You are at the backyard. There is an old well, you might get down using something.");
	Room* park = new Room("Park", "A runned down park filled with unfriendly inhabitants.");
	Room* shed = new Room("Shed", "You are inside a small shed filled with tools.");
	Room* cave1 = new Room("Cave I", "You are sorrounded by nothing but stones. Dim light reaches from the well.");
	Room* cave2 = new Room("Cave II", "You are sorrounded by nothing but stones.");
	Room* cave3 = new Room("Cave III", "You can hear some starnge noises ahead.");
	Room* cave4 = new Room("Cave IV", "You are in an open area inside the cave.");

	// Connections
	Connection* cn1 = new Connection("west", "east", "Little path", house, forest1);
	Connection* cn2 = new Connection("down", "up", "Stairs", house, basement);
	Connection* cn3 = new Connection("north", "south", "Overgrown path", forest1, forest2);
	Connection* cn4 = new Connection("east", "west", "Back door", house, backyard);
	Connection* cn5 = new Connection("north", "south", "Paved road", backyard, park);
	Connection* cn6 = new Connection("east", "west", "Dirt path", forest2, shed);
	Connection* cn7 = new Connection("east", "west", "Dirt path", shed, park);
	Connection* cn8 = new Connection("up", "down", "Well", cave1, backyard);
	Connection* cn9 = new Connection("east", "west", "Underground corridor", cave1, cave2);
	Connection* cn10 = new Connection("south", "north", "Undergound corridor", cave2, cave3);
	Connection* cn11 = new Connection("east", "west", "Wide corridor", cave3, cave4);
	Connection* cn12 = new Connection("down", "up", "Steep slope", cave4, cave1, true);

	cn2->locked = true;
	cn8->locked = true;
	cn3->blocked = true;
	cn3->resistance = 1;
	cn6->blocked = true;
	cn6->resistance = 5;

	entities.push_back(forest1);
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

	entities.push_back(cn1);
	entities.push_back(cn2);
	entities.push_back(cn3);
	entities.push_back(cn4);
	entities.push_back(cn5);
	entities.push_back(cn6);
	entities.push_back(cn7);
	entities.push_back(cn8);
	entities.push_back(cn9);
	entities.push_back(cn10);
	entities.push_back(cn11);
	entities.push_back(cn12);

	// Creatures
	Creature* butler = new Creature("Butler", "It's James, the house Butler.", house);
	butler->hit_points = 10;

	Creature* dispatcher = new Creature("Dispatcher", "The one that commissioned you.", forest1);
	dispatcher->hit_points = 100;
	dispatcher->min_damage = 99;
	dispatcher->max_damage = 100;
	dispatcher->min_protection = 99;
	dispatcher->max_protection = 100;

	Creature* rat1 = new Creature("Rat", "A small rat", backyard);
	rat1->hit_points = 5;
	Creature* rat2 = new Creature("Rat", "A small rat", forest2);
	rat2->hit_points = 5;
	Creature* rat3 = new Creature("Rat", "A small rat", cave2);
	rat3->hit_points = 5;

	Creature* troll = new Creature("Troll", "A big and fearsome troll.", cave4);
	troll->hit_points = 50;
	troll->min_protection = 0;
	troll->max_protection = 3;
	troll->min_damage = 5;
	troll->max_damage = 8;

	Creature* goblin = new Creature("Goblin", "A small goblin that left the pack.", park);
	goblin->hit_points = 10;
	goblin->min_damage = 1;
	goblin->max_damage = 3;

	entities.push_back(rat1);
	entities.push_back(rat2);
	entities.push_back(rat3);
	entities.push_back(dispatcher);
	entities.push_back(butler);
	entities.push_back(troll);
	entities.push_back(goblin);

	// Items
	Item* mailbox = new Item("Mailbox", "Looks like it might contain something.", house);
	Item* key = new Item("Key", "Old iron key.", mailbox);
	cn2->key = key; // we set 'key' as the key for connection 2: stairs

	Item* sword = new Item("Sword", "A small rusty sword.", basement, WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;

	Item* axe = new Item("Axe", "A big wood-cutting axe.", shed, WEAPON);
	axe->min_value = 6;
	axe->max_value = 8;

	Item* rapier = new Item("Rapier", "A well conserved rapier. Very powerful on the right hands.", shed, WEAPON);
	rapier->min_value = 3;
	rapier->max_value = 13;

	Item* buckler = new Item("Buckler", "A small wooden buckler.", butler, ARMOUR);
	buckler->min_value = 1;
	buckler->max_value = 3;

	Item* shield = new Item("Shield", "A resistant shield.", goblin, ARMOUR);
	shield->min_value = 3;
	shield->max_value = 5;
	goblin->AutoEquip();
		
	Item* bag = new Item("Bag", "An old and scratched bag.", forest2);
	Item* rope = new Item("Rope", "A sturdy long rope", bag);
	cn8->key = rope;
	Item* vines = new Item("vines", "Overgrown vines are blocking the path.", cn3);
	Item* logs = new Item("logs", "Some trees fell down and are blocking the path.", cn6);

	Item* head = new Item("Head", "Troll's head. Bring it back to the dispatcher.", troll);
	Item* meat1 = new Item("Meat", "Some raw meat. Restores some hp.", rat1, CONSUMABLE);
	meat1->min_value = 4; meat1->max_value = 6;
	Item* meat2 = new Item("Meat", "Some raw meat. Restores some hp.", rat2, CONSUMABLE);
	meat2->min_value = 4; meat2->max_value = 6;
	Item* meat3 = new Item("Meat", "Some raw meat. Restores some hp.", rat3, CONSUMABLE);
	meat3->min_value = 4; meat3->max_value = 6;

	entities.push_back(mailbox);
	entities.push_back(sword);
	entities.push_back(axe);
	entities.push_back(rapier);
	entities.push_back(buckler);
	entities.push_back(shield);
	entities.push_back(bag);
	entities.push_back(head);
	entities.push_back(meat1);
	entities.push_back(meat2);
	entities.push_back(meat3);

	// Quests
	Quest* main = new Quest("Main quest", "Hunt down the troll and bring his head back.", dispatcher, true);
	main->conversation = "Hello adventurer.\nWhat are you waiting for? Hunt down that troll and bring his head back to me.";
	main->completion = "Good job, as trust worthy as always.\nTime to head back to the HQ.";
	main->objective = "Head";
	dispatcher->quest = main;

	Quest* secondary = new Quest("Something to cook", "Bring to the butler some meat.", butler, false);
	secondary->conversation = "Good day sir.\nI would love to get some meat to cook, but I'm too bussy now. May you bring me some of it.\nYou will be rewarded.";
	secondary->completion = "I hope this buckler comes in handy for your adventures!";
	secondary->objective = "Meat";
	secondary->reward = buckler;
	butler->quest = secondary;
	
	// Player
	player = new Player("Adventurer", "You are an awesome adventurer!", forest1);
	player->hit_points = 25; player->max_hp = 25; //Max hp only is used when healing, so its only required by the player
	entities.push_back(player);
}

// Clears out the entities list when the game starts
Game::~Game()
{
	for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

//Every game tick
bool Game::Tick(vector<string>& args)
{
	bool ret = true;

	if(args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}


void Game::GameLoop()
{
	clock_t now = clock();

	if((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY) //Enters one time per second
	{
		for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

/*
	Commands introduced by the player will be processed here
*/
bool Game::ParseCommand(vector<string>& args)
{
	bool ret = true;

	switch(args.size())
	{
		case 1: // commands with no arguments
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
			else if (Same(args[0], "help") || Same(args[0], "h"))
			{
				Help();
			}
			else
				ret = false;
			break;
		}
		case 2: // commands with one argument
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
			else if (Same(args[0], "talk") || Same(args[0], "tl") || Same(args[0], "quest") || Same(args[0], "qst"))
			{
				player->Talk(args);
			}
			else if (Same(args[0], "use") || Same(args[0], "consume"))
			{
				player->Use(args);
			}
			else
				ret = false;
			break;
		}
		case 3: // commands with two arguments
		{
			if (Same(args[0], "talk") || Same(args[0], "tl") || Same(args[0], "quest") || Same(args[0], "qst"))
			{
				player->Talk(args);
			}
			else
				ret = false;
			break;
		}
		case 4: // commands with three arguments
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
			else if (Same(args[0], "give") || Same(args[0], "bring"))
			{
				player->Give(args);
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

/*
	Show the commands
*/
void Game::Help() {
	cout << "\n-------------------------------";
	cout << "\nHelp";
	cout << "\n-------------------------------";
	cout << "\nYour objective is to complete  ";
	cout << "\nthe main quest delivered by    ";
	cout << "\nthe dispatcher.                ";
	cout << "\n                               ";
	cout << "\n-------- Commands: ------------";
	cout << "\n>look                          ";
	cout << "\n>stats                         ";
	cout << "\n>inventory                     ";
	cout << "\n>look [entity]                 ";
	cout << "\n>[direction]                   ";
	cout << "\n>go [direction]                ";
	cout << "\n>take [item]                   ";
	cout << "\n>take [item] from [container]  ";
	cout << "\n>drop [item]                   ";
	cout << "\n>drop [item] at [container]    ";
	cout << "\n>equip [item]                  ";
	cout << "\n>unequip [item]                ";
	cout << "\n>talk (to) [creature]          ";
	cout << "\n>attack [creature]             ";
	cout << "\n>loot [creature]               ";
	cout << "\n>examine [creature]            ";
	cout << "\n>break [direction]             ";
	cout << "\n>unlock [direction] with [item]";
	cout << "\n>lock [direction] with [item]  ";
	cout << "\n>use [item]                \n\n";
}

/*
	static function that will be executed if the game is over
*/
void Game::GameOver(const bool win)
{
	if (!win) {
		cout << "\n ----------------------------------- \n";
		cout << "\n           GAME       OVER           \n";
		cout << "\n ----------------------------------- \n";
		cout << "          better luck next time        \n";
	}
	else {
		cout << "\n ----------------------------------- \n";
		cout << "\n           CONGRATULATIONS           \n";
		cout << "\n ----------------------------------- \n";
	}
	is_end = true;
}

/*
	Static function that tell to main if the game is over
*/
bool Game::End() {
	if (is_end) return true;
	return false;
}