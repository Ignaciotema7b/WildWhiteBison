#include "..\include\Paylines.h"
#include <fstream> 

Paylines::Paylines()
{
}


Paylines::~Paylines()
{
}

void Paylines::load_paylines(int n_reels, string config_folder)
{
	//Read .csv file
	ifstream file(config_folder + "\\paylines.csv");

	if (!file.good())
	{
		cout << "Warning: paylines.csv not found...\n";
		return;
	}

	string str;

	while (!file.eof())
	{
		vector<int> line;

		for (int r = 0; r < n_reels; r++)
		{
			getline(file, str, ',');
			if (str == "\n") break;
			line.push_back(stoi(str));
		}

		if (line.size() == n_reels)
			lines.push_back(line);
		getline(file, str, '\n');
	}

	file.close();
}
