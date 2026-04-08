#pragma once
#include <random>
#include <string>
#include <iostream>

using namespace std;

class Paylines
{
public:
	Paylines();
	~Paylines();
	vector<vector<int>> lines;

	//Load paylines from file "paylines.csv" in folder "config"
	void load_paylines(int n_reels, string config_folder);
};
