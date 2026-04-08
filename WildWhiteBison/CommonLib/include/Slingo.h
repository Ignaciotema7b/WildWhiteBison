#pragma once
#include "Game.h"
#include "Reel_set.h"
#include "Weighted_values.h"
#include "Game_info.h"
#include "Symbol.h"

class Slingo :
    public Game
{
public:

	void init(unsigned n_reels,unsigned n_rows, unsigned stake, unsigned spins_base, unsigned extra_spins, string config_folder);

	void init(Game_info info);

	void play(unsigned long long plays);
	unsigned n_reels = 0;
	unsigned n_rows = 0;

	unsigned long slingo_numbers = 75;

	unsigned spins_standard;
	unsigned spins_purchase;

	unsigned joker_id;
	unsigned super_joker_id;
	unsigned free_spin_id;

	/// <summary>
	/// Screen size is initialized by calling \p init method of \p config member.
	/// </summary>
	vector<vector<int>> screen;

	/// <summary>
	/// Card size is initialized by calling \p init method of \p config member.
	/// </summary>
	vector<vector<int>> card_numbers;

	vector<Symbol> symbols_list;

	/// <summary>
	/// Screen size is initialized by calling \p init method of \p config member.
	/// </summary>
	vector<int> count_lines_by_marks;

	Reel_set reel_set_base;
	Reel_set reel_set_purchase;

	vector<int> spin_results;
	vector<bool> jokers;
	vector<bool> super_jokers;
	
	/// <summary>
	/// Spins reels and get results into \p spin_results
	/// </summary>
	/// <param name="reel_set"></param>
	void spin_reels(Reel_set &reel_set);

	/// <summary>
	/// Uses optimal strategy to place jokers and super joker in spin_results array
	/// </summary>
	void optimal_strategy();

	void reset_screen();

private:
	//Load symbols from file "symbols.csv" in folder "config_folder"
	void load_symbols(string config_folder);
};

