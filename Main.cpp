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


void testFunc()
{
	printf("hello\n");
}

int main()
{


	RenderWindow window(VideoMode(512, 512), "Evolutionary Nations");

	MapHandler map;
	InputHandler inputHandler(&window);

	void(*callBackFunc)() = &testFunc;

	ButtonTemplate testButton("Assets/TestButton.bmp", Vector2f(10, 10), callBackFunc, 5);

	//need to link buttons to the input handler
	inputHandler.buttons.push_back(testButton);

	Nation testNation(Color(255, 0, 0, 255));
	testNation.addContolledState(&map.states[0][0]);
	testNation.addContolledState(&map.states[1][0]);

	//initially draw stuff to screen.
	testButton.update(&window);
	map.updateStates(&window);
	window.display();

	//For refreshing the screen periodically (currently 30fps)
	Clock screenRefreshClock;
	Time screenRefreshTime = seconds(0.0333);

	//For refreshing the nations periodically
	Clock nationUpdateClock;
	Time nationUpdateTime = seconds(1.5);

	//main loop
	while (window.isOpen())
	{
		//event handling
		Event evnt;
		while (window.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed)
			{
				window.close();
				break;
			}

			//keyboard input
			if (inputHandler.getInput(&evnt))
			{
				//may need to fix this issue later on...
				//every time there is a new event, we want to redraw stuff to screen (makes screen it seem smoother)
				window.clear();
				map.updateStates(&window);
				testButton.update(&window);
				window.display();
			}
		}
		
		//updating screen periodically
		if (screenRefreshClock.getElapsedTime() >= screenRefreshTime)
		{
			screenRefreshClock.restart();
			window.clear();
			map.updateStates(&window);
			testButton.update(&window);
			window.display();
		}

		//updating nations periodically
		if (nationUpdateClock.getElapsedTime() >= nationUpdateTime)
		{
			nationUpdateClock.restart();
			testNation.update(&map);
		}
	}

	return 0;
}
