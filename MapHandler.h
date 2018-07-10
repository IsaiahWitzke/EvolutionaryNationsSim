#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include "State.h"
#include "Nation.h"

using namespace sf;
using namespace std;

class State;
class Nation;

class MapHandler
{
private:
	//the function that gets data from a .bmp
	Image readBMP(string file);

public:

	int width, height;

	//the entire map. A 2D vector of states
	vector< vector<State> > states;

	//all the nations of the world
	vector <Nation> nations;

	MapHandler();
	~MapHandler();

	void updateStates();
};

