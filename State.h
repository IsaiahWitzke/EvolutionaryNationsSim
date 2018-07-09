#pragma once

#include <sfml/Graphics.hpp>
#include "Nation.h"
#include "HitBox.h"

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

	//a hit box for our state so we can click on it and it will output information to the console
	HitBox hitBox;
	void(*callBackFunction)();	//when the hit box is hit it will call this function
	void printInfo();			//this is the function that the callback function will become the callback

	State(bool isWater, Vector2f position, Vector2i positionInMap);
	~State();

	void update();
};

