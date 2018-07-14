#include "stdafx.h"
#include "Nation.h"
#include <windows.h>

extern MapHandler g_map;

string Nation::getDiplomaticView(Nation * nation)
{
	{
		DiplomaticView nationView = diplomaticViews[nation];
		switch (nationView)
		{
		case close:
			return "close";
		case friendly:
			return "friendly";
		case warm:
			return "warm";
		case neutral:
			return "neutral";
		case uneasy:
			return "unstable";
		case hostile:
			return "hostile";

		default:
			return "Nation::getDiplomaticView needs to be updated. Diplomatic View is not understood";
		}
	}
}

string Nation::getDiplomaticRelation(Nation * nation)
{
	DiplomaticRelation nationRelation = diplomaticRelations[nation];
	switch (nationRelation)
	{
	case nothing:
		return "nothing";
	case ally:
		return "ally";
	case belligerent:
		return "belligerent";
	case vasal:
		return "vasal";
	case overlord:
		return "overlord";

	default:
		return "Nation::getDiplomaticRelation needs to be updated. Diplomatic relation is not understood";
	}
}

Nation::Nation(Color color, string name)
{
	this->nationColor = color;
	this->nationName = name;
}

Nation::~Nation()
{
}

void Nation::printInfo()
{
	cout << "Name: " << nationName << endl << endl;

	cout << "Size: " << controlledStates.size() << " states" << endl;
	cout << "Total development: " << getDevelopment() << endl;
	cout << "wallet: " << resources << endl;
	cout << "Revenue: " << revenue << endl;
	cout << "Army strength: " << army << endl;
	
	cout << endl;

	//printing out the relationships with everyone:
	if (diplomaticViews.size() != 0)
	{
		cout << "Dipolmacy: " << endl;
		for (auto& x : diplomaticViews)
		{
			cout << "   feels " << getDiplomaticView(x.first) << " towards " << x.first->nationName << endl;
			cout << "   sees " << x.first->nationName << " as a " << getDiplomaticRelation(x.first) << endl;
		}
	}

	//displaying who the nation is at war with
	if (wars.size() != 0)
	{
		cout << "Involved in the following wars: " << endl;
		for (int i = 0; i < wars.size(); i++)
		{
			wars[i]->printInfo();
		}
	}

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

int Nation::getDevelopment()
{
	int development = 0;

	for (int i = 0; i < controlledStates.size(); i++)
	{
		development += controlledStates[i]->development;
	}

	return development;
}

void Nation::borderConflicts()
{
	//for every state that borders another nation, there is a 1% chance of the diplomatic view of this nation decreasing
	for (int i = 0; i < controlledStates.size(); i++)
	{
		//checking states around current one
		//some relative coordinates
		int x = controlledStates[i]->positionInMap.x;
		int y = controlledStates[i]->positionInMap.y;
		//checking to the left
		if (x != 0 && g_map.states[x - 1][y].controller != NULL && g_map.states[x - 1][y].controller != this)
			if (rand() % 100 == 1)	//1% chance
				decreaseRelations(g_map.states[x - 1][y].controller);

		//to the right
		if (x != g_map.width - 1 && g_map.states[x + 1][y].controller != NULL && g_map.states[x + 1][y].controller != this)
			if (rand() % 100 == 1)
				decreaseRelations(g_map.states[x + 1][y].controller);
		//down
		if (y != g_map.height - 1 && g_map.states[x][y + 1].controller != NULL && g_map.states[x][y + 1].controller != this)
			if (rand() % 100 == 1)
				decreaseRelations(g_map.states[x][y + 1].controller);
		//up
		if (y != 0 && g_map.states[x][y - 1].controller != NULL && g_map.states[x][y - 1].controller != this)
			if (rand() % 100 == 1)
				decreaseRelations(g_map.states[x][y - 1].controller);
	}
}

void Nation::spread()
{
	//first, check around every state that the nation controls,
	//if the nations around the controlled states does not have any owner then "addContolledState" that state
	int controlledStatesSize = controlledStates.size();

	for (int i = 0; i < controlledStatesSize; i++)
	{
		//checking states around current one
		//some relative coordinates
		int x = controlledStates[i]->positionInMap.x;
		int y = controlledStates[i]->positionInMap.y;
		//checking to the left
		if (x != 0 && g_map.states[x - 1][y].controller == NULL)
		{
			//if there a valid state to the left, then we will append it to the temp list of states
			addContolledState(&g_map.states[x - 1][y]);
		}
		//to the right
		if (x != g_map.width-1 && g_map.states[x + 1][y].controller == NULL)
		{
			addContolledState(&g_map.states[x + 1][y]);
		}
		//down
		if (y != g_map.height-1 && g_map.states[x][y + 1].controller == NULL)
		{
			addContolledState(&g_map.states[x][y + 1]);
		}
		//up
		if (y != 0 && g_map.states[x][y - 1].controller == NULL)
		{
			addContolledState(&g_map.states[x][y - 1]);
		}
	}
}

void Nation::colonize()
{
	//it will take 10 resources to complete this task:
	if (this->resources < 10)
	{
		return;
	}

	//to keep track of the best looking state to colonize
	State *bestOption = NULL;

	for (int i = 0; i < controlledStates.size(); i++)
	{
		//checking states around current one
		//some relative coordinates
		int x = controlledStates[i]->positionInMap.x;
		int y = controlledStates[i]->positionInMap.y;
		//checking to the left
		if (x != 0 && g_map.states[x - 1][y].controller == NULL)
		{
			//if there a valid state to the left, then we will see if it is any better than the current "bestOption"
			if (bestOption == NULL || g_map.states[x - 1][y].development > bestOption->development)
			{
				bestOption = &g_map.states[x - 1][y];
			}
		}
		//to the right
		if (x != g_map.width - 1 && g_map.states[x + 1][y].controller == NULL)
		{
			if (bestOption == NULL || g_map.states[x + 1][y].development > bestOption->development)
			{
				bestOption = &g_map.states[x + 1][y];
			}
		}
		//down
		if (y != g_map.height - 1 && g_map.states[x][y + 1].controller == NULL)
		{
			if (bestOption == NULL || g_map.states[x][y + 1].development > bestOption->development)
			{
				bestOption = &g_map.states[x][y + 1];
			}
		}
		//up
		if (y != 0 && g_map.states[x][y - 1].controller == NULL)
		{
			if (bestOption == NULL || g_map.states[x][y - 1].development > bestOption->development)
			{
				bestOption = &g_map.states[x][y - 1];
			}
		}
	}

	//if there are no states possible to colonize, then just stop:
	if (bestOption == NULL)
	{
		return;
	}

	//otherwise we want to add the bestOption to the control of the nation and move take away resources:
	addContolledState(bestOption);
	resources -= 10;
}

void Nation::improveArmy()
{
	if (resources > 10)
	{
		resources -= 10;
		this->army += 1;
	}
}

void Nation::declareWarOnEnemyNeighbor()
{
	//for every state that borders another nation, check to see if there is a enemy nation there:
	for (int i = 0; i < controlledStates.size(); i++)
	{
		//checking states around current one
		//some relative coordinates
		int x = controlledStates[i]->positionInMap.x;
		int y = controlledStates[i]->positionInMap.y;
		
		//checking to the left
		x -= 1;
		warWithStateOffset(x, y);
		//right
		x += 2;
		warWithStateOffset(x, y);
		//down
		x -= 1;
		y += 1;
		warWithStateOffset(x, y);
		//up
		y -= 2;
		warWithStateOffset(x, y);
	}
}

void Nation::warWithStateOffset(int x, int y)
{
	//return if the state we are trying to check is outside of the map
	if (x < 0 || y < 0 || x >= g_map.width || y >= g_map.height)
		return;

	Nation * possibleBelligerent = g_map.states[x][y].controller;

	//check to see if the neighbooring state is actualy a different nation
	if (possibleBelligerent != NULL && possibleBelligerent != this)
	{
		//checking to see if it is a valid target to declare war on
		if (diplomaticViews[possibleBelligerent] == DiplomaticView::hostile &&
			diplomaticRelations[possibleBelligerent] != DiplomaticRelation::belligerent &&
			diplomaticRelations[possibleBelligerent] != DiplomaticRelation::ally)
		{
			//changing color of console output
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
			cout << endl;
			cout << this->nationName << " declares war on " << possibleBelligerent->nationName << endl;
			SetConsoleTextAttribute(hConsole, 7);

			//actually do war stuff
			new War(this, possibleBelligerent);
			return;
		}
	}
}

void Nation::allyWith(Nation * newAlly)
{
	newAlly->diplomaticRelations[this] = DiplomaticRelation::ally;
	newAlly->increaseRelations(this);
	this->diplomaticRelations[newAlly] = DiplomaticRelation::ally;
	this->increaseRelations(newAlly);
}

void Nation::update()
{
	//for the first time that the update function is called, we want to initialize the diplomatic views/reationships with other nations
	if (diplomaticViews.size() == 0)
	{
		for (int i = 0; i < g_map.nations.size(); i++)
		{
			//we don't need relationships with ourselves
			if (g_map.nations[i] == this)
			{
				continue;
			}
			this->diplomaticViews.emplace(g_map.nations[i], DiplomaticView::neutral);
			this->diplomaticRelations.emplace(g_map.nations[i], DiplomaticRelation::nothing);
		}
		//everybody is going to ally afganastan for testing purposes:
		if (this != g_map.nations[0])
			allyWith(g_map.nations[0]);
	}

	//IN FUTURE:
	//there will be an array of functions/actions that the nation can do 
	//every nation has the same set of functions/actions, however, every nation has the array in a unique order
	//this update function will cycle through the array from top to bottom and attempt to do the actions at the top first
	//this is how different nations differ from each other

	//MONEY STUFF

	//income: development * 0.05
	this->taxIncome = getDevelopment() * 0.05;
	//state maitenence: (0.3 x number of states)^2
	this->stateMaintenance = pow((controlledStates.size())*0.3, 1.5);

	this->revenue = taxIncome - stateMaintenance;
	this->resources += revenue;

	//DIPLOMATIC STUFF

	//chance of decreasing relations with other nations bordering the current country
	borderConflicts();

	declareWarOnEnemyNeighbor();

	//if at war attack the enemy army
	if (wars.size() != 0)
	{
		attackEnemyArmy();
	}

	while (true)
	{
		int statesBeforeColonizing = controlledStates.size();
		colonize();
		improveArmy();
		if (controlledStates.size() == statesBeforeColonizing)
			break;
	}
}

void Nation::increaseRelations(Nation *nation)
{
	using IntType = typename std::underlying_type<DiplomaticView>::type;
	if (this->diplomaticViews[nation] == DiplomaticView::close)	//if we are at the max possible relations, then just leave
		return;
	this->diplomaticViews[nation] = static_cast<DiplomaticView>(static_cast<IntType>(this->diplomaticViews[nation]) + 1);
}

void Nation::decreaseRelations(Nation *nation)
{
	
	using IntType = typename std::underlying_type<DiplomaticView>::type;
	if (this->diplomaticViews[nation] == DiplomaticView::hostile)	//if we are at the min possible relations, then just leave
		return;
	this->diplomaticViews[nation] = static_cast<DiplomaticView>(static_cast<IntType>(this->diplomaticViews[nation]) - 1);
}

void Nation::attackEnemyArmy()
{
	//look at all wars
	//if it would be a half decent idea to attack the enemy, then do so

}
