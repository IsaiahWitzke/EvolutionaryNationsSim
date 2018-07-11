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
	string nationNamesPath = "Assets/NationNames.txt";

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
					//finding the name of the nation in the "NationNames.txt" file (found in the assets folder)
					string newNationName = "";
					string lineInTxt;
					ifstream myfile(nationNamesPath);
					if (myfile.is_open())
					{
						//a flag that turns true once a valid nation name has been found
						bool foundNationName = false;
						while (getline(myfile, lineInTxt) && !foundNationName)
						{
							if (nations.size() != 0)
							{
								//making sure that there is no nation already named the nation name
								bool foundSimillarNationName = false;
								for (int i = 0; i < nations.size(); i++)
								{
									//if we found a simmilar nation name, then we want to get a new line from the txt and try again
									if (nations[i]->nationName == lineInTxt)
									{
										foundSimillarNationName = true;
									}
								}
								if (foundSimillarNationName)
									continue;
								else
								{
									newNationName = lineInTxt;
									foundNationName = true;
								}
									 
							}
							else
							{
								newNationName = lineInTxt;
								foundNationName = true;
							}
							
						}
						//if we havent found a nation name, give an error:
						if (!foundNationName)
						{
							cout << "Unable to find enough unique names in file " << nationNamesPath << " try adding some new nation names?" << endl;
							myfile.close();
							system("pause");
						}

						myfile.close();
					}
					else
					{
						cout << "Unable to open file " << nationNamesPath << endl;
						system("pause");
					}
					Nation *tempNation = new Nation(nationStartsMap.getPixel(x, y), newNationName);
					
					//adding the nation to the array of nations
					this->nations.push_back(tempNation);
					this->nations[nations.size()-1]->addContolledState(&states[x][y]);	// the last nation in the array will be the one we are trying to update
				}
			}
		}
	}

	//after we make the nations, we want to give them names

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