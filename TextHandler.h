#pragma once
#include <sfml/Graphics.hpp>
#include "Nation.h"

using namespace sf;
using namespace std;

extern RenderWindow g_window;

class TextHandler
{
public:
	//static text
	vector<Text> staticText;

	//text that updates position every frame:
	vector<Text> dynamicText;

	Font font;

	void addText(string words, Vector2f position);
	void update();

	TextHandler();
	~TextHandler();
};

