#include "stdafx.h"
#include "State.h"
#include "InputHandler.h"
#include <iostream>

extern float GLOBALSCALE;
extern Vector2f GLOBALOFFSET;
extern RenderWindow g_window;
extern InputHandler g_inputHandler;
extern MapHandler g_map;

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
	system("CLS");

	cout << "State info:" << endl;
	cout << "Development: " << development << endl;

	//as long as the state thet we are pressing has a controller, we want to print that nation's info
	if (this->controller != NULL)
	{
		cout << endl;
		this->controller->printInfo();
	}
}

State::State(bool isWater, Vector2f position, Vector2i positionInMap, int development)
{
	//sprite stuff
	sprite.setTexture(makeDefaultTexture());
	sprite.setScale(Vector2f(GLOBALSCALE, GLOBALSCALE));

	//some state data stuff
	this->positionInMap = positionInMap;
	this->isWater = isWater;
	this->development = development;

	//if there is water, we want the sprite to be blue, otherwise it is green
	if (isWater)
	{
		this->color = Color(0, 0, 255, 255);
		this->development = 0;	// also the development of a water province is 0
	}
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
	//the colors of the map
	switch (g_map.mapMode)
	{
	case MapMode::default:
		//every nation has a different color, if there is no controlling nation, then make it a green land or a blue water
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
		break;

	case MapMode::development:
		this->color = Color(0, development, 0, 255);
		break;

	case MapMode::opinion:
		//water
		if (isWater)
		{
			this->color = Color(0, 0, 255, 255);
			break;
		}
			
		//if no state is selected:
		if (g_inputHandler.selectedNation == NULL)
		{
			this->color = Color(128, 128, 128, 255);
			break;
		}

		//states with no controller
		if (controller == NULL)
		{
			this->color = Color(48, 48, 48, 255);
			break;
		}

		//controler is the selected nation:
		if (controller == g_inputHandler.selectedNation)
		{
			this->color = Color(20, 40, 200, 255);	// blue
			break;
		}

		//otherwise... actual opinion stuff of this state's controller
		switch (g_inputHandler.selectedNation->diplomaticViews[controller])
		{
		case DiplomaticView::close:
			this->color = Color(0, 255, 0, 255);	// very green
			break;
		case DiplomaticView::friendly:
			this->color = Color(93, 255, 0, 255);	// greenish
			break;
		case DiplomaticView::warm:
			this->color = Color(180, 255, 0, 255);	// yellow green
			break;
		case DiplomaticView::neutral:
			this->color = Color(255, 255, 0, 255);	// yellow
			break;
		case DiplomaticView::uneasy:
			this->color = Color(255, 128, 0, 255);	// orange
			break;
		case DiplomaticView::hostile:
			this->color = Color(255, 0, 0, 255);	// red
			break;

		default:
			break;
		}

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
