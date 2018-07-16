#include "stdafx.h"
#include "TextHandler.h"
#include "MapHandler.h"
#include <iostream>

extern MapHandler g_map;

void TextHandler::addText(string words, Vector2f position, Time lifetime)
{
	// Declare and load a font
	sf::Font font;
	if (!font.loadFromFile("Assets/Silkscreen/slkscr.ttf"))
	{
		std::cout << "error reading text " << "Assets/Silkscreen/slkscr.ttf" << std::endl;
		system("pause");
	}

	this->font = font;

	// Create a text
	sf::Text text(words, this->font);
	text.setCharacterSize(15);
	text.setStyle(sf::Text::Regular);
	text.setFillColor(sf::Color::White);
	text.setPosition(position);

	//add it to the map of texts
	this->allText.emplace(clock.getElapsedTime() + lifetime, text);
	this->tempText = text;
}

void TextHandler::update()
{
	//a vector of text that will be filled with text that needs to die/be erased from the "allText" map
	vector<Text> textToDie;

	for (const auto& sm_pair : allText)
	{
		//first check if it is time for the buttons to die:
		
		if (sm_pair.first.asSeconds() >= (clock.getElapsedTime()).asSeconds())
		{
			textToDie.push_back(sm_pair.second);
		}
		
		//draw the text
		g_window.draw(sm_pair.second);
	}

	
	//erasing all the text that is too old
	for (int i = 0; i < textToDie.size(); i++)
	{
		
		//allText.erase(textToDie[i]);
	}
	
}

TextHandler::TextHandler()
{

}


TextHandler::~TextHandler()
{
}
