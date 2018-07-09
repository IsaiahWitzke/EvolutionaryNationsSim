#pragma once
#include <string>
#include <iostream>
#include "HitBox.h"
#include "sfml/Graphics.hpp"
#include <functional>

using namespace std;
using namespace sf;

class ButtonTemplate
{
private:
	Texture readBMP(string file);
	
	//sprite stuff
	Texture texture;
	Sprite sprite;
	Vector2f position;
	Vector2u size;

public:
	ButtonTemplate(string picturePath, Vector2f position, void(*callBackFunction)(), float scale);
	~ButtonTemplate();

	//draws button
	void update();

	//the function we want the button to do when pressed:
	function<void()> callBackFunction;

	HitBox hitBox;
};