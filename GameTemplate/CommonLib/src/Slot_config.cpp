#include "..\include\Slot_config.h"



void Slot_config::init(Game_info info)
{
	init(info.n_reels, info.number_of_trackers, info.number_of_rows, info.folder_config);

	if (info.type == "Scatter pays" || info.type == "Clusters " || info.type == "Clusters (8 way)")
		load_paytable_scatter_pays(info);

	type = info.type;
}

void Slot_config::init(unsigned n_reels_param, unsigned n_trackers_param, unsigned n_rows_param, string config_folder)
{
	n_trackers = n_trackers_param;

	init(n_reels_param, n_rows_param, config_folder);

	//Loads reel strips with short names
	for (int reelset = 0; reelset < (int)reel_sets.size(); reelset++)
	{
		for (int reel = 0; reel < n_reels; reel++)
		{
			vector<string> aux;
			string symbol_str;
			int symbol_id;
			for (int pos = 0; pos < (int)reel_sets[reelset].reels_length[reel]; pos++)
			{
				symbol_id = reel_sets[reelset].reel_strips[reel][pos];
				symbol_str = symbols_list[symbol_id].short_name;

				aux.push_back(symbol_str);
			}

			reel_sets[reelset].reel_strips_short_name.push_back(aux);
		}

		for (int tracker = 0; tracker < n_trackers; tracker++)
		{
			vector<string> aux;
			string symbol_str;
			int symbol_id;
			for (int pos = 0; pos < (int)reel_sets[reelset].trackers_length[tracker]; pos++)
			{
				symbol_id = reel_sets[reelset].trackers_strips[tracker][pos];
				symbol_str = symbols_list[symbol_id].short_name;

				aux.push_back(symbol_str);
			}

			reel_sets[reelset].reel_strips_short_name.push_back(aux);
		}


	}

	//Convert config to json:

	//Symbols

	//Reel sets
	for (int i = 0; i < (int) reel_sets.size(); i++)
	{
		for (int reel = 0; reel < (int) reel_sets[i].reel_strips_short_name.size(); reel++)
			json_config["reelsets"]["reel_set_" + to_string(i)]["r" + to_string(reel)] = reel_sets[i].reel_strips_short_name[reel];
	}

	//Paytable
	json_config["paytable"] = paytable;

	//Paylines
	json_config["paylines"] = paylines.lines;
}

void Slot_config::init(unsigned n_reels_param, unsigned n_rows_param, string config_folder)
{
	n_reels = n_reels_param;
	n_rows = n_rows_param;

	load_symbols(config_folder);
	load_reel_sets(reel_sets, config_folder);

	count_symbols_distr();

	load_paytable(config_folder);

	load_prizes_list(config_folder);

	load_wild_substitutions();

	paylines.load_paylines(n_reels, config_folder);
}

void Slot_config::print_dist_symbols()
{
	for (int reel_set = 0; reel_set < (int)reel_sets.size(); reel_set++)
	{

		printf("************************\n");
		printf("        Reel Set: %d    \n" , reel_set);
		printf("************************\n");

		for (int symbol = 0; symbol < (int)symbols_list.size(); symbol++)
		{
			printf("%c%c%c\t|", symbols_list[symbol].short_name[0], symbols_list[symbol].short_name[1], symbols_list[symbol].short_name[2]);

			for (unsigned reel = 0; reel < n_reels; reel++)
				printf("\t%d", reel_sets[reel_set].calculate_min_dist(symbols_list[symbol].symbol_ID, reel));

			printf("\n");
		}

		printf("\n\n");
	}

}

void Slot_config::load_symbols( string config_folder)
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
		for (int i = 0; i < 4; i++)
			s.short_name[i] = '\0';

		for(int i=0; i < (int) str.size(); i++)
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

	//TODO: cargar sustitucion de simbolos wild
	/*size_t number_of_symbols = symbols_list.size();
	for (int s = 0; s < (int) number_of_symbols; s++)
	{
		symbols_list[s].subst.reserve(number_of_symbols);

		for (int t = 0; t < number_of_symbols; t++)
			{
				symbols_list[s].subst[t] = false;
			}

	}*/

}

void Slot_config::load_wild_substitutions()
{
	//Initialize:
	for (int s = 0; s < MAX_N_SYMBOLS; s++)
	{
		for (int t = 0; t < MAX_N_SYMBOLS; t++)
			substitutes_for[s][t] = false;
	}

	//Go through prizes list to load wild substitutions into substitutes_for array.
	for (unsigned i = 0; i < (unsigned)prizes_list.size(); i++)
	{
		for (unsigned j = 0; j < (unsigned)prizes_list[i].wildSubst.size(); j++)
		{
			substitutes_for[prizes_list[i].wildSubst[j]][prizes_list[i].symbol_ID] = true;

			//Add wild symbols IDs to the list:
			if (std::find(wild_symbols.begin(), wild_symbols.end(), prizes_list[i].wildSubst[j]) == wild_symbols.end())
				wild_symbols.push_back(prizes_list[i].wildSubst[j]);
		}
	}
}

void Slot_config::load_reel_sets(vector<Reel_set> &reel_sets,  string config_folder)
{
	//Count number of reel sets
	int n_reel_sets = 0;
	bool file_exists;

	std::ifstream infile(config_folder + "\\reel_set__"  + to_string(n_reel_sets) + ".csv");

	file_exists = infile.good();

	//Veo si existe el archivo
	while (file_exists)
	{
		n_reel_sets++;
		std::ifstream infile(config_folder + "\\reel_set__" + to_string(n_reel_sets) + ".csv");
		file_exists = infile.good();
	}
	
	total_number_base_reel_set = n_reel_sets;

	//Iterate to load all reel sets
	for (int n = 0; n < (int)n_reel_sets; n++)
	{
		Reel_set new_reel_set;
		new_reel_set.init(n_reels,n_trackers, config_folder + "\\reel_set__"  + to_string(n) + ".csv");

		new_reel_set.name = "reel_set__" + to_string(n);

		reel_sets.push_back(new_reel_set);
	}

}

void Slot_config::load_paytable( string config_folder)
{
	//Read .csv file
	ifstream file(config_folder + "\\paytable.csv");
	string str;

	//First row are table headers
	getline(file, str, '\n'); 

	while (!file.eof())
	{
		vector<unsigned> payouts(n_reels + 1);

		for (unsigned i = 0; i < n_reels + 1; i++)
		{
			//Get symbol char
			getline(file, str, ',');
			if (str == "\n" || str == "")
				break;
			payouts[i] = (unsigned) stoi(str);
		}

		paytable.push_back(payouts);
		getline(file, str, '\n');
	}

	file.close();

}

void Slot_config::load_paytable_scatter_pays(Game_info info)
{
	paytable.~vector();
	paytable.resize(symbols_list.size() + 1);


	//Read .csv file
	ifstream file(info.folder_config + "\\paytable.csv");
	string str;

	//First row are table headers
	getline(file, str, '\n');
	int i = 0;

	while (!file.eof())
	{
		vector<unsigned> payouts(n_reels * n_rows + 1);

		for (unsigned i = 0; i < n_reels * n_rows + 1; i++)
		{
			//Get symbol char
			getline(file, str, ',');
			if (str == "\n" || str == "")
				break;
			payouts[i] = (unsigned)stoi(str);
		}

		//paytable.push_back(payouts);
		paytable[i] = payouts;
		i++;
		getline(file, str, '\n');
	}

	file.close();
}

void Slot_config::count_symbols_distr()
{
	int n_symbols = (int)symbols_list.size();

	for (int reel_set = 0; reel_set < (int) total_number_base_reel_set; reel_set++)
	{	
		reel_sets[reel_set].count_symbols_by_reel(n_symbols);
	}
}

void Slot_config::load_prizes_list(string config_folder)
{
	//Read .csv file
	ifstream file(config_folder + "\\prizes.csv");
	string str;

	if (!file.good())
	{
		cout << "Warning: prizes.csv not found...\n";
		return;
	}


	unsigned id = 0;
	//Count number of columns of prizes
	int n_prizes_cols = 0;

	getline(file, str, ','); //

	while (str != "isScatter")
	{
		n_prizes_cols++;
		getline(file, str, ','); //
	}


	getline(file, str, '\n'); //First row are table headers

	while (!file.eof())
	{
		vector<Prize> prize(n_prizes_cols);

		//Get symbol char
		getline(file, str, ',');
		if (str == "\n")
			break;


		for (int i = 0; i < n_prizes_cols; i++)
			prize[i].symbol_ID = id;


		//Get payouts and set initial values
		for (int i = 0; i < n_prizes_cols; i++)
		{
			prize[i].payout = stoi(str);
			prize[i].xN = i; 
			getline(file, str, ',');

		}


		for (int i = 0; i < n_prizes_cols; i++)
			prize[i].isScatter = (str == "true");

		//wildSubst
		getline(file, str, '{');
		getline(file, str, '}');

		if (str != "")
		{
			string delimiter = ",";

			size_t pos = 0;
			string token;
			while ((pos = str.find(delimiter)) != std::string::npos) {
				token = str.substr(0, pos);

				for (int i = 0; i < n_prizes_cols; i++)
				{
					prize[i].wildSubst.push_back(stoul(token));
				}

				//std::cout << token << std::endl;
				str.erase(0, pos + delimiter.length());
			}


		}

		for (int i = 0; i < n_prizes_cols; i++)
		{
			if (prize[i].payout != 0)
				prizes_list.push_back(prize[i]);
		}


		getline(file, str, '\n');

		id++;
	}

	file.close();

	//Sort desc by payouts
	sort(prizes_list.begin(), prizes_list.end(), greater_than_payout());

}
