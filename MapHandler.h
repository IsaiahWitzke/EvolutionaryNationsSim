#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include "State.h"

using namespace sf;
using namespace std;

class State;

class MapHandler
{
private:
	//the function that gets data from a .bmp
	Image readBMP(string file);

public:

	int width, height;

	//the entire map. A 2D vector of states
	vector< vector<State> > states;

	MapHandler();
	~MapHandler();

	void updateStates(RenderWindow * window);
};

