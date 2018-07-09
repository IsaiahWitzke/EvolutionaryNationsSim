#pragma once

#include <SFML\Graphics.hpp>
#include <functional>

using namespace std;
using namespace sf;


// a class that contains 2 points that represent the bounds of a hit box
class HitBox
{
public:
	//the two points of the hit box
	Vector2f topLeft;
	Vector2f bottomRight;

	HitBox();	//does nothing
	HitBox(Vector2f topLeft, Vector2f bottomRight);
	~HitBox();
};

