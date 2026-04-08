#pragma once
#include "Symbol.h"
#include "Paylines.h"
#include "Reel_set.h"
#include "Prize.h"
#include "Weighted_values.h"
#include "Game_info.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

#define MAX_N_SYMBOLS 20

class Slot_config
{
public:

	/// <summary>
	/// Loads symbols list, paytable, sets number of reels and max number of rows.
	/// </summary>
	/// <param name="info"></param>
	void init(Game_info info);

	/// <summary>
	/// Loads symbols list, paytable, sets number of reels and max number of rows.
	/// </summary>
	/// <param name="n_reels_param"></param>
	/// <param name="n_trackers_param"></param>
	/// <param name="n_rows_param"></param>
	void init(unsigned n_reels_param, unsigned n_trackers_param, unsigned n_rows_param, string config_folder);

	/// <summary>
	/// Loads symbols list, paytable, sets number of reels and max number of rows.
	/// </summary>
	/// <param name="n_reels_param"></param>
	/// <param name="n_rows_param"></param>
	void init(unsigned n_reels_param, unsigned n_rows_param, string config_folder);

	unsigned n_reels = 0;
	unsigned n_rows = 0;
	unsigned n_trackers = 0;

	unsigned bet_per_line = 1;  //By default bet per line = 1 credit


	vector<Symbol> symbols_list;
	vector<unsigned> wild_symbols;

	//Reel sets
	vector<Reel_set> reel_sets;
	

	/// <summary>
	/// Row \p i column \p j is the prize for \p j of a kind of symbol \p i.
	/// </summary>
	vector<vector<unsigned>> paytable;

	/// <summary>
	/// List of prizes obejects, sorted by payout desc.
	/// </summary>
	vector<Prize> prizes_list;

	//Fast line evaluation
	unsigned short paytable_lines[MAX_N_SYMBOLS][MAX_N_SYMBOLS][MAX_N_SYMBOLS][MAX_N_SYMBOLS][MAX_N_SYMBOLS][MAX_N_SYMBOLS];

	/// <summary>
	/// If it pays by paylines...
	/// </summary>
	Paylines paylines;

	/// <summary>
	/// TODO: este deberia ser un metodo de la clase Reel_set (o no?)
	/// </summary>
	void print_dist_symbols();

	/// <summary>
	/// Row \p i column \p j is true if symbol \p i substitutes for symbol \p j to evaluate wins. If wild ID is \p i then most elements in row \p i will be "true".
	/// </summary>
	bool substitutes_for[MAX_N_SYMBOLS][MAX_N_SYMBOLS];


	/// <summary>
	/// Type of game: Lines, Ways, Scatter pays, Clusters, Clusters (8 way)
	/// </summary>
	string type;

	json json_config;

protected:
	//Load reel set from files "reel_set_n.csv" in folder "config_folder" (n is reel set ID starting from 0)
	void load_reel_sets(vector<Reel_set>& reel_sets, string config_folder);

	string config_folder;

private:
	//Load symbols from file "symbols.csv" in folder "config_folder"
	void load_symbols( string config_folder);

	//Load wild substitutions
	void load_wild_substitutions();

	//Load paytable from file "paytable.csv" in folder "config_folder"
	virtual void load_paytable(string config_folder);

	//Load paytable for scatter pays games
	void load_paytable_scatter_pays(Game_info info);

	//Load symbols_distr in reel_sets
	void count_symbols_distr();

	//Loads prizes list and sorts by payout descendent
	void load_prizes_list(string config_folder);

	//Por ahora no los uso pero los valores verdaderos se estan cargando con la funcion load_reel_sets
	unsigned total_number_base_reel_set = 0;
	unsigned total_number_fs_reel_set = 0;


};

