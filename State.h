#pragma once

#include <sfml/Graphics.hpp>
#include "Nation.h"


using namespace sf;

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
	Vector2i positionInMap;	//the position in the maps. ie: (2, 4) would be 2 states to the right and 4 states down.

	//this exact function can be found in the "ButtonTemplate" class as well. It will basically turn the state into a button
	//when the state is pressed, the state's info is outputted to the console
	bool isWithinHitBox(Vector2f possiblePoint);

	State(bool isWater, Vector2f position, Vector2i positionInMap);
	~State();

	void update();
};

