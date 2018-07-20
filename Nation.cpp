#include "stdafx.h"
#include "Nation.h"
#include <stdlib.h>

extern MapHandler g_map;

Nation::Nation(Color color, string name)
{
	this->nationColor = color;
	this->nationName = name;

	//initializing the nation's stats
	this->diplomat = rand() % 10;
	this->militant = rand() % 10;
	this->economist = rand() % 10;
}

Nation::~Nation()
{
}

void Nation::breakNation()
{
	//starting new nations
	vector<Nation*> newNations;
	for (int i = 0; i < controlledStates.size(); i++)
	{
		//every 100th state will be the start of a new nation
		if (i % 100 == 1)
		{
			//finding the name of the nation in the "NationNames.txt" file (found in the assets folder)
			string nationNamesPath = "Assets/NationNames.txt";
			string newNationName = "";
			string lineInTxt;
			ifstream myfile(nationNamesPath);
			if (myfile.is_open())
			{
				//a flag that turns true once a valid nation name has been found
				bool foundNationName = false;
				while (getline(myfile, lineInTxt) && !foundNationName)
				{
					//making sure that there is no nation already named the nation name
					bool foundSimillarNationName = false;
					for (int j = 0; j < g_map.nations.size(); j++)
					{
						//if we found a simmilar nation name, then we want to get a new line from the txt and try again
						if (g_map.nations[j]->nationName == lineInTxt)
							foundSimillarNationName = true;
					}
					if (foundSimillarNationName)
						continue;
					else
					{
						newNationName = lineInTxt;
						foundNationName = true;
					}
				}
				//if we havent found a nation name, give an error:
				if (!foundNationName)
				{
					cout << "Unable to find enough unique names in file " << nationNamesPath << " try adding some new nation names? (Nation.cpp)" << endl;
					myfile.close();
					system("pause");
				}

				myfile.close();
			}
			else
			{
				cout << "Unable to open file " << nationNamesPath << endl;
				system("pause");
			}

			//figuring out a random color
			int r = rand() % 255;
			int g = rand() % 255;
			int b = rand() % 255;

			Nation *tempNation = new Nation(Color(r,g,b,255), newNationName);
			newNations.push_back(tempNation);

			//adding the nation to the array of nations
			g_map.nations.push_back(tempNation);
			g_map.nations[g_map.nations.size() - 1]->addContolledState(controlledStates[i]);
		}
	}

	//get rid of all the previous nation's states
	for (int i = 0; i < controlledStates.size(); i++)
	{
		controlledStates[i]->controller = NULL;
	}

	removeSelf();

	for (int i = 0; i < newNations.size(); i++)
	{
		newNations[i]->initDiplomacy();
	}

	//go through all the nations and spread them out as mush as possible
	bool isSpreading = true;
	while (isSpreading)
	{
		isSpreading = false;
		for (int i = 0; i < newNations.size(); i++)
		{
			int statesBeforeSpreading = newNations[i]->controlledStates.size();
			newNations[i]->spread();

			if (statesBeforeSpreading != newNations[i]->controlledStates.size())
				isSpreading = true;
		}
	}
}

void Nation::printDiplomaticView(Nation * nation)
{
	DiplomaticView nationView = diplomaticViews[nation];
	switch (nationView)
	{
	case close:
		SetConsoleTextAttribute(hConsole, 10);	// light green
		cout << "close";
		break;
	case friendly:
		SetConsoleTextAttribute(hConsole, 2);	// dark green
		cout << "friendly";
		break;
	case warm:
		SetConsoleTextAttribute(hConsole, 3);	// blue-ish
		cout << "warm";
		break;
	case neutral:
		SetConsoleTextAttribute(hConsole, 9);	// blue
		cout << "neutral";
		break;
	case uneasy:
		SetConsoleTextAttribute(hConsole, 12);	// dark red
		cout << "unstable";
		break;
	case hostile:
		SetConsoleTextAttribute(hConsole, 4);	// bright red
		cout << "hostile";
		break;
	default:
		cout << "Nation::printDiplomaticView needs to be updated. Diplomatic View is not understood";
		system("pause");
	}

	SetConsoleTextAttribute(hConsole, 7);
}

void Nation::printDiplomaticRelation(Nation * nation)
{
	

	DiplomaticRelation nationRelation = diplomaticRelations[nation];
	switch (nationRelation)
	{
	case nothing:
		SetConsoleTextAttribute(hConsole, 9);	// blue
		cout << "nothing";
		break;
	case ally:
		SetConsoleTextAttribute(hConsole, 10);	// light green
		cout << "ally";
		break;
	case belligerent:
		SetConsoleTextAttribute(hConsole, 12);	// red
		cout << "belligerent";
		break;
	case vassal:
		SetConsoleTextAttribute(hConsole, 14);	// yellow
		cout << "vasal";
		break;
	case overlord:
		SetConsoleTextAttribute(hConsole, 5);	// purple
		cout << "overlord";

	default:
		cout << "Nation::printDiplomaticRelation needs to be updated. Diplomatic relation is not understood";
		system("pause");
	}

	SetConsoleTextAttribute(hConsole, 7);
}

void Nation::printInfo()
{
	cout << "Name: " << nationName << endl << endl;

	cout << "Nation's stats: " << endl;
	cout << "  diplomat: " << diplomat << endl;
	cout << "  militant: " << militant << endl;
	cout << "  economist: " << economist << endl << endl;

	cout << "Stability: " << stability << endl;
	cout << "War Exhaustion: " << warExhaustion << endl;

	cout << "Size: " << controlledStates.size() << " states" << endl;
	cout << "Total development: " << getDevelopment() << endl;
	cout << "Wallet: " << resources << endl;
	cout << "Revenue: "; 
	//if nation making money, revenue is green, otherwise, it is red:
	if (revenue > 0)
		SetConsoleTextAttribute(hConsole, 2);	// green
	else
		SetConsoleTextAttribute(hConsole, 12);	// red
	cout << revenue << endl;
	SetConsoleTextAttribute(hConsole, 7);	// white
	cout << "Army size: " << army << endl;
	cout << "Army strength: " << armyStrength << endl;
	
	cout << endl;

	//printing out the relationships with everyone:
	if (diplomaticViews.size() != 0)
	{
		cout << "Dipolmacy: " << endl;
		for (auto& x : diplomaticViews)
		{
			cout << "   feels ";
			printDiplomaticView(x.first); 
			cout << " towards " << x.first->nationName << endl;
			cout << "   sees " << x.first->nationName << " as a ";
			printDiplomaticRelation(x.first);
			cout << endl;
		}
	}

	//displaying who the nation is at war with
	if (wars.size() != 0)
	{
		cout << endl;
		cout << "Involved in the following wars: " << endl;
		for (int i = 0; i < wars.size(); i++)
		{
			wars[i]->printInfo(this);
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

	//if it has a controller, get the controlled to give up the state
	if (state->controller != NULL)
	{
		for (int i = 0; i < state->controller->controlledStates.size(); i++)
		{
			if (state->controller->controlledStates[i] == state)
			{
				state->controller->controlledStates.erase(state->controller->controlledStates.begin() + i);
			}
		}
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
	//this action can be done multiple times per turn:
	while (true)
	{
		// nations of all kinds would see colonizing as a good thing to do
		// but an economist would want to do this action a lot more 
		int chanceOfAction = 100 * (float(economist) / 10 + 0.5);
		int outcome = rand() % 100;
		if (outcome > chanceOfAction)
			return;

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
				//if there a valid state to the left, then we will see if it is any better than the current "bestOption"
				if (bestOption == NULL || g_map.states[x - 1][y].development > bestOption->development)
					bestOption = &g_map.states[x - 1][y];
			//to the right
			if (x != g_map.width - 1 && g_map.states[x + 1][y].controller == NULL)
				if (bestOption == NULL || g_map.states[x + 1][y].development > bestOption->development)
					bestOption = &g_map.states[x + 1][y];
			//down
			if (y != g_map.height - 1 && g_map.states[x][y + 1].controller == NULL)
				if (bestOption == NULL || g_map.states[x][y + 1].development > bestOption->development)
					bestOption = &g_map.states[x][y + 1];
			//up
			if (y != 0 && g_map.states[x][y - 1].controller == NULL)
				if (bestOption == NULL || g_map.states[x][y - 1].development > bestOption->development)
					bestOption = &g_map.states[x][y - 1];
		}

		//if there are no states possible to colonize, then just stop:
		if (bestOption == NULL)
			return;

		//otherwise we want to add the bestOption to the control of the nation and move take away resources:
		addContolledState(bestOption);
		resources -= 10;
	}
}

void Nation::improveArmy()
{
	//a very militant, economist would want to do this action
	int chanceOfAction = 100 * (float(militant) / 7 + float(economist) / 15);
	int outcome = rand() % 100;
	if (outcome > chanceOfAction)
		return;

	//its an exponetial relationship between the current army strength and the cost to improve the army
	float costToImproveArmy = pow(2, army / 5) + 10;

	if (resources > costToImproveArmy)
	{
		resources -= costToImproveArmy;
		this->army += 1;
	}
}

void Nation::declareWarOnEnemyNeighbor()
{
	//how much this nation wants to go to war: 100 % * (militant/10 - warExhaustion/100 - #of wars involved in/2)
	int chanceOfAction = 100 * (float(militant)/10 - float(warExhaustion)/100 - float(wars.size())/2);
	int outcome = rand() % 100;
	if (outcome > chanceOfAction)
		return;

	//for every state that borders another nation, check to see if there is a enemy nation there:
	for (int i = 0; i < controlledStates.size(); i++)
	{
		//checking states around current one
		//some relative coordinates
		int x = controlledStates[i]->positionInMap.x;
		int y = controlledStates[i]->positionInMap.y;
		
		//checking to the left
		x -= 1;
		if (warWithStateOffset(x, y))	//so that only one war can be declared per "turn"
			return;
		//right
		x += 2;
		if (warWithStateOffset(x, y))
			return;
		//down
		x -= 1;
		y += 1;
		if (warWithStateOffset(x, y))
			return;
		//up
		y -= 2;
		if (warWithStateOffset(x, y))
			return;
	}
}

bool Nation::warWithStateOffset(int x, int y)
{
	//return if the state we are trying to check is outside of the map
	if (x < 0 || y < 0 || x >= g_map.width || y >= g_map.height)
		return false;

	Nation * possibleBelligerent = g_map.states[x][y].controller;

	//check to see if the neighbooring state is actualy a different nation
	if (possibleBelligerent != NULL && possibleBelligerent != this)
	{
		//checking to see if it is a valid target to declare war on
		if (diplomaticViews[possibleBelligerent] == DiplomaticView::hostile &&
			diplomaticRelations[possibleBelligerent] != DiplomaticRelation::belligerent &&
			diplomaticRelations[possibleBelligerent] != DiplomaticRelation::ally)
		{
			//sizing up the enemy (if the enemy is 25% bigger than this nation):
			if (getDevelopment() < 0.75 * float(possibleBelligerent->getDevelopment()))
				return false;
			//changing color of console output
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
			cout << endl;
			cout << this->nationName << " declares war on " << possibleBelligerent->nationName << endl;
			SetConsoleTextAttribute(hConsole, 7);

			//actually do war stuff
			new War(this, possibleBelligerent);
			return true;
		}
	}
	
	return false;
}

void Nation::allyWith(Nation * newAlly)
{
	newAlly->diplomaticRelations[this] = DiplomaticRelation::ally;
	newAlly->increaseRelations(this);
	this->diplomaticRelations[newAlly] = DiplomaticRelation::ally;
	this->increaseRelations(newAlly);
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

void Nation::repairArmy()
{
	while (armyStrength < 1 && resources > 0.5)
	{
		//takes more money to heal a bigger army:
		float armyRepairFactor = 1 / float(army);
		armyStrength += armyRepairFactor;
		if (armyStrength > 1)
			armyStrength = 1;
		resources -= 0.5;
	}
}

void Nation::attackEnemyArmy()
{
	//look at all wars
	//if it would be a half decent idea to attack the enemy, then do so
	for (int i = 0; i < wars.size(); i++)
	{
		//first, figure out if we are the defender
		bool isDefender = false;
		for (int j = 0; j < wars[i]->defenders.size(); j++)
		{
			if (wars[i]->defenders[j] == this)
				isDefender = true;
		}

		//if we are the attacker and the "chanceOfAttackerWinningBattle" is > 0.4, then do it
		if (float(wars[i]->chanceOfAttackerWinningBattle()) > 0.4 && !isDefender)
		{
			wars[i]->battle();
			continue;
		}
		//if we are the defender and the "chanceOfAttackerWinningBattle" is < 0.4, then do it
		if (float(wars[i]->chanceOfAttackerWinningBattle()) < 0.4 && isDefender)
		{
			wars[i]->battle();
			continue;
		}
	}
}

void Nation::takeStates()
{
	//go through all wars
	for (int i = 0; i < wars.size(); i++)
	{
		//first find out if we are the lead defender/attacker
		bool isAttacker = true;
		if (wars[i]->leadAttacker != this)
		{
			isAttacker = false;
			if (wars[i]->leadDefender != this)
			{
				continue;	// only war leaders can make demands
			}
		}

		//we need at least 5 warscore to achieve this deal, so if we are the attacker and the warscore is less than 5, move on:
		if (wars[i]->warScore < 5 && wars[i]->leadAttacker == this)
		{
			continue;
		}
		//same with defenders, except negative
		if (wars[i]->warScore > -5 && wars[i]->leadDefender == this)
		{
			continue;
		}

		//figure out if it is time to end the war. If we can continue to obliterate the enemy then continue, otherwise, stop the war
		float alliesStrength = 0;
		float enemyStrength = 0;
		for (auto& diplomaticRelationPair : diplomaticRelations)
		{
			if (diplomaticRelationPair.second == DiplomaticRelation::ally)
			{
				alliesStrength += diplomaticRelationPair.first->army*diplomaticRelationPair.first->armyStrength;
			}
			if (diplomaticRelationPair.second == DiplomaticRelation::belligerent)
			{
				enemyStrength += diplomaticRelationPair.first->army*diplomaticRelationPair.first->armyStrength;
			}
		}

		//if our side is better than theirs AND we havent already obliterated them, then keep on going with the current war
		if (alliesStrength > enemyStrength && wars[i]->warScore < 100)
		{
			continue;
		}
		
		//for simplicity, the warscore will just be possitve, and every time a state is taken, 5 will be deducted
		wars[i]->warScore = abs(wars[i]->warScore);

		//we are trying to take as many states as possible
		while (wars[i]->warScore >= 5)
		{
			//for every state neighbooring this one, check to see if the controling nation is the lead beligerent to this nation, if so, take the state:
			for (int j = 0; j < controlledStates.size(); j++)
			{
				//checking states around current one
				//some relative coordinates
				int x = controlledStates[j]->positionInMap.x;
				int y = controlledStates[j]->positionInMap.y;

				//checking to the left
				x -= 1;
				if (x > -1 && isAttacker && (g_map.states[x][y].controller == wars[i]->leadDefender))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
				if (x > -1 && !isAttacker && (g_map.states[x][y].controller == wars[i]->leadAttacker))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
				//right
				x += 2;
				if (x < g_map.width && isAttacker && (g_map.states[x][y].controller == wars[i]->leadDefender))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
				if (x < g_map.width && !isAttacker && (g_map.states[x][y].controller == wars[i]->leadAttacker))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
				//down
				x -= 1;
				y += 1;
				if (y < g_map.height && isAttacker && (g_map.states[x][y].controller == wars[i]->leadDefender))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
				if (y < g_map.height && !isAttacker && (g_map.states[x][y].controller == wars[i]->leadAttacker))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
				//up
				y -= 2;
				if (y > -1 && isAttacker && (g_map.states[x][y].controller == wars[i]->leadDefender))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
				if (y > -1 && !isAttacker && (g_map.states[x][y].controller == wars[i]->leadAttacker))
				{
					addContolledState(&g_map.states[x][y]);
					wars[i]->warScore -= 5;
					continue;
				}
			}

			//if we got here without anything happening, then get out, no possible states left to take:
			break;
		}

		SetConsoleTextAttribute(hConsole, 5);	// purple
		cout << wars[i]->warName << " has ended" << endl;
		wars[i]->endWar();
		takeStates(); // recursion so that all possible wars can be ended
		SetConsoleTextAttribute(hConsole, 7);	// back to white
		return;	// once all possible wars have ended, return
	}
}

void Nation::initDiplomacy()
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
}

void Nation::removeSelf()
{

	//go through all nations and wars and get rid of references to self
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		//we don't need relationships with ourselves
		if (g_map.nations[i] == this)
		{
			continue;
		}

		g_map.nations[i]->diplomaticViews.erase(this);
		g_map.nations[i]->diplomaticRelations.erase(this);
	}

	//go through all the wars and get rid of this
	for (int i = 0; i < wars.size(); i++)
	{
		for (int j = 0; j < wars[i]->defenders.size(); j++)
		{
			//if we have came across ourselves, get rid of it
			if (wars[i]->defenders[j] == this)
			{
				wars[i]->defenders.erase(wars[i]->defenders.begin() + j);
				//now if all defenders are gone, end the war
				if (wars[i]->defenders.size() == 0)
					wars[i]->endWar();
			}
			
		}
		//same for attackers
		for (int j = 0; j < wars[i]->attackers.size(); j++)
		{
			if (wars[i]->attackers[j] == this)
			{
				wars[i]->attackers.erase(wars[i]->attackers.begin() + j);
				if (wars[i]->attackers.size() == 0)
					wars[i]->endWar();
			}
		}
	}

	//finally, get rid of self in the g_map so it will never be updated again
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		//we don't need relationships with ourselves
		if (g_map.nations[i] == this)
		{
			g_map.nations.erase(g_map.nations.begin() + i);
		}
	}
}

void Nation::updateRelationships()
{
	//itterate through the nation's diplo relations, if the relation is not ally, overlord, or vasal, assume nothing
	for (auto &i : diplomaticRelations)
	{
		if (i.second == ally || i.second == vassal || i.second == overlord)
			continue;
		i.second = nothing;
	}

	//makes sure that any belligarants are actually in the opposing side in wars
	for (int warIndex = 0; warIndex < wars.size(); warIndex++)
	{
		//is this the defender?
		bool isDefender = false;
		for (int i = 0; i < wars[warIndex]->defenders.size(); i++)
		{
			if (wars[warIndex]->defenders[i] == this)
			{
				isDefender = true;
			}
		}

		//all of those who should be this nation's belligerents
		vector<Nation*> belligerents;
		if (isDefender)
			belligerents = wars[warIndex]->attackers;
		else
			belligerents = wars[warIndex]->defenders;
	
		for (int i = 0; i < belligerents.size(); i++)
		{
			belligerents[i]->diplomaticRelations[this] = DiplomaticRelation::belligerent;
			this->diplomaticRelations[belligerents[i]] = DiplomaticRelation::belligerent;
		}
	}
}


void Nation::update()
{
	//if the nation has no stabillity, it breaks
	if (stability < 1)
		breakNation();

	//if the nation has 0 states, it needs to be removed:
	if (controlledStates.size() == 0)
		removeSelf();

	//for the first time that the update function is called, we want to initialize the diplomatic views/reationships with other nations
	if (diplomaticViews.size() == 0)
		initDiplomacy();

	updateRelationships();

	//MONEY STUFF

	//income: development * 0.05
	this->taxIncome = getDevelopment() * 0.05;
	//state maitenence: (0.3 x number of states)^2
	this->stateMaintenance = pow((controlledStates.size())*0.3, 1.5);
	//same with army
	this->armyMaintenance = pow((army)*0.3, 1.5);

	this->revenue = taxIncome - stateMaintenance - armyMaintenance;
	this->resources += revenue;

	
	//STABILLITY STUFF

	//there is a 0.5% chance of regaining stabillity if the nation is not at war (also war exhaustion decreases)
	if (wars.size() == 0)
	{
		if (rand() % 200 == 1 && stability < 5)
			this->stability++;
		if (rand() % 10 == 1 && warExhaustion > 0)
			this->warExhaustion--;
	}

	//going into debt
	if (resources < 0)
	{
		this->stability--;
		this->resources = abs(revenue) * 5;	// enough cash to last for another 5 "turns"
	}

	//war exhaustion 
	if (warExhaustion >= 40)
	{
		this->stability--;
		this->warExhaustion = 0;	//reset warExhaustion
	}

	//DIPLOMATIC STUFF

	//chance of decreasing relations with other nations bordering the current country
	borderConflicts();

	declareWarOnEnemyNeighbor();

	//peace deals
	if (wars.size() != 0)
	{
		takeStates();
	}

	//repair army as much as possible before going into battle
	repairArmy();

	//if at war attack the enemy army
	if (wars.size() != 0)
	{
		attackEnemyArmy();
	}
	//repair army as much as possible (again)
	repairArmy();

	//all other actions
	improveArmy();
	colonize();
}
