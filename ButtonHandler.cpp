#include "stdafx.h"
#include "ButtonHandler.h"

extern MapHandler g_map;

void developmentMapMode()
{
	g_map.mapMode = MapMode::development;
}

void defaultMapMode()
{
	g_map.mapMode = MapMode::default;
}

void opinionMapMode()
{
	g_map.mapMode = MapMode::opinion;
}

void ButtonHandler::update()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].update();
	}
}

ButtonHandler::ButtonHandler()
{
}


ButtonHandler::~ButtonHandler()
{
}
