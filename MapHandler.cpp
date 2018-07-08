#include "stdafx.h"
#include "MapHandler.h"
#include "inputHandler.h"

extern float GLOBALSCALE;
extern RenderWindow g_window;
extern InputHandler g_inputHandler;

using namespace std;

MapHandler::MapHandler()
{
	//all the different .bmp file names
	string landMap = "Maps/LandMap.bmp";

	Image tempMap = readBMP(landMap);

	//for future reference. This is what the size of the map should stay at across all pictures
	//if it changes, we know there is a size discrepancy between the map images 


	this->width = tempMap.getSize().x;
	this->height = tempMap.getSize().y;

	int referenceWidth = width; 
	int referenceHeight = height;

	//making 2D vector of states
	for (int x = 0; x < width; x++)
	{
		//temp 1D array to feed into the 2D array later on
		vector<State> column;
		for (int y = 0; y < height; y++)
		{
			bool isWater;
			//black pixels are land, white is water
			if (tempMap.getPixel(x, y) == Color(255, 255, 255, 255))
				isWater = true;
			else
				isWater = false;

			column.push_back(State(isWater, Vector2f(x*GLOBALSCALE, y*GLOBALSCALE), Vector2i(x, y) ) );
		}
		this->states.push_back(column);
	}


	//after all states are done initializing, we want to add them to the array of buttons in the input handler
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			g_inputHandler.buttonHitBoxes.push_back(states[x][y].hitBox);
		}
	}
}

MapHandler::~MapHandler()
{
}

void MapHandler::updateStates()
{
	//goes through all the different states and call's their update functions
	for (int x = 0; x < states.size(); x++)
	{
		for (int y = 0; y < states[0].size(); y++)
		{
			states[x][y].update();
		}
	}

	//after all states are done updating, we want to add them to the array of buttons in the input handler
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			g_inputHandler.buttonHitBoxes.push_back(states[x][y].hitBox);
		}
	}
}



Image MapHandler::readBMP(string file)
{
	Image mapImage;

	if (!mapImage.loadFromFile(file))
	{
		cout << "error reading: " << file << endl;
		system("pause");
	}
	else
	{
		return mapImage;
	}

}