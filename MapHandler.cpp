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
	string landMapPath = "Maps/LandMap.bmp";
	string nationStartsPath = "Maps/NationStarts.bmp";

	Image landMap = readBMP(landMapPath);

	//for future reference. This is what the size of the map should stay at across all pictures
	//if it changes, we know there is a size discrepancy between the map images 
	this->width = landMap.getSize().x;
	this->height = landMap.getSize().y;

	//making 2D vector of states
	for (int x = 0; x < width; x++)
	{
		//temp 1D array to feed into the 2D array later on
		vector<State> column;
		for (int y = 0; y < height; y++)
		{
			bool isWater;
			//black pixels are land, white is water
			if (landMap.getPixel(x, y) == Color(255, 255, 255, 255))
				isWater = true;
			else
				isWater = false;

			column.push_back(State(isWater, Vector2f(x*GLOBALSCALE, y*GLOBALSCALE), Vector2i(x, y) ) );
		}
		this->states.push_back(column);
	}

	//initializing the nations
	Image nationStartsMap = readBMP(nationStartsPath);
	
	//first, check if the sizes of the pictutres are the same, if they arent, error
	if (width != nationStartsMap.getSize().x || height != nationStartsMap.getSize().y)
	{
		cout << "Map images are not the same size" << endl;
		system("pause");
	}

	//going through all the pixels
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (states[1][1].controller != NULL)
			{
				cout << x << "," << y << " " << int(states[1][1].controller->nationColor.g) << endl;
			}
			else
			{
				cout << "null" << endl;
			}
			
			//any pixel that is not white is a start state of a nation
			if (nationStartsMap.getPixel(x, y) != Color(255, 255, 255, 255))
			{
				//before we make a new nation, we need to make sure that the color isn't the same as a nation already made
				bool isNewNation = true;

				for (int i = 0; i < nations.size(); i++)
				{
					if (nations[i]->nationColor == nationStartsMap.getPixel(x, y))
					{
						isNewNation = false;
						//add to the nation of the same color
						nations[i]->addContolledState(&states[x][y]);
					}
				}

				//initializing a new nation with the state at the current x,y position
				if (isNewNation)
				{
					if (x == 2 && y == 15)
					{
						cout << "" << endl;
					}

					Nation *tempNation = new Nation(nationStartsMap.getPixel(x, y));

					this->nations.push_back(tempNation);

					this->nations[nations.size()-1]->addContolledState(&states[x][y]);	// the last nation in the array will be the one we are trying to update

					cout << "" << endl;

				}
			}
		}
	}

	cout << int(states[1][1].controller->nationColor.g) << endl;

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