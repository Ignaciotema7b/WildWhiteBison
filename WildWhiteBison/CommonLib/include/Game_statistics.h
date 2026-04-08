#pragma once
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Win_and_RTP_distribution.h"
#include "nlohmann/json.hpp"

#define PAYOUTS_LIST_SIZE 100000
#define MAX_NUMBER_OF_PRIZES 100

using namespace std;
using json = nlohmann::json;

/**
 * @brief Basic statistics for all kind of games.
 *
 * @author Pablo Delieutraz.
 * @date August 2021.
 */
class Game_statistics
{
public:
	Game_statistics();
	~Game_statistics();

	/// <summary>
	/// @brief Number of plays performed in the Monte Carlo simulation.
	/// </summary>
	unsigned long long int plays = 0;
	/// <summary>
	/// @brief Number of hits in the Monte Carlo simulation, i.e. number of plays with positive winnings.
	/// </summary>
	unsigned long long int hits = 0;
	/**
	 * \~english
	 * @brief Total credits won in the Monte Carlo simulation applying the cap in every play where the payout exceeds the cap. 
	 * \~spanish
	 * @brief Total de cr&eacute;ditos ganados en la simulacion Monte Carlo aplicando el cap en cada jugada donde el pago excede al cap.
	 */
	unsigned long long int total_payout = 0;

	/**
	 * \~english
	 * @brief Total credits won in the Monte Carlo simulation. There is no limit in amount won in each play.
	 * \~spanish
	 * @brief Total de cr&eacute;ditos ganados en la simulacion Monte Carlo. No hay l&iacute;mites en el pago de cada jugada.
	*/
	unsigned long long total_payout_without_cap = 0;

	/// <summary>
	/// @brief Maximum payout in credits won in a single play.
	/// </summary>
	unsigned long long int max_payout = 0;


	/// <summary>
	///  @brief \f$\sum_{i=1}^{n} x_i^2 \f$, where \f$n\f$ is the number of plays and \f$x_i\f$ is the amount won in the \f$i\f$-th play.
	/// 
	/// This value is used to calculate the standard deviation. 
	/// </summary>
	//unsigned long long int total_payout_sq = 0;
	double total_payout_sq = 0;

	/// <summary>
	/// @brief Number of hits for each prize in base game. RTP by symbol can be calculated using these values.
	/// </summary>
	unsigned long long prize_hits_base[MAX_NUMBER_OF_PRIZES];
	/// <summary>
	/// @brief Number of hits for each prize in free spins. RTP by symbol can be calculated using these values.
	/// </summary>
	unsigned long long prize_hits_fs[MAX_NUMBER_OF_PRIZES];


	/// <summary>
	/// Prints basic statistics to console.
	/// </summary>
	/// <param name=""></param>
	void print_stats(unsigned stake);


	/// <summary>
	/// This method must be called after every play. It counts plays, total payout, hits, etc.
	/// </summary>
	/// <param name="win"> total prize won in current play</param>
	/// <param name="stake"> total bet amount</param>
	void count_stats_play(unsigned long long int win, unsigned stake);

	/**
 * @brief
 * @param stake
 * @return
 */
	void print_stats_WO_coin_out(unsigned stake);
	void count_stats_play(unsigned long long int win, bool is_fs);


	/// <summary>
	/// Prints exact coin out distribution to console.
	/// </summary>
	/// <param name=""></param>
	void print_coinout();

	/// <summary>
	/// Resets all fields
	/// </summary>
	void reset();

	/**
	 * @brief 
	 * @return 
	 */
	unsigned max_win_hits();

	vector<vector<unsigned long long>> get_wins_hits();

	double standard_deviation(unsigned stake);

	void count_stats_play(unsigned long long int win, unsigned stake, unsigned using_reelset);

	void print_rtp_n_this_per_reelset();

	json stats_to_json(unsigned stake);

	void test_rtp_confidence_95(double rtp_target, unsigned stake);
	
	//Session stats
	unsigned long starting_balance;
	unsigned long current_balance;

	unsigned long long total_sessions = 0;
	unsigned long long sessions_loosing = 0;
	unsigned long long sessions_duplicating = 0;


private:

	/*unsigned n_ranks = 10;
	double ranks[10] = { 0,0.25,0.5,1,2,5,10,20,50,100 };
	unsigned wins[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; //wins[i]= hits between ranks[i-1] and ranks[i] 1<i<n-1

	double rtp_distribution[11] = { 0,0,0,0,0,0,0,0,0,0,0 };*/

	unsigned n_ranks = 12;
	double ranks[12] = { 0,1,2,5,10,20,30,50,100,200,500,1000 };
	//double ranks[12] = { 0,1,2,5,10,25,50,100,200,500,100000000,200000000 };
	unsigned wins[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; //wins[i]= hits between ranks[i-1] and ranks[i] 1<i<n-1
	double rtp_distribution[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };

	//Print win distribution 
	void print_ranks(double rank[], unsigned size, unsigned wins[], unsigned long long hits, double rtp_distribution[], unsigned long long win, const char title[]);


	//Exact win distribution:
	unsigned long long non_winning_plays = 0;

	unsigned long long non_winning_plays_no_fs = 0;
	unsigned long long non_winning_plays_fs = 0;

	/// <summary>
	/// It stores in position \p i the number of plays having a total win equals to \p i credits.
	/// 
	/// The limit is 99999 credits. Payouts greater than this limit are stored in another vector.
	/// </summary>
	unsigned coinout[PAYOUTS_LIST_SIZE];

	unsigned coinout_no_fs[PAYOUTS_LIST_SIZE];
	unsigned coinout_fs[PAYOUTS_LIST_SIZE];

	vector<unsigned long long> bigger_wins;

	vector<unsigned long long> bigger_wins_no_fs;
	vector<unsigned long long> bigger_wins_fs;

	//Prints coin distribution with intervals by default.
	void print_win_distr(void);

	//Calculate some stats
	void add_win_distr_value(double win, unsigned stake);

	vector <unsigned long long > total_win_by_reelset;
	vector <unsigned long long > total_hits_by_reelset;


	vector<vector<unsigned long long>> summary_big_wins();

	vector<vector<unsigned long long>> summary_big_wins_no_fs();
	vector<vector<unsigned long long>> summary_big_wins_fs();

	vector<vector<unsigned long long>> summary_coinout();

	vector<vector<unsigned long long>> summary_coinout_no_fs();
	vector<vector<unsigned long long>> summary_coinout_fs();
	
};

