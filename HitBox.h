#pragma once

#include <SFML\Graphics.hpp>

using namespace std;
using namespace sf;


// a class that contains 2 points that represent the bounds of a hit box
class HitBox
{
public:
	//the two points of the hit box
	Vector2f topLeft;
	Vector2f bottomRight;

	//a callback function for when the button is hit
	void(*callBackFunction)();

	HitBox();	//does nothing
	HitBox(Vector2f topLeft, Vector2f bottomRight, void (*callBackFunction)());
	~HitBox();
};

