#pragma once

#include <sfml/Graphics.hpp>
#include "Nation.h"
#include "HitBox.h"
#include "MapHandler.h"
#include <functional>

using namespace sf;
using namespace std;

class Nation;

class State
{
private:
	
	//used in the initialization of the state:
	Texture makeDefaultTexture();

public:

	//sprite related members
	Texture texture;
	Sprite sprite;
	Color color;
	Vector2f position;
	Vector2u size;

	//the nation that controls this state 
	Nation* controller;

	//data about state
	bool isWater;
	int development = 0;	// a value from 0-255 describing how valueable the state is
	Vector2i positionInMap;	// the position in the maps. ie: (2, 4) would be 3 states to the right and 5 states down.

	//a hit box for our state so we can click on it and it will output information to the console
	HitBox hitBox;

	void printInfo(); // when the hit box is hit it will call this function

	State(bool isWater, Vector2f position, Vector2i positionInMap, int development);
	~State();

	void update();
};

