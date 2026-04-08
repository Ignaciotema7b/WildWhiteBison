#include "..\include\Slingo.h"

void Slingo::init( unsigned n_reels_param,unsigned n_rows_param, unsigned stake, unsigned spins_base, unsigned extra_spins, string config_folder)
{
	//Initialize RNG and stake
	Game::init(stake);

	n_reels = n_reels_param;
	n_rows = n_rows_param;

	load_symbols(config_folder);

	//Joker, Super Joker and Free spin symbols:
	for (int s = 0; s < (int)symbols_list.size(); s++)
	{
		if (symbols_list[s].symbol_name == "Joker")
			joker_id = symbols_list[s].symbol_ID;
		if (symbols_list[s].symbol_name == "Super Joker")
			super_joker_id = symbols_list[s].symbol_ID;
		if (symbols_list[s].symbol_name == "Free Spin")
			free_spin_id = symbols_list[s].symbol_ID;
	}

	//Init screen size
	for (unsigned row = 0; row < n_rows_param; row++)
	{
		vector<int> reels(n_reels_param, -1);
		screen.push_back(reels);
		card_numbers.push_back(reels);
	}

	spins_standard = spins_base;
	spins_purchase = extra_spins;

	//Initialize spin_results, joker and super_jokers counters' size
	for (unsigned reel = 0; reel < n_reels_param; reel++)
	{
		spin_results.push_back(0);
		jokers.push_back(false);
		super_jokers.push_back(false);
	}

	//Read reel sets
	reel_set_base.init(n_reels_param, config_folder + "\\slingo_reel_set_base.csv" );
	reel_set_purchase.init(n_reels_param, config_folder + "\\slingo_reel_set_purchase.csv");

}

void Slingo::init(Game_info info)
{
	init(info.n_reels, info.number_of_rows, info.bet, info.spins_base, info.spins_purchase, info.folder_config);
}

void Slingo::play(unsigned long long plays)
{
}

void Slingo::spin_reels(Reel_set& reel_set)
{
	reel_set.spin(rng);

	for (int i = 0; i < (int)reel_set.stops.size(); i++)
		spin_results[i] = reel_set.reel_strips[i][reel_set.stops[i]];

}

void Slingo::optimal_strategy()
{

}

void Slingo::reset_screen()
{
	for (int i = 0; i < n_reels; i++)
	{
		for (int j = 0; j < n_rows; j++)
		{
			screen[i][j] = 0;
		}
	}
}

void Slingo::load_symbols(string config_folder)
{
	//Read .csv file
	ifstream file(config_folder + "\\symbols.csv");

	if (!file.good())
	{
		cout << "Warning: symbols.csv not found...\n";
		return;
	}

	unsigned id = 0;

	string str;
	getline(file, str, '\n');


	while (!file.eof())
	{
		Symbol s;

		getline(file, s.symbol_name, ',');
		getline(file, str, ',');

		if ((int)str.size() > 3)
		{
			printf("Warning: length of short name for symbol must be 3 at most\n");
			system("PAUSE");
		}

		//Init variable
		for (int i = 0; i < 3; i++)
			s.short_name[i] = '\0';

		for (int i = 0; i < (int)str.size(); i++)
			s.short_name[i] = str[i];

		getline(file, str, ',');

		//ID is the same as index in the Symbols list
		s.symbol_ID = id;

		if (s.symbol_name != "\n")
		{
			//Check if IDs are in increasing order
			if (id != stoi(str))
			{
				printf("Warning: symbol IDs must be increasing...\n");
				system("PAUSE");
			}

			this->symbols_list.push_back(s);
			id++;
		}

		getline(file, str, '\n');

	}

	file.close();
}
