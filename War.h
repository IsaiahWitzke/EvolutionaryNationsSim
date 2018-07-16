#pragma once

#include "Nation.h"
#include "MapHandler.h"
#include <vector>

using namespace std;

class Nation;

class War
{
public:
	string warName;

	Nation *leadAttacker;
	Nation *leadDefender;

	vector<Nation*> defenders;
	vector<Nation*> attackers;

	int warScore = 0;	// positive if attackers are winning, negative if defenders are winning

	//to start a war init a War object with an attacker and defender
	War(Nation * attacker, Nation * defender);
	~War();

	void addDefender(Nation * newDefender);
	void addAttacker(Nation * newAttacker);

	//returns the chance of the attacker winning a battle (1 being 100% in favour of attackers, being 100% in favour of defenders):
	float chanceOfWinningBattle();
	void battle();

	void printInfo();
};

