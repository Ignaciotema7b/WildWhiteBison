#pragma once
#include <random>
#include <vector>
#include <fstream> 
#include <string>

using namespace std;

/**
 * @brief Class to perform reels' spin.
 *
 *
 * @author Pablo Delieutraz.
 * @date August 2021.
 */
class Reel_set
{
public:

	/**
	 * Initialize reel strips, stops, etc.
	 * 
	 * \param n_reels number of reels in this reel set
	 * \param file_path file containing the reel strips. The elements in strips must be the ID of the symbol.
	 */
	void init(int n_reels, string file_path);

	/**
	 * Initialize reel strips, stops, etc.
	 *
	 * \param n_reels number of reels in this reel set
	 * \param file_path file containing the reel strips and trackers. The elements in strips must be the ID of the symbol.
	 */
	void init(int n_reels, int n_trackers, string file_path);

	/**
	 * Reel strip formed by symbols IDs.
	 */
	vector<vector<int>> reel_strips;

	/**
	 * Reel strip formed by symbols IDs.
	 */
	vector<vector<int>> trackers_strips;


	/**
	* Reel strips formed by short name of symbols.
	* It includes trackers.
	*/
	vector<vector<string>> reel_strips_short_name;

	/**
	 * Vector of stops of each reel. 
	 */
	vector<unsigned> stops;

	/**
	 * Vector of stops of each reel.
	 */
	vector<unsigned> stops_trackers;

	/**
	 * Updates \p stops with random values. The stop of reel \p i will be between 0 and L - 1, where L is the length of reel \p i.
	 * 
	 */
	void spin(mt19937_64 &rng);

	void force_stops(vector<unsigned> stops_param);

	int calculate_min_dist(int symbol, int reel);


	vector<unsigned> reels_length;
	vector<unsigned> trackers_length;

	string name;

	/// <summary>
	/// It returns a string to log the stops of reels. The first stops are the reel stops and the last is the stop of tracker (if there is a tracker).
	/// </summary>
	/// <returns>String. For example if it is a 3-reel game it return the stops in the following format: "ST:stop:length,ST:stop:length,ST:stop:length" </returns>
	string log_stops();


	vector<vector<unsigned>> symbols_distribution;
	void count_symbols_by_reel(int number_of_symbols);


	int number_of_reels = 0;
	int number_of_trackers = 0;

};

