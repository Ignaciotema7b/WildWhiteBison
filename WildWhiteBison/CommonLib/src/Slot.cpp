#include "..\include\Slot.h"



void Slot::init(unsigned n_reels, unsigned n_rows, unsigned stake, unsigned n_symbols)
{
	//Inicializa RNG y stake
	Game::init(stake);

	//Init screen size
	for (unsigned row = 0; row < n_rows; row++)
	{
		vector<int> reels(n_reels, -1);
		screen.push_back(reels);
	}

	//Initialize reels heights:
	for (unsigned reel = 0; reel < n_reels; reel++)
		reels_heights.push_back(n_rows);

	//Initialize screen_symbols_count size
	for (unsigned s = 0; s < n_symbols; s++)
	{
		vector<unsigned> v(n_reels, 0);
		screen_symbols_count.push_back(v);
	}

	nrows = (unsigned)screen.size();
	nreels = (unsigned)screen[0].size();
	ntrackers = 0;

	//Init symbol multipliers...
	for (unsigned row = 0; row < n_rows; row++)
	{
		vector<unsigned> multiplier(n_reels, 0);
		symbol_multipliers.push_back(multiplier);
	}

	//Initialize vector dim to evaluate clusters
	vector<bool> v(n_reels, false);
	for (int i = 0; i < n_rows; i++)
		visited.push_back(v);

}

void Slot::init(unsigned n_reels, unsigned n_trackers, unsigned n_rows, unsigned stake, unsigned n_symbols)
{
	init(n_reels, n_rows, stake, n_symbols);
	ntrackers = n_trackers;

	//Add rows to screen to include trackers:
	for (unsigned row = 0; row < n_trackers; row++)
	{
		vector<int> reels(n_reels, -1);
		screen.push_back(reels);
	}

}

void Slot::get_screen(Reel_set& reel_set)
{
	for (unsigned r = 0; r < nrows; ++r)
	{
		for (unsigned c = 0; c < nreels ; ++c)
		{
			screen[r][c] = -1;

			//Completar con los simbolos necesarios segun la altura del reel:
			if (reels_heights[c] > r)
				screen[r][c] = reel_set.reel_strips[c][(reel_set.stops[c] + r) % ((int)reel_set.reel_strips[c].size())];
		}
	}

	//Trackers are added always at the bottom:
	for (unsigned r = nrows; r < nrows + ntrackers; ++r)
	{
		for (unsigned c = 0; c < nreels; ++c)
		{
			screen[r][c] = -1;

			//Completar con los simbolos necesarios segun la altura del reel:
			if (trackers_reels[0] <= c && c <= trackers_reels[1])
				screen[r][c] = reel_set.trackers_strips[r-nrows][(reel_set.stops_trackers[r - nrows] + c) % ((int)reel_set.reel_strips[r - nrows].size())];
				//reel_set.reel_strips[c][(reel_set.stops[c] + r) % ((int)reel_set.reel_strips[c].size())];
		}
	}

}

void Slot::print_screen()
{
	for (unsigned r = 0; r < (int)screen.size(); r++)
	{
		for (unsigned c = 0; c < (int)screen[0].size(); c++)
		{
			if(screen[r][c] >= 0)
				printf("%d\t", screen[r][c]);
			else
				printf(" \t");
		}
		printf("\n");
	}
	printf("\n\n");

}

void Slot::print_screen(Slot_config& config)
{
	for (unsigned r = 0; r < (int)screen.size(); r++)
	{
		for (unsigned c = 0; c < (int)screen[0].size(); c++)
		{
			if (screen[r][c] >= 0)
				printf("%s\t", config.symbols_list[screen[r][c]].short_name);
			else
				printf(" \t");
		}
		printf("\n");
	}
	printf("\n\n");
}

string Slot::log_screen(Slot_config& config)
{
	string str = "\n{\"S\":[";

	for (unsigned r = 0; r < (int)screen.size(); r++)
	{
		str += "\n[";
		for (unsigned c = 0; c < (int)screen[r].size(); c++)
		{
			if (screen[r][c] >= 0)
				str += "\"" + (string) config.symbols_list[screen[r][c]].short_name + "\"";
			else
				str += "\"_\"";

			if (c < (int)screen[r].size() - 1) {
				str += ",";
			}
		}
		str += "]";
		if (r < (int)screen.size() - 1) {
			str += ",";
		}
	}
	str += "\n]\n}\n";

	return str;
}

string Slot::log_screen(Slot_config& config, int spin)
{
	string str = "\n  \"spin_" + std::to_string(spin) + "\": {\n\"S\":[";

	for (unsigned r = 0; r < (int)screen.size(); r++)
	{
		str += "\n[";
		for (unsigned c = 0; c < (int)screen[r].size(); c++)
		{
			if (screen[r][c] >= 0)
				str += "\"" + (string)config.symbols_list[screen[r][c]].short_name + "\"";
			else
				str += "\"_\"";

			if (c < (int)screen[r].size() - 1) {
				str += ",";
			}
		}
		str += "]";
		if (r < (int)screen.size() - 1) {
			str += ",";
		}
	}
	str += "\n]\n}\n";

	return str;
}

unsigned Slot::get_ways_win(Slot_config &config)
{
	//Actualizo contador de simbolos por reel:
	count_symbols_on_screen();


#ifdef LOG_PROTO
	vector<json> proto_prizes;
#endif // LOG_PROTO


	//Add wilds to the counter of symbols that subtitutes:
	//		If "s" is wild and substitutes for "t" then counter of "t" is updated with occurrences/counter of "s"
	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		for (unsigned t = 0; t < (unsigned)config.symbols_list.size(); t++)
		{
			if (config.substitutes_for[s][t])
			{
				for (unsigned r = 0; r < config.n_reels; r++)
				{
					screen_symbols_count[t][r] += screen_symbols_count[s][r];
				}
			}
				
		}
	}


	unsigned win = 0;
	unsigned ways;
	unsigned reel;

	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		ways = 1;
		reel = 0;

		while (reel < config.n_reels && screen_symbols_count[s][reel] > 0)
		{
			ways *= screen_symbols_count[s][reel];
			reel++;
		}

		/*if (config.paytable[s][reel])
		{
			cout << "SIMBOLO: " << s << endl;
			cout << "xN: " << reel << endl;
			cout << "WAYS: " << ways << endl;
			cout << "PAGA:" << config.paytable[s][reel] << endl;

		}*/
		win += config.paytable[s][reel] * ways;
#ifdef LOG_PROTO
		if (config.paytable[s][reel]) {
			json prize;
			//prize["name"] = symbols_name[s]; //Se usa esto???

			vector<json> symbols;
			for (unsigned row = 0; row < (unsigned)screen.size(); row++)
			{
				for (unsigned reelIndex = 0; reelIndex < reel; reelIndex++)
				{
					if (screen[row][reelIndex] >= 0 && (screen[row][reelIndex] == s || config.substitutes_for[screen[row][reelIndex]][s]))
					{
						json symbol;
						symbol["symbolIndex"] = screen[row][reelIndex] + 1;
						//symbol["name"] = symbols_name[screen[row][reelIndex]]; //Se usa esto???
						symbol["reel"] = reelIndex;
						symbol["position"] = row;
						symbols.push_back(symbol);
					}
				}
			}
			prize["symbols"] = symbols;
			proto_prizes.push_back(prize);

		}
#endif
	}
#ifdef LOG_PROTO
this_step_proto_play[proto_log_state]["wonPrizes"] = proto_prizes;
#endif
	return win;
}

unsigned Slot::get_ways_win(Slot_config& config, vector<unsigned> reel_multipliers)
{
	//Actualizo contador de simbolos por reel:
	count_symbols_on_screen();

#ifdef LOG_PROTO
	vector<json> proto_prizes;
#endif // LOG_PROTO

	//Add wilds to the counter of symbols that subtitutes:
	//		If "s" is wild and substitutes for "t" then counter of "t" is updated with occurrences/counter of "s"
	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		for (unsigned t = 0; t < (unsigned)config.symbols_list.size(); t++)
		{
			if (config.substitutes_for[s][t])
			{
				for (unsigned r = 0; r < config.n_reels; r++)
				{
					screen_symbols_count[t][r] += screen_symbols_count[s][r];
				}
			}

		}
	}


	unsigned win = 0;
	unsigned ways;
	unsigned reel;

	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		ways = 1;
		reel = 0;

		while (reel < config.n_reels && screen_symbols_count[s][reel] > 0)
		{
			ways *= screen_symbols_count[s][reel] * reel_multipliers[reel];
			reel++;
		}

		win += config.paytable[s][reel] * ways;
#ifdef LOG_PROTO
		if (config.paytable[s][reel]) {
			json prize;
			//prize["name"] = symbols_name[s]; //Se usa esto???

			vector<json> symbols;
			for (unsigned row = 0; row < (unsigned)screen.size(); row++)
			{
				for (unsigned reelIndex = 0; reelIndex < reel; reelIndex++)
				{
					if (screen[row][reelIndex] >= 0 && (screen[row][reelIndex] == s || config.substitutes_for[screen[row][reelIndex]][s]))
					{
						json symbol;
						symbol["symbolIndex"] = screen[row][reelIndex] + 1;
						//symbol["name"] = symbols_name[screen[row][reelIndex]]; //Se usa esto???
						symbol["reel"] = reelIndex;
						symbol["position"] = row;
						symbols.push_back(symbol);
					}
				}
			}
			prize["symbols"] = symbols;
			proto_prizes.push_back(prize);

		}
#endif
	}
#ifdef LOG_PROTO
	this_step_proto_play[proto_log_state]["wonPrizes"] = proto_prizes;
#endif
	return win;
}

int Slot::get_line_win(Slot_config& config, int line_number)
{
	int x;
	int symbol;

	for (int i = 0; i < (int)config.prizes_list.size(); i++)
	{
		x = 0;
		
		//Check if the xN first symbols are matching the prize symbol (or is a wild...)
		while (x < (int) config.prizes_list[i].xN)
		{
			
			symbol = screen[config.paylines.lines[line_number][x]][x];
			x++;
			//If symbol in screen is different from symbol in prize and it is not a wild substitution..
			if (symbol != config.prizes_list[i].symbol_ID &&
				find(config.prizes_list[i].wildSubst.begin(), config.prizes_list[i].wildSubst.end(), symbol) == config.prizes_list[i].wildSubst.end())
			{
				x--;
				break;
			}
		}

		//Only highest prize is paid and prizes_list is ordered by payout desc.
		if (x == config.prizes_list[i].xN)
			return i;

			
	}
	
	return -1;
}

void Slot::reset_symbol_multipliers()
{
	for (int row = 0; row < (int)symbol_multipliers.size(); row++)
		for (int reel = 0; reel < (int)symbol_multipliers[0].size(); reel++)
			symbol_multipliers[row][reel] = 0;
}

vector<Cluster> Slot::findClusters(Slot_config& config)
{
	vector<Cluster> clusters;
	
	for (int i = 0; i < nrows; i++)
		for (int j = 0; j < nreels; j++)
			visited[i][j] = false;

	for (int i = 0; i < nrows; ++i) {
		for (int j = 0; j < nreels; ++j) {
			if (screen[i][j] != -1 && 
				find(config.wild_symbols.begin(), config.wild_symbols.end(), screen[i][j]) == config.wild_symbols.end()) {
				Cluster cluster;
				cluster_visited = visited; //Copy 

				dfs(config, cluster_visited, i, j, screen[i][j], cluster);
				
				if (!cluster.empty()) {
					clusters.push_back(cluster);
					// Actualizamos visited solo para las celdas que no son wild
					for (const auto& coord : cluster) {
						if(find(config.wild_symbols.begin(), config.wild_symbols.end(), screen[coord.first][coord.second]) == config.wild_symbols.end())
						{
							visited[coord.first][coord.second] = true;
						}
					}
				}
			}
		}
	}

	return clusters;
}

vector<winning_Cluster> Slot::get_winning_clusters(Slot_config& config)
{
	vector<winning_Cluster> resp;
	vector<Cluster> clusters = findClusters(config);

	int symb_aux;
	bool stop;

	for (int i = 0; i < (int)clusters.size(); i++)
	{
		int j = 0;
		winning_Cluster aux;
		stop = false;

		aux.cells = clusters[i];

		//Find symbol in cluster excluding wilds:
		aux.symbol = -1;
		aux.pays = 0;

		while (!stop)
		{
			symb_aux = screen[clusters[i][j].first][clusters[i][j].second];

			if (find(config.wild_symbols.begin(), config.wild_symbols.end(), symb_aux) == config.wild_symbols.end())
				aux.symbol = symb_aux;

			j++;

			stop = (j >= (int)clusters[i].size()) || (aux.symbol >= 0);
		}

		if (aux.symbol >= 0)
			aux.pays = config.paytable[aux.symbol][(int)aux.cells.size()];

		if (aux.pays > 0)
			resp.push_back(aux);
	}

	return resp;
}


unsigned Slot::get_lines_win(Slot_config& config)
{
	unsigned total_win = 0;
	unsigned win = 0;
	int prize;

	for (int line = 0; line < config.paylines.lines.size(); line++) {

		prize = get_line_win(config, line);

		if (prize >= 0 && !config.prizes_list[prize].isScatter)
			win = config.prizes_list[prize].payout ;
		else
			win = 0;

		total_win += win;
	}


	return total_win;
}

unsigned Slot::get_lines_win_fast_evaluation(Slot_config& config)
{
	unsigned total_win = 0;
	unsigned win = 0;
	int prize;


	for (int line = 0; line < config.paylines.lines.size(); line++) {

		int s[6] = { 0,0,0,0,0,0 };
		for (int r = 0; r < config.n_reels; r++)
			s[r] = screen[config.paylines.lines[line][r]][r];

		prize = config.paytable_lines[s[0]][s[1]][s[2]][s[3]][s[4]][s[5]];

		total_win += prize;
	}


	return total_win;
}

unsigned Slot::get_lines_win_with_multiplier(Slot_config& config, multiplier type_of_multiplier)
{
	unsigned total_win = 0;
	unsigned win = 0;
	unsigned multiplier = 1;
	int prize;

#ifdef LOG_PROTO
	vector<json> proto_prizes;
#endif // LOG_PROTO

	for (int line = 0; line < config.paylines.lines.size(); line++) {

		prize = get_line_win(config, line);
		if (prize >= 0)
		{
			//Initialize multiplier
			switch (type_of_multiplier)
			{
			case additive:
				multiplier = 0;
				break;
			case multiplicative:
				multiplier = 1;
				break;
			default:
				break;
			}

			for (int reel = 0; reel < (int) config.prizes_list[prize].xN; reel++)
			{
				switch (type_of_multiplier)
				{
				case additive:
					multiplier += symbol_multipliers[config.paylines.lines[line][reel]][reel];
					break;
				case multiplicative:
					if(symbol_multipliers[config.paylines.lines[line][reel]][reel] > 0)
						multiplier *= symbol_multipliers[config.paylines.lines[line][reel]][reel];
					break;
				default:
					break;
				}
			}
			
			//If multipliers are additive and there is no multiplier it should pay as x1
			if (multiplier == 0)
				multiplier = 1;

			win = config.prizes_list[prize].payout * multiplier;

#ifdef LOG_PROTO
			json prizeJson;
			unsigned s = config.prizes_list[prize].symbol_ID;
			vector<json> symbols;
			for (unsigned row = 0; row < (unsigned)screen.size(); row++)
			{
				for (unsigned reelIndex = 0; reelIndex < config.prizes_list[prize].xN; reelIndex++)
				{
					if (screen[row][reelIndex] >= 0 && (screen[row][reelIndex] == s || config.substitutes_for[screen[row][reelIndex]][s]))
					{
						json symbol;
						symbol["symbolIndex"] = screen[row][reelIndex];
						symbol["reel"] = reelIndex;
						symbol["position"] = row;
						symbols.push_back(symbol);
					}
				}
			}
			prizeJson["symbols"] = symbols;
			proto_prizes.push_back(prizeJson);
#endif
		}
		else
			win = 0;

		total_win += win;
	}

#ifdef LOG_PROTO
	this_step_proto_play[proto_log_state]["wonPrizes"] = proto_prizes;

#endif

	return total_win;
}

//TODO: no esta terminada
void Slot::print_theoretical_rtp_by_reel_sets(Slot_config& config)
{
	//TODO: por defecto calculo Ways L2R
	for (int reel_set = 0; reel_set < (int)config.reel_sets.size(); reel_set++)
	{
		for (int symbol = 0; symbol < (int)config.paytable.size(); symbol++)
		{
			for (int oak = 1; oak < (int)config.paytable[0].size(); oak)
			{
				if (config.paytable[symbol][oak] != 0)
				{

				}
			}
		}
	}

}

unsigned Slot::get_ways_win_R2L(Slot_config &config)
{
	//Actualizo contador de simbolos por reel:

	count_symbols_on_screen();

	//Add wilds to the counter of symbols that subtitutes:
	//		If "s" is wild and substitutes for "t" then counter of "t" is updated with occurrences/counter of "s"
	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		for (unsigned t = 0; t < (unsigned)config.symbols_list.size(); t++)
		{
			if (config.substitutes_for[s][t])
			{
				for (unsigned r = 0; r < config.n_reels; r++)
				{
					screen_symbols_count[t][r] += screen_symbols_count[s][r];
				}
			}

		}
	}

	unsigned win = 0;
	unsigned ways;
	int reel;
	unsigned xN;

	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		ways = 1;
		reel = config.n_reels-1;
		xN = 0;

		while (reel >= 0 && screen_symbols_count[s][reel] > 0)
		{
			ways *= screen_symbols_count[s][reel];
			reel--;
			xN++;
		}

		win += config.paytable[s][xN] * ways;
	}

	return win;
}

unsigned Slot::get_ways_win_both(Slot_config &config)
{
	//Actualizo contador de simbolos por reel:
	count_symbols_on_screen();

	//Add wilds to the counter of symbols that subtitutes:
	//		If "s" is wild and substitutes for "t" then counter of "t" is updated with occurrences/counter of "s"
	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		for (unsigned t = 0; t < (unsigned)config.symbols_list.size(); t++)
		{
			if (config.substitutes_for[s][t])
			{
				for (unsigned r = 0; r < config.n_reels; r++)
				{
					screen_symbols_count[t][r] += screen_symbols_count[s][r];
				}
			}

		}
	}

	unsigned win = 0;
	unsigned ways;
	int reel;
	unsigned xN;

	//Left to right
	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		ways = 1;
		reel = 0;

		while (reel < (int) config.n_reels && screen_symbols_count[s][reel] > 0)
		{
			ways *= screen_symbols_count[s][reel];
			reel++;
		}

		win += config.paytable[s][reel] * ways;
	}

	//Right to left
	for (unsigned s = 0; s < (unsigned)config.symbols_list.size(); s++)
	{
		ways = 1;
		reel = config.n_reels -1;
		xN = 0;
		while (reel >= 0 && screen_symbols_count[s][reel] > 0)
		{
			ways *= screen_symbols_count[s][reel];
			reel--;
			xN++;
		}

		//6 oak are counted only once
		if(xN < config.n_reels)
			win += config.paytable[s][xN] * ways;
	}

	return win;
}

unsigned Slot::get_scatter_wins(Slot_config& config)
{
	unsigned win = 0;

#ifdef LOG_PROTO
	vector<json> proto_prizes;
#endif // LOG_PROTO

	//Reset symbols counting
	for (int s = 0; s < MAX_N_SYMBOLS; s++)
		symbols_count[s] = 0;

	for (int reel = 0; reel < config.n_reels; reel++)
	{
		for (int row = 0; row < config.n_rows; row++)
		{
			symbols_count[screen[row][reel]]++;
		}
	}

	//Add wild in the counting of the symbols that it replaces
	if (!config.wild_symbols.empty())
	{
		if (symbols_count[config.wild_symbols[0]])
		{
			for (int i = 0; i < MAX_N_SYMBOLS; i++)
				if (config.substitutes_for[config.wild_symbols[0]][i])
					symbols_count[i] += symbols_count[config.wild_symbols[0]];
		}
	}


	//Calculate payouts and add to total win
	for (int s = 0; s < (int)config.symbols_list.size(); s++) {
		win += config.paytable[s][symbols_count[s]];

#ifdef LOG_PROTO
		if (config.paytable[s][symbols_count[s]]) {
			json prize;
			vector<json> symbols;
			for (unsigned row = 0; row < (unsigned)screen.size(); row++)
			{
				for (unsigned reel = 0; reel < (unsigned)screen[row].size(); reel++)
				{
					if (screen[row][reel] >= 0 && (screen[row][reel] == s || config.substitutes_for[screen[row][reel]][s]))
					{
						json symbol;
						symbol["symbolIndex"] = screen[row][reel];
						symbol["reel"] = reel;
						symbol["position"] = row;
						symbols.push_back(symbol);
					}
				}
			}
			prize["symbols"] = symbols;
			proto_prizes.push_back(prize);

		}
#endif
	}
#ifdef LOG_PROTO
	this_step_proto_play[proto_log_state]["wonPrizes"] = proto_prizes;
#endif
	return win;
}



unsigned Slot::count_scatters(unsigned symbolID)
{
	unsigned n = 0;

	//Cuento todas las filas de screen
	for (unsigned row = 0; row < (unsigned) screen.size(); row++)
	{
		for (unsigned reel = 0; reel < nreels; reel++)
		{
			if (screen[row][reel] == (int)symbolID)
				n++;
		}
	}

	return n;
}

void Slot::count_symbols_on_screen()
{
	//Reset
	for (unsigned symbol = 0; symbol < (int) screen_symbols_count.size(); symbol++)
	{
		for (int reel = 0; reel < (int) screen_symbols_count[0].size() ; reel++)
		{
			screen_symbols_count[symbol][reel] = 0;
		}
	}

	//Cuento todas las filas de screen
	for (unsigned row = 0; row < (unsigned) screen.size(); row++)
	{
		for (unsigned reel = 0; reel < nreels; reel++)
		{
			if (screen[row][reel] >= 0 && screen[row][reel] < (int) screen_symbols_count.size() )
			{
				screen_symbols_count[screen[row][reel]][reel]++;
			}
		}
	}
}

void Slot::explode_symbols_ways(Slot_config& config)
{
	//USAR DESPUES DE get_way_wins PARA QUE ESTEN SUMADOS LO WILDS EN LAS CUENTAS DE CADA SIMBOLO

	int reel;

	//Left to right... 
	for (unsigned row = 0; row < (unsigned) screen.size() ; row++)
	{
		reel = 0;
		unsigned symbol = screen[row][0];

		if (symbol != -1)
		{
			while (reel < (int)nreels && screen_symbols_count[symbol][reel] > 0)
			{
				reel++;
			}

			if (config.paytable[symbol][reel])
			{
				//Explode all winning "symbols" from left to right
				for (int col = 0; col < reel; col++)
				{
					for (unsigned row = 0; row < (unsigned) screen.size() ; row++)
					{
						if (screen[row][col] == symbol || config.substitutes_for[screen[row][col]][symbol])
							screen[row][col] = -1;
					}
				}
			}
		}
	}

}

void Slot::explode_symbols_scatter_pays(Slot_config& config)
{
	for (unsigned reel = 0; reel < config.n_reels; reel++)
	{
		for (unsigned row = 0; row < config.n_rows; row++)
		{
			if (config.paytable[screen[row][reel]][symbols_count[screen[row][reel]]] > 0)
				screen[row][reel] = -1;

		}
	}
}

void Slot::drop_new_symbols(Reel_set& reel_set)
{

	for (unsigned reel = 0; reel < nreels; ++reel)
	{
		//Drop symbols at the bottom first...
		for (int row = (int)reels_heights[reel] - 1; row >= 0 ; row--)
		{
			if (screen[row][reel] == -1)
			{
				//Move the stop one place up...(if the function get_screen was called then the symbol at "stop" was already used)
				reel_set.stops[reel] = (reel_set.stops[reel] + reel_set.reels_length[reel] - 1) % reel_set.reels_length[reel];
				screen[row][reel] = reel_set.reel_strips[reel][reel_set.stops[reel]]; 
			}

		}
	}

	//Trackers are added always at the bottom:
	for (unsigned r = nrows; r < nrows + ntrackers; ++r)
	{
		for (unsigned c = 0; c < nreels; ++c)
		{
			//If there is an empty space in the tracker then fill in with a new symbol...
			if (trackers_reels[0] <= c && c <= trackers_reels[1] && screen[r][c] == -1)
			{
				//Move the stop one place "down"...(if the function get_screen was called then the symbol at "stop" was already used)
				reel_set.stops_trackers[r - nrows] = (reel_set.stops_trackers[r - nrows] + 1) % reel_set.trackers_length[r - nrows];
				screen[r][c] = reel_set.trackers_strips[r - nrows][(reel_set.stops_trackers[r - nrows] ) % ((int)reel_set.trackers_strips[r - nrows].size())];
			}
				
		}
	}

}

void Slot::clear_screen()
{
	for (int row = 0; row < (int)screen.size(); row++)
		for (int reel = 0; reel < (int)screen[0].size(); reel++)
			screen[row][reel] = -1;
}

void Slot::rename_symbols_on_screen(int old_id, int new_id)
{
	for (unsigned row = 0; row < (unsigned)screen.size(); row++)
	{
		for (unsigned reel = 0; reel < nreels; reel++)
		{
			if (screen[row][reel] == old_id)
				screen[row][reel] = new_id;
		}
	}
}

void Slot::cascade()
{
	//Move symbols to bottom...
	for (unsigned reel = 0; reel < nreels; reel++)
	{
		int fill_row = reels_heights[reel] - 1;
		int current_row = fill_row;

		while (current_row >= 0)
		{
			if (screen[current_row][reel] >= 0) //If there is a symbol then...let it fall
			{
				screen[fill_row][reel] = screen[current_row][reel];

				if (current_row != fill_row) //If symbol went down (it may keep in its position)...then clean the cell
					screen[current_row][reel] = -1;
				fill_row--;
			}

			current_row--;
		}
	}

	//Move symbols in trackers to the left
	for (unsigned tracker = 0; tracker < ntrackers; tracker++)
	{
		int fill_reel = trackers_reels[0];
		unsigned current_reel = fill_reel;

		while (current_reel <= trackers_reels[1])
		{
			//Trackers are at the bottom of screen[][]
			if (screen[nrows + tracker][current_reel] >= 0) //If there is a symbol then...move it to the left
			{
				screen[nrows + tracker][fill_reel] = screen[nrows + tracker][current_reel];

				if (current_reel != fill_reel) //If symbol went down (it may keep in its position)...then clean the cell
					screen[nrows + tracker][current_reel] = -1;
				fill_reel++;
			}

			current_reel++;
		}
	}
}

void Slot::generate_all_symbols_combinations(Slot_config& config, int index)
{
	int prize;
	int win;
	if (index == config.n_reels)
	{
		int s[6] = { 0,0,0,0,0,0 };
		for (int r = 0; r < config.n_reels; r++)
			s[r] = screen[config.paylines.lines[0][r]][r];

		prize = get_line_win(config, 0);

		if (prize >= 0 && !config.prizes_list[prize].isScatter)
			win = config.prizes_list[prize].payout;
		else
			win = 0;

		config.paytable_lines[s[0]][s[1]][s[2]][s[3]][s[4]][s[5]] = win;
		
		//for (int r = 0; r < config.n_reels; r++)
		//	cout << s[r] << ",";

		//cout << "  Pays: " << config.paytable_lines[s[0]][s[1]][s[2]][s[3]][s[4]][s[5]] << endl;
		return;
	}

	for (int i = 0; i < (int)config.symbols_list.size(); i++)
	{
		screen[config.paylines.lines[0][index]][index] = i;
		generate_all_symbols_combinations(config, index + 1);
	}
}

void Slot::dfs(Slot_config& config, vector<vector<bool>>& visited, int x, int y, int symbol_ID, Cluster& cluster)
{
	//If the reel doesn't exist then return.
	if (y < 0 || y >= info.n_reels)
		return;

	//If the row in the current reel doesn't exist or the cell was already visited then return
	if (x < 0 || x >= reels_heights[y] || visited[x][y]) 
		return;

	//If the symbol in the cell is not the symbol in the cluster nor a wild that replaces it then return
	if (screen[x][y] != symbol_ID && !config.substitutes_for[screen[x][y]][symbol_ID])
		return;

	visited[x][y] = true;
	cluster.emplace_back(x, y);

	//If diagonal cells are neighbors then evaluate the 8 neighbors...
	if (config.type == "Clusters (8 way)")
	{
		for (int i = 0; i < 8; ++i) {
			int nx = x + dx_8[i];
			int ny = y + dy_8[i];
			dfs(config, visited, nx, ny, symbol_ID, cluster);
		}
	}
	else //...otherwise evaluate only left, right, up and down cells
	{
		for (int i = 0; i < 4; ++i) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			dfs(config, visited, nx, ny, symbol_ID, cluster);
		}
	}
}

#ifdef LOG_PROTO
vector<vector<int>> Slot::parse_screen_for_proto(vector<vector<int>> screen) { // se puede poner en la subclase
	//Init screen size
	vector<vector<int>> new_screen;
	for (unsigned row = 0; row < screen[0].size(); row++)
	{
		//vector<int> rows(screen[row].size(),-1);
		vector<int> rows(0);
		new_screen.push_back(rows);
	}

	for (unsigned reel = 0; reel < screen[0].size(); reel++) {
		for (int row = 0; row < reels_heights[reel]; row++) {
			new_screen[reel].push_back(screen[row][reel]);
		}

	}
	return new_screen;
}



vector<int> Slot::parse_top_tracker_for_proto(vector<vector<int>> screen)
{
	vector<int> topTracker;
	for (size_t i = 0; i < 4; i++)
	{
		//unsigned symbolId = screen[7][i + 1] + 1;
		unsigned symbolId = screen[7][i + 1]; // cambio para numerar desde 0
		topTracker.push_back(symbolId);
	}
	return topTracker;
}

#endif