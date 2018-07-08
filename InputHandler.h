#pragma once

#include <sfml\Graphics.hpp>
#include <iostream>
#include <vector>
#include "ButtonTemplate.h"
#include "HitBox.h"

using namespace sf;

class InputHandler
{
private:
	bool isWithinHitbox(Vector2f possiblePoint, HitBox hitBox);
public:
	InputHandler();
	~InputHandler();

	//array of all buttons' hit boxes
	vector<HitBox> buttonHitBoxes;

	bool getInput(Event * event);
};

