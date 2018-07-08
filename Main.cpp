// EvolutionaryNationsSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "State.h"
#include "MapHandler.h"
#include "InputHandler.h"
#include "ButtonTemplate.h"
#include <iostream>
#include <sfml/Graphics.hpp>

using namespace std;
using namespace sf;

float GLOBALSCALE = 5;
Vector2f GLOBALOFFSET = Vector2f(0, 0);

RenderWindow g_window(VideoMode(512, 512), "Evolutionary Nations");

//the entire collection of states the entire simulation is based off of
MapHandler g_map;

//all input is handled here
InputHandler g_inputHandler;

void testFunc()
{
	printf("hello\n");
}

int main()
{
	void(*callBackFunc)() = &testFunc;

	ButtonTemplate testButton("Assets/TestButton.bmp", Vector2f(10, 10), callBackFunc, 5);

	//need to link buttons to the input handler
	g_inputHandler.buttonHitBoxes.push_back(testButton.hitBox);

	Nation testNation(Color(255, 0, 0, 255));
	testNation.addContolledState(&g_map.states[0][0]);
	testNation.addContolledState(&g_map.states[1][0]);

	//initially draw stuff to screen.
	testButton.update();
	g_map.updateStates();
	g_window.display();

	//For refreshing the screen periodically (currently 30fps)
	Clock screenRefreshClock;
	Time screenRefreshTime = seconds(0.0333);

	//For refreshing the nations periodically
	Clock nationUpdateClock;
	Time nationUpdateTime = seconds(1.5);

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
				g_window.clear();
				g_inputHandler.buttonHitBoxes.empty();	//want to get rid of all buttons because the update functions add them back in
				g_map.updateStates();
				testButton.update();
				g_window.display();
			}
		}
		
		//updating screen periodically
		if (screenRefreshClock.getElapsedTime() >= screenRefreshTime)
		{
			screenRefreshClock.restart();
			g_window.clear();
			g_inputHandler.buttonHitBoxes.clear();	//want to get rid of all buttons because the update functions add them back in
			g_map.updateStates();
			testButton.update();
			g_window.display();
		}

		//updating nations periodically
		if (nationUpdateClock.getElapsedTime() >= nationUpdateTime)
		{
			nationUpdateClock.restart();
			testNation.update();
		}
	}

	return 0;
}
