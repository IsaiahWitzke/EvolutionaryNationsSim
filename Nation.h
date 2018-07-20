#pragma once

#include <sfml/Graphics.hpp>
#include "State.h"
#include "MapHandler.h"
#include "War.h"
#include <vector>
#include <iostream>
#include <string>
#include <windows.h>

using namespace sf;
using namespace std;

class State;
class MapHandler;
class War;

//different types of diplomatic VIEWPOINTS (that is, how a nation feels about another)
enum DiplomaticView
{
	hostile,
	uneasy,
	neutral,
	warm,
	friendly,
	close
};

//different types of diplomatic relations
enum DiplomaticRelation
{
	nothing,
	ally,
	belligerent,
	vassal,
	overlord,
};


class Nation
{
private:
	bool warWithStateOffset(int xOffset, int yOffset);	// to minimize copy/pasting
	void allyWith(Nation * newAlly);	// handles the allying another nation
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	void initDiplomacy();
	void removeSelf();	//gets rid of self. called when the nation dies
	void updateRelationships();	// goes through all the diplomatic relationships and makes sure that any belligarants are actually in the opposing side in wars

public:
	
	string nationName;

	//these are the nation stats that determine how willing the nation is to do different actions
	int diplomat;
	int militant;
	int economist;

	vector<State *> controlledStates;

	Color nationColor;
	
	
	int warExhaustion = 0;	// if this ever exceeds 40, stabillity drops, but war warExhuastion drops back to 0
	int stability = 1;		// how stable the nation is. If ever negative, the nation breaks

	void breakNation();

	map<Nation *, DiplomaticView> diplomaticViews;			// a map of the nation's diplomatic views with others
	map<Nation *, DiplomaticRelation> diplomaticRelations;	// a map of the nation's diplomatic relationships with others

	

	//to be able to print info, we need a string output of these pairs
	void printDiplomaticView(Nation * nation);
	void printDiplomaticRelation(Nation * nation);
	
	Nation(Color color, string name);
	~Nation();

	//print out the nation's information
	void printInfo();

	//to add states to this nation's control, the following function should be called:
	void addContolledState(State* state);

	//this is where all the nation's thinking/behavior will go
	void update();

	//to increase/decrease relations
	void increaseRelations(Nation *nation);
	void decreaseRelations(Nation *nation);

	//how much resources the nation has. It is with the resources that the nation can excecute actions
	//gain resources at beginning of turn. The more development, the more resources gained
	//the nation also has to pay for state maitanace at the beginning of the turn, as there are more states there is an exponential amount more state maitenance 
	//every action takes a certian amount of resources
	//if resources ever go negative, the stabillity drops, resources go to 5 again
	float resources = 5;

	//things to do with income and stuff...
	float stateMaintenance;
	float taxIncome;
	float revenue;
	float armyMaintenance;

	//to find the total development of the nation:
	int getDevelopment();

	//nations beside eachother might have border conflicts. The longer te border the more chance of conflict
	void borderConflicts();

	//all the different actions the nation can do:
	void spread();	// to spread around in every possible direction
	void colonize();	// attempt to colonize a neighboring state with the most development
	void improveArmy();	// improves the "army" int by one
	void declareWarOnEnemyNeighbor();	// if there is a neighbor who is an enemy, and who's strength is relative or weaker than than the current nation's alliance, then go to war
		
	void improveRelationsWithNonAlly();	// finds a nation who is not an ally, has a good opinion of the current nation, and attempts to better relations with that nation
	void improveRelationsWithAlly();	// so that if there is an alliance that is not doing so well, the nation can attempt to save it
	void breakAlliance();	// if there is an alliance that this nation has with another who doesnt think so well of this nation, the nation can break the alliance
	void attemptToAlly();	// finds a nation who is not an ally, has a good opinion of the current nation, and attempts to ally them

	void improveRelationsWithVassal();	// if there is a vassal who is not friendly, then attempt to improve relations
	void attemptToIntegrateVassal();	// attempts to integrate a vassal into nation
	void maintainStabiliy();	// attempts to keep the nation's stability at a reasonable level
	void developeState();	// improves the development of a random state

	//things to do with war
	vector<War*> wars;	// all the wars that the nation is involved in
	int army = 1;	// how big/strong the nation's army is
	float armyStrength = 1;	// how damaged the army is
	void repairArmy();	// spend as much resources as possible to return "armyStrength" back to 1
	//actions that can only be excecuted while at war:
	void attackEnemyArmy();	// attempts to attack enemy army if army is comprarable or better than enemy's 
							// (+2 warscore and +0.2 army damadge if successful, -2 warscore and +1 army damadge if unsuccessful)
	void occupyEnemyState();	// attempts to occupy a neighboring (+1 warscore, and occupation of state if successful)
	// to end wars, these are the peace deals that the winner can make:
	void takeStates();	// attempt to take the most states possible
	void takeResources();
	void vassalize();
	void whitePeace();
};

