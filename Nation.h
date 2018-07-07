#pragma once

#include <sfml/Graphics.hpp>
#include "State.h"
#include "MapHandler.h"
#include <vector>
#include <iostream>


using namespace sf;
using namespace std;

class State;
class MapHandler;

class Nation
{
private:


public:
	
	vector<State *> controlledStates;

	Color nationColor;
	
	Nation(Color color);
	~Nation();

	//to add states to this nation's control, the following function should be called:
	void addContolledState(State* state);

	//this is where all the nation's thinking/behavior will go
	void update();

	//to spread around in every possible direction
	void spread();
};

