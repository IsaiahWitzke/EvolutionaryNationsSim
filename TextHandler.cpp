#include "stdafx.h"
#include "TextHandler.h"
#include "MapHandler.h"
#include <iostream>

extern float GLOBALSCALE;
extern MapHandler g_map;

void TextHandler::addText(string words, Vector2f position)
{
	// Create a text
	sf::Text text(words, this->font);
	text.setCharacterSize(7);
	text.setStyle(sf::Text::Regular);
	text.setFillColor(sf::Color::White);
	text.setPosition(position);

	//add it to the vector
	this->staticText.push_back(text);
}

void TextHandler::update()
{
	//to display nation names
	dynamicText.clear();
	
	for (int i = 0; i < g_map.nations.size(); i++)
	{
		//for nations that dont have any states, but havent been removed quite yet
		if (g_map.nations[i]->controlledStates.size() == 0)
		{
			continue;
		}
		
		sf::Text nationNameText(g_map.nations[i]->nationName, this->font);
		nationNameText.setCharacterSize(10 * GLOBALSCALE);
		nationNameText.setStyle(sf::Text::Regular);
		nationNameText.setFillColor(sf::Color::White);
		nationNameText.setOutlineColor(sf::Color::Black);
		nationNameText.setOutlineThickness(GLOBALSCALE);
		nationNameText.setPosition(g_map.nations[i]->controlledStates[0]->sprite.getPosition());
		this->dynamicText.push_back(nationNameText);
	}

	for (int i = 0; i < staticText.size(); i++)
	{
		//draw the permanate text
		g_window.draw(staticText[i]);
	}

	for (int i = 0; i < dynamicText.size(); i++)
	{
		//draw the dynamic text
		g_window.draw(dynamicText[i]);
	}
}

TextHandler::TextHandler()
{
	// Declare and load a font
	sf::Font font;
	if (!font.loadFromFile("Assets/Silkscreen/slkscr.ttf"))
	{
		std::cout << "error reading text " << "Assets/Silkscreen/slkscr.ttf" << std::endl;
		system("pause");
	}

	this->font = font;
}


TextHandler::~TextHandler()
{
}
