#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

class SettingInput
{
public:
	SettingInput(string settingsPath);
	~SettingInput();

	int defaultStability = 1;
	float incomePerDevelopment = 0.5;
	float stateMaintenance = 0.1;
	float armyMaintenance = 0.1;

};

