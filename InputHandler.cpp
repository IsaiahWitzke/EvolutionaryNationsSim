#include "stdafx.h"
#include "InputHandler.h"

using namespace sf;
using namespace std;

extern float GLOBALSCALE;
extern Vector2f GLOBALOFFSET;
extern RenderWindow g_window;

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
		//go through all buttons' hit boxes, if the mouse location is within the button, do the button's callback function then leave
		for (int i = 0; i < buttonHitBoxes.size(); i++)
		{
			if (isWithinHitbox(Vector2f(Mouse::getPosition(g_window)), buttonHitBoxes[i]))
			{
				buttonHitBoxes[i].callBackFunction();
			}
		}
	}
	

	return false;
}
