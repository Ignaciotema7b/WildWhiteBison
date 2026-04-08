#include "WildWhiteBison.h"
#include <filesystem>

enum Symbols_Id {
	WILD = 0, SCATTER = 10, S_SYMBOL = 11,
	REGULAR = 0, BONUS_BUY = 1
};

//#define WILD 0
//#define SCATTER 10
//#define S_SYMBOL 11
//
//#define REGULAR 0
//#define BONUS_BUY 1

void WildWhiteBison::generate_json(Game_info info)
{
	json j;

	//Config
	j["config"] = config.json_config;

	//Info
	info.json_info["seed"] = rng_seed;
	std::ostringstream state;
	state << rng;

	j["info"] = info.json_info;
	j["info"]["rng_final_state"] = state.str();

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

void WildWhiteBison::init(Game_info info)
{
	config.init(info);
	Slot::init(info.n_reels, info.number_of_trackers, info.number_of_rows, info.bet, (unsigned)config.symbols_list.size());

	if (info.type == "Paylines")
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

void WildWhiteBison::play(unsigned long long plays)
{
	unsigned using_reel_set;

	//Total win for each play
	unsigned long long total_win;
	unsigned long long free_spins_win;
	unsigned long long bonus_win;

#ifdef LOG_PROTO
	rng.seed(888);
	rng_seed = 888;
#endif // LOG_PROTO

	int play_mode = config.play_mode.sample(rng);

	bool hold_and_spin;
	bool bison_spin;

	for (int reel = 0; reel < nreels; reel++) {
		reels_heights[reel] = reels_heights_local[reel];
	}

	int n_scatters;
	int n_H_symbols;
	int n_super_symbols;

	for (unsigned long long iter = 0; iter < plays; iter++)
	{
		//cout << iter << endl;
		//Start play iteration...
		if (!(iter % plays_to_print) && iter > 0)
		{
			if (iter == 52000000)
				int debug = 1;

			system("CLS");
			std::cout << endl << "Play mode: " << info.mode << endl;
			stats.print_stats(stake);
		}


		// Reset
		base_win = 0;
		total_win = 0;
		free_spins_win = 0;
		bonus_win = 0;
		converted_wilds.clear();
		n_scatters = 0;
		n_H_symbols = 0;
		n_super_symbols = 0;

		bison_spin = false;
		hold_and_spin = config.w_activate_HyS.sample(rng);
		if (!hold_and_spin)
			bison_spin = config.w_activate_bison_spin.sample(rng);


		if (hold_and_spin)
			using_reel_set = config.w_reel_sets_main[2].sample(rng);
		else if (bison_spin)
			using_reel_set = config.w_reel_sets_main[1].sample(rng);
		else
			using_reel_set = config.w_reel_sets_main[0].sample(rng);

#ifdef LOG_PROTO
		whole_proto_play.clear();
		this_step_proto_play.clear();
		proto_log_state = "basespin";
		proto_remaining_fs = -1;
#endif


		config.reel_sets[using_reel_set].spin(rng);
		clear_screen();

		// agregar super symbols a mano
		if (!hold_and_spin) {
			int n_super_symbols = config.add_super_symbols.sample(rng);
			for (int reel = 0; reel < n_super_symbols; reel++) {
				if (reel != 0 && reel != 4)
					screen[random(info.number_of_rows)][reel] = S_SYMBOL;
				else
					screen[random(info.number_of_rows - 1)][reel] = S_SYMBOL;
			}
		}

		drop_new_symbols(config.reel_sets[using_reel_set]);

		//Sugerencia. Contar los s�mbolos especiales para imprimir carteles de alerta en caso que haya errores en la planilla o en el c�digo.
		for (int reel = 0; reel < nreels; reel++) {
			int n_sc = 0;
			for (int row = 0; row < reels_heights[reel]; row++) {
				if (screen[row][reel] == SCATTER) {
					if (reel == 1 || reel == 3) {
						std::cout << "ERROR: en el reel " << reel << " del reel set " << using_reel_set << " hay un scatter" << endl;
						system("pause");
					}
					n_sc++;
					n_scatters++;
				}
				if (screen[row][reel] > 0 && screen[row][reel] <= 4) {
					n_H_symbols++;
				}
				if (screen[row][reel] == S_SYMBOL) {
					n_super_symbols++;
				}
			}
			if (n_sc > 1) {
				std::cout << "ERROR: en el reel " << reel << " del reel set " << using_reel_set << " hay mas de un scatter" << endl;
				system("pause");
			}
		}

		if (bison_spin && n_H_symbols == 0) {
			/*cout << "ERROR: est� el Bison Spin activo pero no hay ningun simbolo H para convertir en Wild" << endl;
			system("pause");*/
			bison_spin = false;
		}
		if (hold_and_spin && n_super_symbols < 5) {
			cout << "ERROR: est� el HyS activo pero no hay 5 super symbols en el reel set " << using_reel_set << endl;
			system("pause");
		}

		if (bison_spin)
			convert_high_to_wilds();

#ifdef LOG_PROTO
		// get winnings
		base_win += get_lines_win(config);

		// log proto
		proto_screen = parse_screen_for_proto(screen);
		this_step_proto_play[proto_log_state]["bisonSpinActive"] = bison_spin;
		this_step_proto_play[proto_log_state]["holdAndSpinActive"] = hold_and_spin;
		this_step_proto_play[proto_log_state]["linesWin"] = base_win;
		this_step_proto_play[proto_log_state]["screen"] = proto_screen;
#else
		base_win += get_lines_win_fast_evaluation(config);
#endif

		if (hold_and_spin) {
			bonus_win += play_bonus();

			stats.bonus_hits++;
		}

		//count_symbols_on_screen();
		bool free_spins = n_scatters == 3;
		if (free_spins) {
			free_spins_win = play_free_spins();
			stats.free_spins_hits++;
		}

		// Stats
		stats.total_payout_without_cap += base_win + bonus_win + free_spins_win;

		if (base_win + bonus_win + free_spins_win >= bet_for_cap * cap) {
			stats.hits_cap++;

			cap_wins(base_win, bonus_win, free_spins_win);
		}

		total_win = base_win + bonus_win + free_spins_win;

		// Stats
		if (bison_spin) {
			stats.bison_spin_win += base_win;
			stats.bison_spin_hits++;
		}
		else
			stats.base_win += base_win;

		stats.free_spins_win += free_spins_win;
		stats.bonus_win += bonus_win;

		stats.count_stats_play(base_win + free_spins_win + bonus_win, stake);

		if (bison_spin)
			stats.win_distr_bison_spin.count_stats_play(base_win, stake);
		else stats.win_distr_base.count_stats_play(base_win, stake);

		if (n_scatters == 3) // free spins
			stats.win_distr_fg.count_stats_play(free_spins_win, stake);

		if (hold_and_spin)
			stats.win_distr_bonus.count_stats_play(bonus_win, stake);

		stats.n_scatters_count[n_scatters]++;

		if (n_super_symbols >= 7)
			stats.n_ss_count[7]++;
		else stats.n_ss_count[n_super_symbols]++;

#ifdef LOG_PROTO

		whole_proto_play.push_back(this_step_proto_play);

		if (info.mode == "Regular") {
			if (json_proto_count <= PROTO_BASE_PLAYS) {
				all_proto_play.push_back(whole_proto_play);
				if (++json_proto_count == PROTO_BASE_PLAYS) {
					std::ofstream o("game\\proto\\plays_gral.json");
					json final_json;
					final_json["spins"] = all_proto_play;
					o << std::setw(2) << final_json.dump() << std::endl;
					proto_logs_ready++;
					cout << "gral logs ready" << endl;

					all_proto_play.clear();
				}
			}

			if (hold_and_spin && json_proto_bonus_count <= PROTO_BONUS_PLAYS) {
				all_bonus_proto_play.push_back(whole_proto_play);
				if (++json_proto_bonus_count == PROTO_BONUS_PLAYS) {
					std::ofstream o("game\\proto\\plays_bonus.json");
					json final_json;
					final_json["spins"] = all_bonus_proto_play;
					o << std::setw(2) << final_json.dump() << std::endl;
					proto_logs_ready++;
					cout << "bonus logs ready" << endl;

					all_bonus_proto_play.clear();
				}
			}

			if (free_spins && json_proto_fgs_count <= PROTO_FGS_PLAYS) {
				all_fgs_proto_play.push_back(whole_proto_play);
				if (++json_proto_fgs_count == PROTO_FGS_PLAYS) {
					std::ofstream o("game\\proto\\plays_fgs.json");
					json final_json;
					final_json["spins"] = all_fgs_proto_play;
					o << std::setw(2) << final_json.dump() << std::endl;
					proto_logs_ready++;
					cout << "free spins logs ready" << endl;

					all_fgs_proto_play.clear();
				}
			}

			if (proto_logs_ready == 3) {
				cout << "all REG logs ready" << endl;
				exit(0);
			}
		}

		if (info.mode == "Bonus Buy") {
			if (json_proto_bb_count <= PROTO_BONUS_BUY_PLAYS) {
				all_bb_proto_play.push_back(whole_proto_play);
				if (++json_proto_bb_count == PROTO_BONUS_BUY_PLAYS) {
					std::ofstream o("game\\proto\\plays_bb.json");
					json final_json;
					final_json["spins"] = all_bb_proto_play;
					o << std::setw(2) << final_json.dump() << std::endl;
					proto_logs_ready++;
					cout << "bonus buy logs ready" << endl;
					all_bb_proto_play.clear();
				}
			}
			if (proto_logs_ready == 1) {
				cout << "all BonusBuy logs ready" << endl;
				exit(0);
			}
		}
		
#endif

	}

	//Save simulation data into json file
	generate_json(info);

	//Clear screen and print simulation stats
	system("CLS");
	std::cout << endl << "Game: " << info.game_name << endl;
	std::cout << endl << "Play mode: " << info.mode << endl;
	stats.print_stats(stake);
}

void WildWhiteBison::convert_high_to_wilds()
{
#ifdef LOG_PROTO
	proto_screen = screen;
	for (auto& row : proto_screen) {
		std::fill(row.begin(), row.end(), -1);
	}
#endif
	//print_screen(config);
	for (int row = 0; row < info.number_of_rows; row++)
		for (int reel = 1; reel < 4; reel++)
			if (screen[row][reel] >= 1 && screen[row][reel] <= 4) { // if is a high symbol
				screen[row][reel] = WILD;
				int multiplier = config.w_wild_mult.sample(rng);
				converted_wilds.insert({ {row, reel}, multiplier });
#ifdef LOG_PROTO
				proto_screen[row][reel] = multiplier;
#endif // LOG_PROTO
			}

#ifdef LOG_PROTO
	proto_screen = parse_screen_for_proto(proto_screen);
	this_step_proto_play[proto_log_state]["convertedWildsScreen"] = proto_screen;
#endif
}

unsigned long long WildWhiteBison::play_free_spins()
{
	unsigned long long total_win = 0;
	total_multi = 0;

	bool bison_spin_fs;
	unsigned using_reel_set;
	int new_win = 0;
	int n_spins = config.n_free_spins;
	int total_n_spins = 0;
	int n_bison_spin = 0;


	while (n_spins) {
#ifdef LOG_PROTO
		whole_proto_play.push_back(this_step_proto_play);
		this_step_proto_play.clear();
		proto_log_state = "freeSpin";
#endif
		total_n_spins++;
		stats.n_spins_fs++;
		n_spins--;
		// reset spin multi
		new_multi = 0;
		new_win = 0;


		using_reel_set = config.w_reel_sets_fs.sample(rng);
		config.reel_sets[using_reel_set].spin(rng);

		// clear screen
		for (int row = 0; row < info.number_of_rows; row++)
			for (int reel = 0; reel < info.n_reels; reel++)
				if (!converted_wilds.contains({ row, reel })) // wild converted symbols become sticky
					screen[row][reel] = -1;

		drop_new_symbols(config.reel_sets[using_reel_set]);

		//check retrigger
		for (int row = 0; row < reels_heights[4]; row++) {
			int n_sc = 0;
			if (screen[row][4] == SCATTER) {
				n_spins += 1;
				n_sc++;
			}
			if (n_sc > 1) {
				std::cout << "ERROR: en el reel 4 del reel set " << using_reel_set << " hay mas de un scatter en los free spins" << endl;
				system("pause");
			}
		}


		bison_spin_fs = config.w_activate_bison_spin_fs.sample(rng);

		//Contar los s�mbolos especiales para imprimir carteles de alerta en caso que haya errores en la planilla o en el c�digo.
		int n_H_symbols = 0;
		for (int reel = 0; reel < nreels; reel++) {
			int n_sc = 0;
			for (int row = 0; row < reels_heights[reel]; row++) {
				if (screen[row][reel] == SCATTER && reel != 4) {
					std::cout << "ERROR: en los free spins, en el reel " << reel << " del reel set " << using_reel_set << " hay un scatter" << endl;
					system("pause");
					n_sc++;
				}
				if (screen[row][reel] > 0 && screen[row][reel] <= 4) {
					n_H_symbols++;
				}
				if (screen[row][reel] == S_SYMBOL) {
					std::cout << "ERROR: en los free spins, en el reel " << reel << " del reel set " << using_reel_set << " hay un super symbol" << endl;
				}
			}
		}

		if (bison_spin_fs && n_H_symbols == 0) {
			/*cout << "ERROR: est� el Bison Spin activo pero no hay ningun simbolo H para convertir en Wild" << endl;
			system("pause");*/
			bison_spin_fs = false;
		}

		if (bison_spin_fs) {
			convert_high_to_wilds();

			// stats
			n_bison_spin++;
		}
			

		add_multi_fs(config);
		
#ifdef LOG_PROTO
		// get winnings
		new_win += get_lines_win(config);
#else
		new_win += get_lines_win_fast_evaluation(config);
#endif
		

		// delete paid wilds
		vector<Coord> wilds_to_erase;
		for (const auto& wild : converted_wilds) {
			if (wild.second == 0)
				wilds_to_erase.push_back(wild.first);
		}
		for (const auto& wild : wilds_to_erase)
			if (converted_wilds.erase(wild) != 1) {
				std::cout << "ERROR: Habían mas de un wild con la misma clave o ninguno en el momento de limpiarlos en los fs" << endl;
				system("pause");
			}



		if (new_multi && new_win) {
			total_multi += new_multi;
		}

		if (total_multi && new_multi)
			total_win += new_win * total_multi;
		else
			total_win += new_win;


#ifdef LOG_PROTO // log proto
		proto_screen = parse_screen_for_proto(screen);
		this_step_proto_play[proto_log_state]["bisonSpinActive"] = bison_spin_fs;
		this_step_proto_play[proto_log_state]["remainingSpins"] = n_spins;
		this_step_proto_play[proto_log_state]["linesWin"] = new_win;
		this_step_proto_play[proto_log_state]["totalWin"] = total_win;
		this_step_proto_play[proto_log_state]["totalMulti"] = total_multi;
		this_step_proto_play[proto_log_state]["screen"] = proto_screen;
#endif
	}

	// Stats
	if (stats.multipliers_count.contains(total_multi))
		stats.multipliers_count[total_multi]++;
	else
		stats.multipliers_count[total_multi] = 1;


	if (stats.n_spins_fs_count.contains(total_n_spins))
		stats.n_spins_fs_count[total_n_spins]++;
	else
		stats.n_spins_fs_count[total_n_spins] = 1;

		if (stats.n_bison_spin_fs_count.contains(n_bison_spin))
		stats.n_bison_spin_fs_count[n_bison_spin]++;
	else
		stats.n_bison_spin_fs_count[n_bison_spin] = 1;

	

	return total_win;
}

void WildWhiteBison::add_multi_fs(WildWhiteBison_config& config)
{
	int prize;
	int n_wilds;


	for (int line = 0; line < config.paylines.lines.size(); line++) {
		n_wilds = 0;

		int s[6] = { 0,0,0,0,0,0 };
		for (int r = 0; r < config.n_reels; r++)
			s[r] = screen[config.paylines.lines[line][r]][r];



		prize = config.paytable_lines[s[0]][s[1]][s[2]][s[3]][s[4]][s[5]];

		if (prize)
			for (int reel = 0; reel < info.n_reels; reel++) {
				int row = config.paylines.lines[line][reel];
				if (s[reel] == WILD && converted_wilds.contains({ row, reel })) {
					new_multi += converted_wilds[{row, reel}];
					converted_wilds[{row, reel}] = 0; // add multi once
				}
			}
	}
}

void WildWhiteBison::drop_start_symbols(Reel_set& reel_set)
{
	for (unsigned reel = 0; reel < nreels; ++reel)
	{
		//Drop symbols at the bottom first...
		for (int row = (int)reels_heights[reel] - 1; row >= 0; row--)
		{
			//Move the stop one place up...(if the function get_screen was called then the symbol at "stop" was already used)
			reel_set.stops[reel] = (reel_set.stops[reel] + reel_set.reels_length[reel] - 1) % reel_set.reels_length[reel];
			screen[row][reel] = reel_set.reel_strips[reel][reel_set.stops[reel]];

		}
	}

	// remove extra symbols
	screen[3][0] = -1;
	screen[3][4] = -1;
}

unsigned long long WildWhiteBison::play_bonus()
{

	int super_symbols_count = 0;
	int spins = 3;
	int prize = 0;

	// limpiar pantalla y dejar los super symbols en su lugar
	for (int row = 0; row < info.number_of_rows; row++)
		for (int reel = 0; reel < info.n_reels; reel++)
			if (screen[row][reel] != S_SYMBOL)
				screen[row][reel] = -1;
			else
				super_symbols_count++;

#ifdef LOG_PROTO
	whole_proto_play.push_back(this_step_proto_play);
	this_step_proto_play.clear();
	proto_log_state = "bonusSpin";
	// log proto
	proto_screen = parse_screen_for_proto(screen);
	this_step_proto_play[proto_log_state]["remainingRespins"] = spins;
	this_step_proto_play[proto_log_state]["screen"] = proto_screen;
	whole_proto_play.push_back(this_step_proto_play);
#endif


	while (spins > 0) {
		spins--;
		stats.n_bonus_spins_count++;
		int n_symbols_at_spin = super_symbols_count;
		for (int reel = 0; reel < info.n_reels; reel++)
			for (int row = 0; row < reels_heights[reel]; row++)
				if (screen[row][reel] == S_SYMBOL)
					continue;
				else if (random(100) < config.prob_add_symbol.weights[n_symbols_at_spin]) {
					spins = 3; //reset spins
					super_symbols_count++;
					screen[row][reel] = S_SYMBOL;
				}
		
#ifdef LOG_PROTO
		this_step_proto_play.clear();
		// log proto
		proto_screen = parse_screen_for_proto(screen);
		this_step_proto_play[proto_log_state]["remainingRespins"] = spins;
		this_step_proto_play[proto_log_state]["prize"] = 0;
		this_step_proto_play[proto_log_state]["screen"] = proto_screen;
		whole_proto_play.push_back(this_step_proto_play);
#endif
	}

	if (super_symbols_count >= 15) {
		stats.super_symbols_count[super_symbols_count]++;
		prize = config.wheel_prizes.sample(rng) * bet_for_cap;
		// stats (hold and win desglose)
		if (prize != 10000 * bet_for_cap) {
			stats.wheel_win += prize;
			stats.wheel_hits++;
		} else {
			stats.jackpot_hits++;
			stats.jackpot_win += prize;
		}

	} else
		stats.super_symbols_count[super_symbols_count]++;

	if (!prize)
		prize = config.bonus_prizes.weights[super_symbols_count - 5] * bet_for_cap;

#ifdef LOG_PROTO
	this_step_proto_play.clear();
	// log proto
	proto_screen = parse_screen_for_proto(screen);
	this_step_proto_play[proto_log_state]["remainingRespins"] = spins;
	this_step_proto_play[proto_log_state]["prize"] = prize;
	this_step_proto_play[proto_log_state]["screen"] = proto_screen;
	whole_proto_play.pop_back(); // borro el ultimo estado de respins sin premio para que el estado final del bonus spin sea el que tenga premio
	whole_proto_play.push_back(this_step_proto_play); 
	this_step_proto_play.clear();
#endif

	

	return prize;
}