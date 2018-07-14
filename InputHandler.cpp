#include "stdafx.h"
#include "InputHandler.h"

using namespace sf;
using namespace std;

extern float GLOBALSCALE;
extern Vector2f GLOBALOFFSET;
extern RenderWindow g_window;
extern bool g_isNationUpdate;

bool InputHandler::isWithinHitbox(Vector2f possiblePoint, HitBox hitBox)
{
	if (possiblePoint.x > hitBox.topLeft.x && possiblePoint.x < hitBox.bottomRight.x)
	{
		if (possiblePoint.y > hitBox.topLeft.y && possiblePoint.y < hitBox.bottomRight.y)
		{
			return true;
		}
	}
	return false;
}

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

	if (event->type == Event::KeyPressed)
	{
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
		//pausing/unpausing the simulation
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			g_isNationUpdate = !g_isNationUpdate;
			return true;
		}
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
		//so that we dont press a state underneath a button
		bool isButtonPressed = false;

		//go through all buttons' hit boxes, if the mouse location is within the button, do the button's callback function then leave
		//backwards because the items further down the list are rendered on the screen first
		for (int i = buttons.size()-1; i >= 0 ; i--)
		{
			if (isWithinHitbox(Vector2f(Mouse::getPosition(g_window)), buttons[i].hitBox))
			{
				buttons[i].callBackFunction();
				isButtonPressed = true;
				break;	//so we dont trigger multiple hitboxes with one click
			}
		}

		if (!isButtonPressed)
		{
			//same thing but with states
			for (int i = states.size() - 1; i >= 0; i--)
			{
				if (isWithinHitbox(Vector2f(Mouse::getPosition(g_window)), states[i].hitBox))
				{
					states[i].printInfo();
					break;	//so we dont trigger multiple hitboxes with one click
				}
			}
		}
	}
	

	return false;
}
