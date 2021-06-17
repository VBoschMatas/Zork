#ifndef __Creature__
#define __Creature__

#include <string>
#include "entity.h"

class Room;
class Item;
class Quest;

using namespace std;

class Creature : public Entity
{
public:
	Creature(const char* name, const char* description, Room* room);
	~Creature();

	virtual void Look(const vector<string>& args) const;
	virtual void Inventory() const;
	virtual bool AutoEquip();
	virtual void Tick();

	virtual bool Attack(const vector<string>& args);
	virtual int MakeAttack();
	virtual int ReceiveAttack(int damage);
	virtual void Die();
	virtual void Stats() const;
	virtual void QuestStatus() const;
	virtual Item* CompleteQuest(Item* item);

	Room* GetRoom() const;
	bool PlayerInRoom() const;
	bool IsAlive() const;

public :
	int max_hp;
	int hit_points;
	int min_damage;
	int max_damage;
	int min_protection;
	int max_protection;
	Creature* combat_target;
	Item* weapon;
	Item* armour;
	Quest* quest;
};

#endif //__Creature__