#include "stdafx.h"
#include "SettingInput.h"


SettingInput::SettingInput(string settingsPath)
{
	string lineInTxt;
	ifstream myfile(settingsPath);
	if (myfile.is_open())
	{
		//a little parser
		while (getline(myfile, lineInTxt))
		{
			//hashes (coments) and empty lines
			if (lineInTxt.size() == 0)
				continue;
			if (lineInTxt[0] == '#')
				continue;

			istringstream iss(lineInTxt);

			vector<string> tokens{ istream_iterator<string>{iss},
				istream_iterator<string>{} };

			if (tokens[0] == "DefaultStability:") { this->defaultStability = stoi(tokens[1]); }

			if (tokens[0] == "IncomePerDevelopment:") { this->incomePerDevelopment = stof(tokens[1]); }

			if (tokens[0] == "StateMaintenance:") { this->stateMaintenance = stof(tokens[1]); }
			
			if (tokens[0] == "armyMaintenance:") { this->armyMaintenance = stof(tokens[1]); }
		}

		myfile.close();
	}
	else
	{
		cout << "Unable to open file " << settingsPath << endl;
		system("pause");
	}
}


SettingInput::~SettingInput()
{
}
