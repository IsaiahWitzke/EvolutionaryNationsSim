#include "stdafx.h"
#include "ButtonTemplate.h"
#include "InputHandler.h"

extern float GLOBALSCALE;
extern RenderWindow g_window;
extern InputHandler g_inputHandler;

Texture ButtonTemplate::readBMP(string file)
{
	Texture mapTexture;
	
	if (!mapTexture.loadFromFile(file))
	{
		cout << "error reading: " << file << endl;
		system("pause");
	}
	else
	{
		this->size = mapTexture.getSize();
		return mapTexture;
	}
}

ButtonTemplate::ButtonTemplate(string picturePath, Vector2f position, void (*callBackFunction)(), float scale )
{
	//sprite stuff
	this->texture = readBMP(picturePath);
	this->position = position;
	this->sprite.setTexture(texture);
	this->sprite.setScale(Vector2f(scale, scale));
	this->sprite.setPosition(position);

	//call back function
	this->callBackFunction = callBackFunction;

	//setting up the hit box
	float bottomLeftX = sprite.getPosition().x + (sprite.getScale().x*size.x);
	float bottomLeftY = sprite.getPosition().y + (sprite.getScale().y*size.y);
	HitBox newHitBox(sprite.getPosition(), Vector2f(bottomLeftX, bottomLeftY));
	this->hitBox = newHitBox;
	g_inputHandler.buttons.push_back(*this);
}

ButtonTemplate::~ButtonTemplate()
{
}

void ButtonTemplate::update()
{
	//updating the hit box (probably doesn't need to be done for stationary buttons, but here for future ui improvements, ie: moving buttons?)
	float bottomLeftX = sprite.getPosition().x + (sprite.getScale().x*size.x);
	float bottomLeftY = sprite.getPosition().y + (sprite.getScale().y*size.y);
	HitBox newHitBox(sprite.getPosition(), Vector2f(bottomLeftX, bottomLeftY));
	this->hitBox = newHitBox;
	g_inputHandler.buttons.push_back(*this);
	g_window.draw(this->sprite);
}
