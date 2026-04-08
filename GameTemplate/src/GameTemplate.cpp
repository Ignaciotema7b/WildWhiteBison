#include "GameTemplate.h"
#include <filesystem>

void GameTemplate::generate_json(Game_info info)
{
	json j;

	//Config
	j["config"] = config.json_config;

	//Info
	info.json_info["seed"] = rng_seed;
	j["info"] = info.json_info;

	//Stats
	j["stats"] = stats.stats_to_json(stake);

	//Save JSON
	time_t finish_time = time(NULL);

	string rtp = to_string(stats.total_payout * 1.0 / stats.plays / info.bet);

	//If folder "results" does not exist then create it.
	if (!std::filesystem::exists(info.folder_results))
		std::filesystem::create_directory(info.folder_results);

	std::ofstream o(info.folder_results + "\\_rtp_" + rtp + "__" + to_string(finish_time) +
		"__" + info.game_name + "_v" + info.version + ".json");
	o << std::setw(2) << j << std::endl;
}

void GameTemplate::init(Game_info info)
{	
	config.init(info);
	Slot::init(info.n_reels, info.number_of_trackers, info.number_of_rows, info.bet, (unsigned)config.symbols_list.size());
	
	if(info.type == "Paylines")
		Slot::generate_all_symbols_combinations(config, 0);

	//Load reel sizes...
	for (int i = 0; i < (int)info.reel_heights.size(); i++)
	{
		reels_heights[i] = info.reel_heights[i];
	}

	//Sessions info
	stats.starting_balance = info.starting_balance;
	stats.current_balance = info.starting_balance;

	bet_for_cap = info.bet_for_cap;
	plays_to_print = info.plays_to_print;

#ifdef LOG_PLAYS
	randomLogs.init(info);
#endif // LOG_PLAYS

}

void GameTemplate::play(unsigned long long plays)
{
	unsigned using_reel_set;

	//Base game
	unsigned long long base_win;
	unsigned long long new_win;

	//Total win for each play.
	unsigned total_win;

	for (unsigned long long iter = 0; iter < plays; iter++)
	{
		//Start play iteration...
		if (iter > 0 && !(iter % plays_to_print))
		{
			system("CLS");
			cout << endl << "Play mode: " << info.mode << endl;
			stats.print_stats(stake);
		}

		//Reset
		base_win = 0;
		new_win = 0;
		total_win = 0;

		//Resets all elements in "screen"...(sets  screen[i][j] = -1)
		clear_screen();

		//Choose a reel set for base game.
		using_reel_set = config.w_reel_sets_main.sample(rng);

		//Pick the stop for each reel and fill in the screen with symbols from the reel set.
		config.reel_sets[using_reel_set].spin(rng);
		drop_new_symbols(config.reel_sets[using_reel_set]);


		//...write game logic here



		//Stats
		stats.count_stats_play(total_win, stake);




	}

	//Save simulation data into json file
	generate_json(info);

	//Clear screen and print simulation stats
	system("CLS");
	cout << endl << "Play mode: " << info.mode << endl;
	stats.print_stats(stake);
}

