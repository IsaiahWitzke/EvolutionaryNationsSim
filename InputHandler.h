#pragma once

#include <sfml\Graphics.hpp>
#include <iostream>
#include <vector>
#include "ButtonTemplate.h"


using namespace sf;

class InputHandler
{
private:

	Window *window;

public:
	InputHandler(Window *window);
	~InputHandler();

	//array of buttons
	vector<ButtonTemplate> buttons;

	bool getInput(Event * event);
};

