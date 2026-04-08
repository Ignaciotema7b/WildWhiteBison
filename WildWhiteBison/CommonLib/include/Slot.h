#pragma once
#include "Game.h"
#include "Slot_config.h"
#include "Game_info.h"
#include <unordered_set>
#include <stack>

using Coord = std::pair<int, int>;
using Cluster = std::vector<Coord>;
struct winning_Cluster {
	int symbol;
	Cluster cells;
	unsigned pays;
};

enum multiplier {
	additive,
	multiplicative
};



class Slot :
    public Game
{
public:


	void init(unsigned n_reels, unsigned n_rows, unsigned stake, unsigned n_symbols);
	void init(unsigned n_reels, unsigned n_trackers, unsigned n_rows, unsigned stake, unsigned n_symbols);

	/// <summary>
	/// Screen size is initialized by calling \p init method of \p config member. 
	/// If screen size is variable (for example Megaways) the number of rows and reels are the maximum possible.
	/// </summary>
	vector<vector<int>> screen;

	/// <summary>
	/// Visible part of the reels. 
	/// If number of rows are fixed this values won't change but if it is a Megaways style this values are used to indicate the number of rows at each moment. 
	/// </summary>
	vector<unsigned> reels_heights;

	/// <summary>
	/// Fills in the \p screen with symbols from \p reel_set. It uses \p reels_heights in this class and \p stops from \p reel_set
	/// to pick the symbols to be shown.  
	/// </summary>
	/// <param name="reel_set">Reel set</param>
	void get_screen(Reel_set& reel_set);


	/// <summary>
	/// Prints screen in console with symbols IDs.
	/// </summary>
	void print_screen();

	/// <summary>
	/// Prints screen in console with symbols short names.
	/// </summary>
	void print_screen(Slot_config& config);

	/// <summary>
	/// Returns a string with the symbols on screen in JSON format.
	/// </summary>
	/// <param name="config"></param>
	/// <returns></returns>
	string log_screen(Slot_config& config);

	/// <summary>
	/// Returns a string with the symbols on screen in JSON format for new PP format.
	/// </summary>
	/// <param name="config"></param>
	/// <returns></returns>
	string log_screen(Slot_config& config, int spin);


	unsigned get_ways_win(Slot_config &config);

	/// <summary>
	/// Calcula el pago total de ways con multiplicadores por reel (equivalente a tener mas ways)
	/// </summary>
	/// <param name="config"></param>
	/// <param name="multipliers"></param>
	/// <returns></returns>
	unsigned get_ways_win(Slot_config& config, vector<unsigned> multipliers);


	/// <summary>
	/// Return the index of prize being paid in line_number. If not prize is won then returns -1.
	/// </summary>
	/// <param name="config"></param>
	/// <param name="line_number"></param>
	/// <returns></returns>
	int get_line_win(Slot_config& config, int line_number);

	/// <summary>
	/// Each symbol on screen can reveal a multiplier that is used in the prizes where it is involved.
	/// </summary>
	vector<vector<unsigned>> symbol_multipliers;

	/// <summary>
	/// Returns the total win amount evaluating all the paylines.
	/// </summary>
	/// <param name="config">Configuration, it contains paylines, symbols, paytable, etc.</param>
	/// <returns></returns>
	unsigned get_lines_win(Slot_config& config);

	unsigned get_lines_win_fast_evaluation(Slot_config& config);

	unsigned get_lines_win_with_multiplier(Slot_config& config, multiplier type_of_multiplier );

	/// <summary>
	/// WIP
	/// </summary>
	/// <param name="config"></param>
	void print_theoretical_rtp_by_reel_sets(Slot_config& config);


	//TODO: add logic for wild symbols
	unsigned get_ways_win_R2L(Slot_config &config);
	
	/// <summary>
	/// Returns total win evaluating ways from left to right and from right to left. If a symbol is present in all reels then the prize x of a kind displayed in paytable is awarded only once.
	/// </summary>
	/// <param name="config"></param>
	/// <returns></returns>
	unsigned get_ways_win_both(Slot_config &config);

	/// <summary>
	/// Used for "scattered" pay games. This function admits at most 1 wild symbol defined in \p config
	/// </summary>
	/// <param name="config"></param>
	/// <returns></returns>
	unsigned get_scatter_wins(Slot_config& config);

	/// <summary>
	/// Sets all elements in symbol_multipliers to 1 (symbol_multipliers[i][j] = 1 for all i, j)
	/// </summary>
	void reset_symbol_multipliers();

	vector<Cluster> findClusters(Slot_config& config);

	vector<winning_Cluster> get_winning_clusters(Slot_config& config);

protected:

	/// <summary>
	/// Number of symbols by reel. Position \p (i,j) is the amount of symbols \p i in reel \p j.
	/// </summary>
	vector<vector<unsigned>>  screen_symbols_count;

	/// <summary>
	/// Total number of each symbol on screen. It can count wilds as any symbol that they replace.
	/// Each wild symbol is also counted as every symbol that it replaces.
	/// </summary>
	unsigned symbols_count[MAX_N_SYMBOLS];

	/// <summary>
	/// Counts number of ocurrences of symbol \p symbolID on screen (scattered).
	/// </summary>
	/// <param name="symbolID"></param>
	/// <returns></returns>
	unsigned count_scatters(unsigned symbolID);

	/// <summary>
	/// This function updates the values in \ref screen_symbols_count
	/// </summary>
	virtual void count_symbols_on_screen();

	/// <summary>
	/// TODO: add description
	/// </summary>
	void explode_symbols_ways(Slot_config& config);

	/// <summary>
	/// TODO: add description
	/// </summary>
	/// <param name="config"></param>
	void explode_symbols_scatter_pays(Slot_config& config);

	/// <summary>
	/// TODO: add description: llena los lugares vacios con simbolos desde stop+1 salteando los lugares que ya estuvieran ocupados.
	/// Actualiza 
	/// </summary>
	/// <param name="reel_set"></param>
	void drop_new_symbols(Reel_set& reel_set);

	/// <summary>
	/// Sets all elements in screen to -1 (screen[i][j] = -1 for all i, j)
	/// </summary>
	void clear_screen();


	/// <summary>
	/// Replaces all symbols \p old_id on screen by a new ID \p new_id
	/// </summary>
	/// <param name="old_id"></param>
	/// <param name="new_id"></param>
	void rename_symbols_on_screen(int old_id, int new_id);

protected:
	/// <summary>
	/// Reels where trackers start (\p trackers_reels[0] ) and where trackers finish (\p trackers_reels[1] )
	/// </summary>
	unsigned trackers_reels[2];

	/// <summary>
	/// Once symbols have exploded leaving empty spaces, this method move symbols to the bottom of screen.
	/// Symbols in trackers are moved to the left.
	/// </summary>
	void cascade();

	/// <summary>
	/// TODO: add description
	/// </summary>
	/// <param name="config"></param>
	/// <param name="index"></param>
	void generate_all_symbols_combinations(Slot_config& config, int index);

	unsigned nreels = 0;
	unsigned nrows = 0;
	unsigned ntrackers = 0;

#ifdef LOG_PROTO
	json this_step_proto_play;
	vector<vector<int>> parse_screen_for_proto(vector<vector<int>> screen);
	vector<int> parse_top_tracker_for_proto(vector<vector<int>> screen);

#endif
	string proto_log_state; //"cascade", "basegame","freespin"   lo dejo afuera porque no jode, y me evito ifdefs


private:

	//Direction for cell's neighbors. dx and dy must be ordered and the pair (dx[i], dy[i]) is a direction for a 
	vector<int> dx_8 = { -1, -1, -1, 0, 0, 1, 1, 1 };
	vector<int> dy_8 = { -1, 0, 1, -1, 1, -1, 0, 1 };

	vector<int> dx = { -1, 0,  0, 1 };
	vector<int> dy = { 0, -1, 1, 0 };

	//To use by function findClusters
	vector<vector<bool>> visited;
	vector<vector<bool>> cluster_visited;

	//Depth first search
	void dfs(Slot_config& config, vector<vector<bool>>& visited, int x, int y, int symbol_ID, Cluster& cluster);
};

