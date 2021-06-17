#include <iostream>
#include "globals.h"
#include "room.h"
#include "connection.h"
#include "item.h"
#include "creature.h"
#include "quest.h"
#include "game.h"

Creature::Creature(const char* title, const char* description, Room* room) :
Entity(title, description, (Entity*)room)
{
	type = CREATURE;
	hit_points = 1;
	max_hp = 1;
	min_damage = max_damage = min_protection = max_protection = 0;
	weapon = armour = NULL;
	combat_target = NULL;
	quest = NULL;
}

Creature::~Creature()
{}

/*
	Check the information about a creature
*/
void Creature::Look(const vector<string>& args) const
{
	if(IsAlive())
	{
		cout << name << "\n";
		cout << description << "\n";
	}
	else
	{
		cout << name << "'s corpse\n";
		cout << "Here lies dead: " << description << "\n";
	}
}


/*
	Check the inventory of a creature
*/
void Creature::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if(items.size() == 0)
	{
		cout << name << " does not hold any items\n";
		return;
	}

	cout << "\n" << name << " has:\n";
	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			cout << (*it)->name << " (as weapon)\n";
		else if(*it == armour)
			cout << (*it)->name << " (as armour)\n";
		else
			cout << (*it)->name << "\n";
	}
}


/*
	Equips automatically all equipment form inventory
*/
bool Creature::AutoEquip()
{
	if(!IsAlive())
		return false;

	list<Entity*> items;
	FindAll(ITEM, items);

	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		Item* i = (Item*)(*it);

		if(i->item_type == WEAPON)
			weapon = i;
		if(i->item_type == ARMOUR)
			armour = i;
	}

	return true;
}


/*
	Take the information about the room the creature is
*/
Room* Creature::GetRoom() const
{
	return (Room*)parent;
}

/*
	Check if the player is in the same room
*/
bool Creature::PlayerInRoom() const
{
	return parent->Find(PLAYER) != NULL;
}

/*
	Check if the creature is still alive
*/
bool Creature::IsAlive() const
{
	return hit_points > 0;
}

/*
	Action performed every game tick
*/
void Creature::Tick()
{
	if(combat_target != NULL)
	{
		if(parent->Find(combat_target) == true)
			MakeAttack();
		else
			combat_target = NULL;
	}
}

/*
	Attack at target
*/
bool Creature::Attack(const vector<string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
		return false;

	combat_target = target;
	cout << "\n" << name << " attacks " << target->name << "!\n";
	return true;
}

int Creature::MakeAttack()
{
	if(!IsAlive() || !combat_target->IsAlive())
	{
		combat_target = combat_target->combat_target = NULL;
		return false;
	}

	int result = (weapon) ? weapon->GetValue() : Roll(min_damage, max_damage);

	if(PlayerInRoom())
		cout << name << " attacks " << combat_target->name << " for " << result << "\n";

	combat_target->ReceiveAttack(result);

	// make the attacker react and take me as a target
	if(combat_target->combat_target == NULL)
		combat_target->combat_target = this;

	return result;
}

int Creature::ReceiveAttack(int damage)
{
	int prot = (armour) ? armour->GetValue() : Roll(min_protection, max_protection);
	int received = damage - prot;
	if (received < 0) received = 0;
	hit_points -= received;

	if(PlayerInRoom())
		cout << name << " is hit for " << received << " damage (" << prot << " blocked) \n";

	if(IsAlive() == false)
		Die();

	return received;
}

/*
	If creature has 0 hp, die. If it is player: Game Over
*/
void Creature::Die()
{
	if(PlayerInRoom())
		cout << name << " dies.\n";
	if (type == PLAYER)
		Game::GameOver(false);
}

/*
	Check the stats about a creature
*/
void Creature::Stats() const
{
	cout << "\nHit Points: " << hit_points;
	cout << "\nAttack: (" << ((weapon) ? weapon->name : "no weapon") << ") ";
	cout << ((weapon) ? weapon->min_value : min_damage) << "-" << ((weapon) ? weapon->max_value : max_damage);
	cout << "\nProtection: (" << ((armour) ? armour->name : "no armour") << ") ";
	cout << ((armour) ? armour->min_value : min_protection) << "-" << ((armour) ? armour->max_value : max_protection);
	cout << "\n";
}

/*
	Respond to the player depending on your quest status
*/
void Creature::QuestStatus() const
{
	if (quest->is_completed == true) {
		cout << "\n" << name << ": \"" << quest->completion << "\"\n";
		return;
	}
	else {
		cout << "\n" << name << ": \"" << quest->conversation << "\"\n";
		return;
	}
}

/*
	Check if the quest is completed, if it is and the item given is correct, return the reward
*/
Item* Creature::CompleteQuest(Item* item)
{
	if (quest->is_completed == true) {
		cout << "\nAlready completed.\n";
		return NULL;
	}
	if (quest->objective == item->name) {
		item->ChangeParentTo(this);
		quest->is_completed = true;
		cout << "\nQUEST COMPLETE!\n";
		cout << name << ": \"" << quest->completion << "\"\n";
		if (quest->is_main == true) {
			Game::GameOver(true);
		}
		return quest->reward;
	}

	cout << "\nThat's not the correct item\n";
	return NULL;
}