#include "stdafx.h"
#include "Nation.h"


Nation::Nation(Color color)
{
	this->nationColor = color;
}

Nation::~Nation()
{
}

void Nation::addContolledState(State * state)
{
	//right away, no nation can own a water province;
	if (state->isWater)
	{
		return;
	}

	//otherwise we want to set up the two-way link between the nation and the state
	this->controlledStates.push_back(state);
	state->controller = this;
}

void Nation::spread(MapHandler *map)
{
	//first, check around every state that the nation controls,
	//if the nations around the controlled states does not have any owner then "addContolledState" that state


	vector<State *> tempControlledStates = controlledStates;
	int controlledStatesSize = controlledStates.size();

	for (int i = 0; i < controlledStatesSize; i++)
	{
		//checking states around current one
		//some relative coordinates
		int x = controlledStates[i]->positionInMap.x;
		int y = controlledStates[i]->positionInMap.y;
		//checking to the left
		if (x != 0 && map->states[x - 1][y].controller == NULL)
		{
			//if there a valid state to the left, then we will append it to the temp list of states
			addContolledState(&map->states[x - 1][y]);
		}
		//to the right
		if (x != map->width && map->states[x + 1][y].controller == NULL)
		{
			addContolledState(&map->states[x + 1][y]);
		}
		//down
		if (y != map->height && map->states[x][y + 1].controller == NULL)
		{
			addContolledState(&map->states[x][y + 1]);
		}
		//up
		if (y != 0 && map->states[x][y - 1].controller == NULL)
		{
			addContolledState(&map->states[x][y - 1]);
		}
	}
}

void Nation::update(MapHandler *map)
{
	//IN FUTURE:
	//there will be an array of functions/actions that the nation can do 
	//every nation has the same set of functions/actions, however, every nation has the array in a unique order
	//this update function will cycle through the array from top to bottom and attempt to do the actions at the top first
	//this is how different nations differ from each other

	//for now, the nation will just attempt to spread out in all directions forever
	spread(map);
}

