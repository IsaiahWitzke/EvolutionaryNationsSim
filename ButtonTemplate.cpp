#include "stdafx.h"
#include "ButtonTemplate.h"

extern float GLOBALSCALE;


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
}

ButtonTemplate::~ButtonTemplate()
{
}

void ButtonTemplate::update(RenderWindow * window)
{
	window->draw(this->sprite);
}

bool ButtonTemplate::isWithin(Vector2f possiblePoint)
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
