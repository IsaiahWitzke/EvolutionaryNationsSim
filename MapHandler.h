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

//global enum for how the map should be drawn:
enum MapMode
{
	default,
	development
};

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
	vector <Nation*> nations;

	//the way the world is rendered.
	MapMode mapMode = MapMode::default;

	MapHandler();
	~MapHandler();

	void updateStates();
};

