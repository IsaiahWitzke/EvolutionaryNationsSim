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

//will become the callback function
//NOTE: this is a global function at the moment, I didn't know how to make a pointer to a function in a scope without the scope messing with it
void State::printInfo()
{
	cout << "State info: " << endl;
	cout << "Is it water? " << this->isWater << endl;
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

	//setting up the hit box
	//callback function
	this->callBackFunction = &printInfo;
	float bottomLeftX = sprite.getPosition().x + (sprite.getScale().x*size.x);
	float bottomLeftY = sprite.getPosition().y + (sprite.getScale().y*size.y);
	HitBox newHitBox(sprite.getPosition(), Vector2f(bottomLeftX, bottomLeftY), callBackFunction);
	this->hitBox = newHitBox;
	g_inputHandler.buttonHitBoxes.push_back(hitBox);
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
	HitBox newHitBox(sprite.getPosition(), Vector2f(bottomLeftX, bottomLeftY), callBackFunction);
	this->hitBox = newHitBox;
	g_inputHandler.buttonHitBoxes.push_back(hitBox);
}


State::~State()
{
}
