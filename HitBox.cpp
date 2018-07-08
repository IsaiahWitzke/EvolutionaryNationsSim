#include "stdafx.h"
#include "HitBox.h"
#include <iostream>

using namespace std;

HitBox::HitBox()
{
	//cout << "default hit box constructor" << endl;
}

HitBox::HitBox(Vector2f topLeft, Vector2f bottomRight, void(*callBackFunction)())
{
	//cout << "NOT default hit box constructor" << endl;
	this->callBackFunction = callBackFunction;
	this->topLeft = topLeft;
	this->bottomRight = bottomRight;
}

HitBox::~HitBox()
{
}
