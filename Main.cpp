// Main.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "State.h"
#include "MapHandler.h"
#include "InputHandler.h"
#include "ButtonTemplate.h"
#include "ButtonHandler.h"
#include "TextHandler.h"
#include <iostream>
#include <sfml/Graphics.hpp>

using namespace std;
using namespace sf;

float GLOBALSCALE = 5;
Vector2f GLOBALOFFSET = Vector2f(0, 0);

RenderWindow g_window(VideoMode(1000, 1000), "Evolutionary Nations");

//the _settings file can be edited by the user to generate different initial simulation variables
SettingInput g_settingInput = SettingInput("_Settings.txt");	

//the entire collection of states the entire simulation is based off of
MapHandler g_map;

//all input is handled here
InputHandler g_inputHandler;

//all buttons
ButtonHandler g_buttonHandler;

//all text
TextHandler g_textHandler;

//if false, then the nations do not update
bool g_isNationUpdate = true;

//number of nation updates since start
int g_timeSinceStart = 0;

//everytime the screen needs to be updated this is called
void g_update()
{
	g_window.clear();
	g_inputHandler.buttons.clear();	//want to get rid of all buttons and states because their update functions will add them back in
	g_inputHandler.states.clear();
	g_map.updateStates();
	g_buttonHandler.update();
	g_textHandler.update();
	g_window.display();
}

void g_updateNations()
{
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		g_map.nations[i]->update();
	}
	//updating the date text
	g_timeSinceStart++;
	string sTimeSinceStart = to_string(g_timeSinceStart);
	g_textHandler.staticText[0].setString(sTimeSinceStart);
}

//couple of global functions from "ButtonHandler.cpp" for button callbacks
extern void developmentMapMode();
extern void defaultMapMode();
extern void opinionMapMode();

int main()
{
	//buttons
	void(*callBack)() = &developmentMapMode;
	g_buttonHandler.buttons.push_back(*new ButtonTemplate("Assets/DevelopmentMapModeButton.bmp", Vector2f(200, 10), callBack, 2));
	callBack = &defaultMapMode;
	g_buttonHandler.buttons.push_back(*new ButtonTemplate("Assets/DefaultMapModeButton.bmp", Vector2f(10, 10), callBack, 2));
	callBack = &opinionMapMode;
	g_buttonHandler.buttons.push_back(*new ButtonTemplate("Assets/OpinionMapModeButton.bmp", Vector2f(110, 10), callBack, 2));

	//initially draw stuff to screen.
	g_update();

	//For refreshing the screen periodically (currently 30fps)
	Clock screenRefreshClock;
	Time screenRefreshTime = seconds(0.0333);

	//For refreshing the nations periodically
	Clock nationUpdateClock;
	Time nationUpdateTime = seconds(0.5);

	g_textHandler.addText("0", Vector2f(430, 0));
	g_textHandler.staticText[0].setCharacterSize(35);

	g_textHandler.addText("Date: ", Vector2f(330, 0));
	g_textHandler.staticText[1].setCharacterSize(35);

	//initially pause the simulation and display some text to the user:
	g_isNationUpdate = false;
	cout << "Welcome to evelutionary nations! \n";
	cout << "Press space to play / pause the simulation \n";
	cout << "WASD to pan around, mouse wheel to zoom in/out \n";

	//main loop
	while (g_window.isOpen())
	{
		//event handling
		Event evnt;
		while (g_window.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed)
			{
				g_window.close();
				break;
			}

			//keyboard input
			if (g_inputHandler.getInput(&evnt))
			{
				//may need to fix this issue later on...
				//every time there is a new event, we want to redraw stuff to screen (makes screen it seem smoother)
				g_update();
			}
		}
		
		//updating screen periodically
		if (screenRefreshClock.getElapsedTime() >= screenRefreshTime)
		{
			screenRefreshClock.restart();
			g_update();
		}

		//updating nations periodically
		if (nationUpdateClock.getElapsedTime() >= nationUpdateTime && g_isNationUpdate)
		{
			g_updateNations();
			nationUpdateClock.restart();
		}
	}

	return 0;
}
