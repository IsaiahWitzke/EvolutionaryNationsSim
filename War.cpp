#include "stdafx.h"
#include "War.h"
#include <sstream>
#include <windows.h>

extern MapHandler g_map;

War::War(Nation * attacker, Nation * defender)
{
	//the war name:
	ostringstream tempWarName;
	tempWarName << "The " << attacker->nationName << "-" << defender->nationName << " war of agression";
	this->warName = tempWarName.str();

	this->leadAttacker = attacker;
	this->leadDefender = defender;

	addAttacker(attacker);
	addDefender(defender);

	//call in the defender's allies into the war
	//go through all nations in the world, if they are an ally, call them into the war
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		if (g_map.nations[i] == defender)
			continue;
		if (defender->diplomaticRelations[g_map.nations[i]] == DiplomaticRelation::ally)
		{
			addDefender(g_map.nations[i]);
		}
	}

	//then attackers
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		if (g_map.nations[i] == attacker)
			continue;
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

	return;
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

void War::endWar()
{

	//getting rid of defender's wars first
	//iterate through all defenders, then remove them from the war
	for (int i = 0; i < this->defenders.size(); i++)
	{
		this->removeBelligerent(this->defenders[i]);
	}
	//then attackers
	for (int i = 0; i < this->attackers.size(); i++)
	{
		this->removeBelligerent(this->attackers[i]);
	}

}

void War::removeBelligerent(Nation * formerBelligerent)
{
	//get rid of war from wars vector
	for (int i = 0; i < formerBelligerent->wars.size(); i++)
	{
		if (formerBelligerent->wars[i] == this)
		{
			formerBelligerent->wars.erase(formerBelligerent->wars.begin() + i);
			break;
		}
	}
}

float War::chanceOfAttackerWinningBattle()
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
	//changing color of console output
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl;
	cout << warName << " battle: " << endl;

	float outcome = float(rand() % 100) / 100;

	//do the battle: (if random # between (0 to 100)/100 > chanceOfWinningBattle, then the attackers win)
	if (outcome <= chanceOfAttackerWinningBattle())
	{
		SetConsoleTextAttribute(hConsole, 6);	// puke yellow
		cout << "attackers win (" << leadAttacker->nationName << ")" << endl;
		//warscore increases
		this->warScore += 2;

		//losses on both side (more on the defending side)
		for (int i = 0; i < defenders.size(); i++)
		{
			defenders[i]->armyStrength *= 0.3;
			defenders[i]->warExhaustion += 4;
		}
		for (int i = 0; i < attackers.size(); i++)
		{
			attackers[i]->armyStrength *= 0.8;
			attackers[i]->warExhaustion += 1;
		}
	}

	//defenders win
	if (outcome > chanceOfAttackerWinningBattle())
	{
		SetConsoleTextAttribute(hConsole, 3);	// blue
		cout << "defenders win (" << leadDefender->nationName << ")" << endl;
		//warscore decreases
		this->warScore -= 2;

		//losses on both side (more on the attacking side)
		for (int i = 0; i < defenders.size(); i++)
		{
			defenders[i]->armyStrength *= 0.8;
			defenders[i]->warExhaustion += 1;
		}
		for (int i = 0; i < attackers.size(); i++)
		{
			attackers[i]->armyStrength *= 0.3;
			attackers[i]->warExhaustion += 4;
		}
	}

	SetConsoleTextAttribute(hConsole, 7);
}

void War::printInfo(Nation * nation)
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

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	//see if the nation of interest is part of the winning side
	bool isDefender = false;
	for (int i = 0; i < defenders.size(); i++)
	{
		if (this->defenders[i] == nation)
		{
			isDefender = true;
		}
	}

	//want to output whether or not the nation of interest is winning:
	if (isDefender)
	{
		if (warScore >= 0)
		{
			SetConsoleTextAttribute(hConsole, 4);	// red
			cout << "  " << "loosing by " << warScore << " points" << endl;
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 2);	// green
			cout << "  " << "winning by " << -1 * warScore << " points" << endl;
		}
	}
	else
	{
		if (warScore >= 0)
		{
			SetConsoleTextAttribute(hConsole, 2);	// green
			cout << "  " << "winning by " << warScore << " points" << endl;
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 4);	// red
			cout << "  " << "loosing by " << -1 * warScore << " points" << endl;
		}
	}
	
	SetConsoleTextAttribute(hConsole, 7);	// white
}
