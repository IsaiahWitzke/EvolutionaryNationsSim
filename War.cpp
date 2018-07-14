#include "stdafx.h"
#include "War.h"
#include <sstream>

extern MapHandler g_map;

War::War(Nation * attacker, Nation * defender)
{
	//the war name:
	ostringstream tempWarName;
	tempWarName << "The " << attacker->nationName << "-" << defender->nationName << " war of agression";
	this->warName = tempWarName.str();

	addAttacker(attacker);
	addDefender(defender);

	//call in the defender's allies into the war
	//go through all nations in the world, if they are an ally, call them into the war
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		if (defender->diplomaticRelations[g_map.nations[i]] == DiplomaticRelation::ally)
		{
			addDefender(g_map.nations[i]);
		}
	}

	//then attackers
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		if (attacker->diplomaticRelations[g_map.nations[i]] == DiplomaticRelation::ally)
		{
			addAttacker(g_map.nations[i]);
		}
	}

}

War::~War()
{
}


void War::addDefender(Nation * newDefender)
{
	this->defenders.push_back(newDefender);
	newDefender->wars.push_back(this);
	//go through all attackers and put each other at war
	for (int i = 0; i < attackers.size(); i++)
	{
		newDefender->diplomaticRelations[attackers[i]] = DiplomaticRelation::belligerent;
		newDefender->decreaseRelations(attackers[i]);
		attackers[i]->diplomaticRelations[newDefender] = DiplomaticRelation::belligerent;
		attackers[i]->decreaseRelations(newDefender);
	}	
}

void War::addAttacker(Nation * newAttacker)
{
	newAttacker->wars.push_back(this);
	this->attackers.push_back(newAttacker);
	for (int i = 0; i < defenders.size(); i++)
	{
		newAttacker->diplomaticRelations[defenders[i]] = DiplomaticRelation::belligerent;
		newAttacker->decreaseRelations(defenders[i]);
		defenders[i]->diplomaticRelations[newAttacker] = DiplomaticRelation::belligerent;
		defenders[i]->decreaseRelations(newAttacker);
	}
}

float War::chanceOfWinningBattle()
{
	float attackerStrength = 0;
	float defenderStrength = 0;

	//go through all attackers, take their "army" variable, multiply it by the "armyStrength" variable and add it on to the "attackerStrength"
	for (int i = 0; i < attackers.size(); i++)
	{
		attackerStrength += attackers[i]->army * attackers[i]->armyStrength;
	}

	//same with defenders
	for (int i = 0; i < defenders.size(); i++)
	{
		defenderStrength += defenders[i]->army * defenders[i]->armyStrength;
	}

	return attackerStrength / (attackerStrength + defenderStrength);
}

void War::battle()
{

}

void War::printInfo()
{
	cout << warName << endl;
	cout << "  Attackers:" << endl;
	for (int i = 0; i < attackers.size(); i++)
	{
		cout << "   " << attackers[i]->nationName << endl;
	}
	cout << "  Defenders:" << endl;
	for (int i = 0; i < defenders.size(); i++)
	{
		cout << "   " << defenders[i]->nationName << endl;
	}
}
