#pragma once

#include <sfml\Graphics.hpp>
#include <iostream>
#include <vector>
#include "ButtonTemplate.h"
#include "HitBox.h"
#include "State.h"
#include "Nation.h"

using namespace sf;

class InputHandler
{
private:
	bool isWithinHitbox(Vector2f possiblePoint, HitBox hitBox);
public:
	InputHandler();
	~InputHandler();

	//array of all buttons' and the states so we can "hit" them later on
	vector<ButtonTemplate> buttons;
	vector<State> states;

	//when the user clicks a nation it becomes the "selected nation"
	Nation * selectedNation = NULL;

	bool getInput(Event * event);
};

