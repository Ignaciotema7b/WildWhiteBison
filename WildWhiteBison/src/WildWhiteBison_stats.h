#pragma once
#include "../CommonLib/include/Game_statistics.h"

class WildWhiteBison_stats :
    public Game_statistics
{
public:
    json stats_to_json(unsigned stake);

    void print_stats(unsigned apuesta);

    unsigned long long base_win = 0;
    unsigned long long bison_spin_win = 0;
    unsigned long long bonus_win = 0;
    unsigned long long free_spins_win = 0;

    unsigned long long bonus_hits = 0;
    unsigned long long bison_spin_hits = 0;
    unsigned long long free_spins_hits = 0;

    array<int, 19> super_symbols_count{0};
	map<int, int> multipliers_count;
    map<int, int> n_spins_fs_count;
	array<int, 4> n_scatters_count{ 0 };
	array<int, 8> n_ss_count{ 0 };
	unsigned long long n_bonus_spins_count = 0;
	map<int, int> n_bison_spin_fs_count;
    
    unsigned long long n_spins_fs = 0;

    unsigned long long hits_cap = 0;

    // hold and win desglose
	unsigned long long wheel_win = 0;
	unsigned long long jackpot_win = 0;
	unsigned long long wheel_hits = 0;
	unsigned long long jackpot_hits = 0;
    

    //Coin distribution
    Win_and_RTP_distribution win_distr_base = Win_and_RTP_distribution("Base Game");
    Win_and_RTP_distribution win_distr_bison_spin = Win_and_RTP_distribution("Bison Spin");
    Win_and_RTP_distribution win_distr_fg = Win_and_RTP_distribution("Free Games");
    Win_and_RTP_distribution win_distr_bonus = Win_and_RTP_distribution("Hold & Spin");


};

