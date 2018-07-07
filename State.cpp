#include "stdafx.h"
#include "State.h"
#include <iostream>

extern float GLOBALSCALE;
extern Vector2f GLOBALOFFSET;
extern RenderWindow g_window;
using namespace std;

Texture State::makeDefaultTexture()
{
	Texture texture;

	if (!texture.loadFromFile("pixel.bmp"))
	{
		cout << "Could not load default texture image" << endl;
		system("PAUSE");
	}

	this->size = texture.getSize();
	return texture;
}


bool State::isWithinHitBox(Vector2f possiblePoint)
{
	if (possiblePoint.x > position.x && possiblePoint.x < position.x + (size.x * sprite.getScale().x))
	{
		if (possiblePoint.y > position.y && possiblePoint.y < position.y + size.y * sprite.getScale().x)
		{
			return true;
		}
	}
	return false;
}

State::State(bool isWater, Vector2f position, Vector2i positionInMap)
{
	//sprite stuff
	sprite.setTexture(makeDefaultTexture());
	sprite.setScale(Vector2f(GLOBALSCALE, GLOBALSCALE));

	//some state data stuff
	this->positionInMap = positionInMap;
	this->isWater = isWater;

	//if there is water, we want the sprite to be blue, otherwise it is green
	if (isWater)
		this->color = Color(0, 0, 255, 255);
	else
		this->color = Color(0, 255, 0, 255);

	this->controller = NULL;
	this->position = position;
}

void State::update()
{
	//every nation has a different color, if there is no controlling nation, then make it a green default color
	if (controller != NULL)
	{
		this->color = controller->nationColor;
	}
	else
	{
		if (isWater)
			this->color = Color(0, 0, 255, 255);
		else
			this->color = Color(0, 255, 0, 255);
	}

	this->sprite.setColor(color);
	
	//updating position on screen stuff
	float newX = GLOBALSCALE*(position.x + GLOBALOFFSET.x) + g_window.getSize().x / 2;
	float newY = GLOBALSCALE*(position.y + GLOBALOFFSET.y) + g_window.getSize().y / 2;

	this->sprite.setPosition(Vector2f(newX, newY));
	sprite.setScale(Vector2f(GLOBALSCALE * 15, GLOBALSCALE * 15));
	g_window.draw(sprite);
}


State::~State()
{
}
