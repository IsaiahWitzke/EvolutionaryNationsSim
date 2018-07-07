#pragma once

#include <sfml\Graphics.hpp>
#include <iostream>
#include <vector>
#include "ButtonTemplate.h"


using namespace sf;

class InputHandler
{
private:

public:
	InputHandler();
	~InputHandler();

	//array of buttons' hit box functions
	vector<ButtonTemplate*> buttons;

	bool getInput(Event * event);
};

