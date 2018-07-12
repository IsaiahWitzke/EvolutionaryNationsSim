//has all buttons in the scene, also keeps track of all the different callback functions that all the different buttons have

#pragma once
#include <vector>
#include "ButtonTemplate.h"
#include "MapHandler.h"

//all the callback functions any button will ever need
//for changing map modes
void developmentMapMode();
void defaultMapMode();

class ButtonHandler
{
public:
	//an array of all buttons
	vector<ButtonTemplate> buttons;

	//just goes through all the button's update functions 
	void updateButtons();

	ButtonHandler();
	~ButtonHandler();
};

