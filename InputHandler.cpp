#include "stdafx.h"
#include "InputHandler.h"

using namespace sf;
using namespace std;

extern float GLOBALSCALE;
extern Vector2f GLOBALOFFSET;
extern RenderWindow g_window;

InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

bool InputHandler::getInput(Event * event)
{
	const float PANSPEED = 7;
	const float SCROLLSPEED = 0.3;

	//panning around the map
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		GLOBALOFFSET.y += PANSPEED / GLOBALSCALE;
		return true;
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		GLOBALOFFSET.y -= PANSPEED / GLOBALSCALE;
		return true;
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		GLOBALOFFSET.x -= PANSPEED / GLOBALSCALE;
		return true;
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		GLOBALOFFSET.x += PANSPEED / GLOBALSCALE;
		return true;
	}

	//zooming in and out
	if (event->type == Event::MouseWheelScrolled)
	{
		if (event->mouseWheelScroll.wheel == Mouse::VerticalWheel)
		{
			if (event->mouseWheelScroll.delta == 1 && GLOBALSCALE < 16)
			{
				GLOBALSCALE += SCROLLSPEED;
				return true;
			}
			if(event->mouseWheelScroll.delta == -1 && GLOBALSCALE > 0.1)
			{
				GLOBALSCALE -= SCROLLSPEED;
				return true;
			}		
		}
	}

	//buttons
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		//go through all buttons, if the mouse location is within the button, do the button's callback function then leave
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i]->isWithinHitBox(Vector2f(Mouse::getPosition(g_window))))
			{
				buttons[i]->callBackFunction();
			}
		}
	}
	

	return false;
}
