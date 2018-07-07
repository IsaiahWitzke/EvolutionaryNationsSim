#include "stdafx.h"
#include "ButtonTemplate.h"

extern float GLOBALSCALE;
extern RenderWindow g_window;

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

void ButtonTemplate::update()
{
	g_window.draw(this->sprite);
}

bool ButtonTemplate::isWithinHitBox(Vector2f mousePos)
{
	if (mousePos.x > position.x && mousePos.x < position.x + (size.x * sprite.getScale().x))
	{
		if (mousePos.y > position.y && mousePos.y < position.y + size.y * sprite.getScale().x)
		{
			return true;
		}
	}
	return false;
}
