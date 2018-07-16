#pragma once
#include <sfml/Graphics.hpp>

using namespace sf;
using namespace std;

extern RenderWindow g_window;

class TextHandler
{
public:

	Clock clock;
	map<Time, Text> allText;

	Font font;
	Text tempText;

	void addText(string words, Vector2f position, Time lifetime);
	void update();

	TextHandler();
	~TextHandler();
};

