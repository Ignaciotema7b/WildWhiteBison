#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "nlohmann/json.hpp"

#define PAYOUTS_LIST_SIZE 100000

using namespace std;
using json = nlohmann::json;

class Win_and_RTP_distribution
{
public:
	Win_and_RTP_distribution(string name);

	string feature_name;

	
	json to_json();


	void count_stats_play(unsigned long long int win, unsigned stake);

	//Prints coin distribution with intervals by default.
	void print_win_distr(void);


	void print_hits_by_ranges(vector<double> intervals, unsigned stake, bool open_closed);

	//TODO: no calcula con detalle el punto "medio" exacto
	unsigned median();

private:
	json json_win_distr;
	unsigned n_ranks = 12;
	double ranks[12] = { 0,1,2,5,10,20,30,50,100,200,500,1000 };
	unsigned wins[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; //wins[i]= hits between ranks[i-1] and ranks[i] 1<i<n-1

	double rtp_distribution[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };

	//Print win distribution 
	void print_ranks(double rank[], unsigned size, unsigned wins[], unsigned long long hits, double rtp_distribution[], unsigned long long win);

	//Calculate some stats
	void add_win_distr_value(double win, unsigned stake);

	//Exact win distribution:
	unsigned long long non_winning_plays = 0;

	/// <summary>
	/// It stores in position \p i the number of plays having a total win equals to \p i credits.
	/// 
	/// The limit is 99999 credits. Payouts greater than this limit are stored in another vector.
	/// </summary>
	unsigned coinout[PAYOUTS_LIST_SIZE];

	vector<unsigned> bigger_wins;

	unsigned long long total_hits = 0;
	unsigned long long total_win = 0;

	/// <summary>
	///  @brief \f$\sum_{i=1}^{n} x_i^2 \f$, where \f$n\f$ is the number of plays and \f$x_i\f$ is the amount won in the \f$i\f$-th play.
	/// 
	/// This value is used to calculate the standard deviation. 
	/// </summary>
	unsigned long long int total_payout_sq = 0;

};

