#include "stdafx.h"
#include "State.h"
#include "InputHandler.h"
#include <iostream>

extern float GLOBALSCALE;
extern Vector2f GLOBALOFFSET;
extern RenderWindow g_window;
extern InputHandler g_inputHandler;
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

//when pressed. this fires
void State::printInfo()
{
	cout << endl << "=========================" << endl;

	cout << "State info:" << endl;

	//as long as the state thet we are pressing has a controller, we want to print that nation's info
	if (this->controller != NULL)
	{
		cout << endl;
		this->controller->printInfo();
	}

	cout << "=========================" << endl;
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
	this->position.x = position.x * 3;
	this->position.y = position.y * 3;

	//hitbox setup
	float bottomLeftX = sprite.getPosition().x + (sprite.getScale().x*size.x);
	float bottomLeftY = sprite.getPosition().y + (sprite.getScale().y*size.y);
	HitBox newHitBox(sprite.getPosition(), Vector2f(bottomLeftX, bottomLeftY));
	this->hitBox = newHitBox;
	g_inputHandler.states.push_back(*this);
}

void State::update()
{
	//every nation has a different color, if there is no controlling nation, then make it a green default color or a blue water
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

	//updating the hit box
	float bottomLeftX = sprite.getPosition().x + (sprite.getScale().x*size.x);
	float bottomLeftY = sprite.getPosition().y + (sprite.getScale().y*size.y);
	HitBox newHitBox(sprite.getPosition(), Vector2f(bottomLeftX, bottomLeftY));
	this->hitBox = newHitBox;
	g_inputHandler.states.push_back(*this);
}


State::~State()
{
}
