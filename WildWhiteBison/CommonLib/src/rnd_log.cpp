#include "..\include\rnd_log.h"

void rnd_log::init(Game_info info)
{
	randomLog_path = info.folder_config;
	randomLog_path.erase(randomLog_path.size() - 6);
	randomLog_path += "logs\\randomLog.txt";


	gameDetails_path = info.folder_config;
	gameDetails_path.erase(gameDetails_path.size() - 6);
	gameDetails_path += "logs\\gameDetails.txt";

	fopen_s(&file_rnd_log, randomLog_path.c_str(), "w");
	fopen_s(&file_log_plays, gameDetails_path.c_str(), "w");

	config.init(info);
}

void rnd_log::log_rnd(Weighted_values w)
{
	str_log_rnd += w.name + ":" +
		std::to_string(w.last_random) + ":" +
		std::to_string(w.total_weight) + ",";
}

void rnd_log::log_stops(Reel_set reel_set)
{
	//Stop for first reel
	str_log_rnd += "ST:" +
		std::to_string(reel_set.stops[0]) + ":" +
		std::to_string(reel_set.reels_length[0]);

	//Stop for reels 2 
	for (int i = 1; i < reel_set.number_of_reels; i++)
	{
		str_log_rnd += ",ST:" +
			std::to_string(reel_set.stops[i]) + ":" +
			std::to_string(reel_set.reels_length[i]);
	}

	//Trackers stops
	for (int i = 0; i < reel_set.number_of_trackers; i++)
	{
		//Adjust stop: it is logged after stop is updated for next spin
		//Here the adjustment is made considering a tracker of 4 symbols
		str_log_rnd += ",ST:" +
			std::to_string((reel_set.stops_trackers[i] + (int)reel_set.trackers_strips[i].size() - 3) % (int)reel_set.trackers_strips[i].size()) + ":" +
			std::to_string(reel_set.trackers_length[i]);
	}

	str_log_rnd += ",";
}

void rnd_log::log_stops(Reel_set reel_set, string mask, vector<int> offsets)
{
	//Stop for first reel
	str_log_rnd += mask + ":" +
		std::to_string((reel_set.stops[0] + offsets[0]) % reel_set.reels_length[0]) + ":" +
		std::to_string(reel_set.reels_length[0]);

	//Stop for reels 2 
	for (int i = 1; i < reel_set.number_of_reels; i++)
	{
		str_log_rnd += "," + mask + ":" +
			std::to_string((reel_set.stops[i] + offsets[i] ) % reel_set.reels_length[i]) + ":" +
			std::to_string(reel_set.reels_length[i]);
	}

	//Trackers stops
	for (int i = 0; i < reel_set.number_of_trackers; i++)
	{
		//Adjust stop: it is logged after stop is updated for next spin
		//Here the adjustment is made considering a tracker of 4 symbols
		str_log_rnd += "," + mask + ":" +
			std::to_string((reel_set.stops_trackers[i] + (int)reel_set.trackers_strips[i].size() - 3) % (int)reel_set.trackers_strips[i].size()) + ":" +
			std::to_string(reel_set.trackers_length[i]);
	}

	str_log_rnd += ",";
}

void rnd_log::log_stops(Reel_set reel_set, int reel)
{
		str_log_rnd += "ST:" +
			std::to_string(reel_set.stops[reel]) + ":" +
			std::to_string(reel_set.reels_length[reel]) + ",";
}

void rnd_log::log_spin_win(unsigned long long spin_win, vector<vector<int>> screen, string additional_info)
{
	//Random log
	//Primero remuevo la ultima coma
	if (str_log_rnd.back() == ',')
		str_log_rnd.pop_back();
	str_log_rnd += "#" + to_string_with_precision((spin_win) * 1.0 / 100) + ";";

	//Screen
	if(log_spin_count == 0)
		str_log_play += "{" + log_screen(screen) + ",";
	else
		str_log_play += log_screen(screen) + ",";

	log_spin_count++;
}

void rnd_log::log_total_win(unsigned long long total_win, vector<vector<int>> screen, string additional_info)
{
	//Random log
	if (str_log_rnd.back() == ',')
		str_log_rnd.pop_back();
	str_log_rnd += "#" + to_string_with_precision((total_win) * 1.0 / 100) + "\n";

	//Screen
	//Primero remuevo la ultima coma
	if (str_log_play.back() == ',')
		str_log_play.pop_back();
	str_log_play += "}\n========== end round: " + std::to_string(log_count) + " ==========\n\n";

	++log_count;

	fprintf(file_log_plays, str_log_play.c_str());
	fprintf(file_rnd_log, str_log_rnd.c_str());

	str_log_rnd = "";
	str_log_play = ""; 
	log_spin_count = 0;

	if (log_count == number_of_logs)
	{
		fclose(file_log_plays);
		fclose(file_rnd_log);
		cout << "log generated" << endl;
		system("PAUSE");
		exit(1);
	}
}

void rnd_log::log_rnd(string mask, unsigned result, unsigned range)
{
	str_log_rnd += mask + ":" + std::to_string(result) + ":" +
		std::to_string(range) + ",";
}

void rnd_log::log_rnd_without_replacement(Weighted_values w, unsigned sample_size)
{
	str_log_rnd += w.log_without_replacement(sample_size) + ",";
}

void rnd_log::resume()
{
	str_log_rnd = "";
	str_log_play = "";
	log_spin_count = 0;
}

string rnd_log::log_screen(vector<vector<int>> screen, string additional_info)
{
	string str = "\n  \"spin_" + std::to_string(log_spin_count) + "\": {\n\"S\":[";

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

	if (additional_info != "")
		str += "\n],\n" + additional_info + "\n}\n";
	else
		str += "\n]\n}\n";

	return str;
}
