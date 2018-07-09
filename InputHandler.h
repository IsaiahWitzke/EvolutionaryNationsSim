#pragma once

#include <sfml\Graphics.hpp>
#include <iostream>
#include <vector>
#include "ButtonTemplate.h"
#include "HitBox.h"
#include "State.h"

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

	bool getInput(Event * event);
};

