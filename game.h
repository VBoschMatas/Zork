#ifndef __Game__
#define __Game__

#include <string>
#include <list>
#include <time.h>

#define TICK_FREQUENCY 0.5f

using namespace std;

class Entity;
class Player;
class Quest;

static bool is_end = false;

class Game
{
public :

	Game();
	~Game();

	bool Tick(vector<string>& args);
	bool ParseCommand(vector<string>& args);
	void GameLoop();
	void Help();

	static void GameOver(const bool win);
	static bool End();

private:

	clock_t tick_timer;
	list<Entity*> entities;
	Player* player;
};

#endif 