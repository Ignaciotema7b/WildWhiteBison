#pragma once
#include "../CommonLib/include/Slot.h"
#include "../CommonLib/include/rnd_log.h"
#include "WildWhiteBison_config.h"
#include "WildWhiteBison_stats.h"
#include <set>

class WildWhiteBison :
    public Slot
{
public:
    void generate_json(Game_info info);
    WildWhiteBison_config config;
    WildWhiteBison_stats stats;

    unsigned long long base_win;
    unsigned long long total_multi;
    unsigned long long new_multi;
    void convert_high_to_wilds();
    unsigned long long play_free_spins();
    unsigned long long play_bonus();
    void add_multi_fs(WildWhiteBison_config& config);
    map<Coord, int> converted_wilds;
    vector<int> reels_heights_local = { 3, 4, 4, 4, 3 };

    void init(Game_info info);

    void play(unsigned long long plays);

    void drop_start_symbols(Reel_set& reel_set);


private:

    unsigned bet_for_cap;
    unsigned plays_to_print;

#ifdef LOG_PROTO
#define PROTO_BASE_PLAYS 10000
#define PROTO_FGS_PLAYS 2000
#define PROTO_BONUS_PLAYS 5000
#define PROTO_BONUS_BUY_PLAYS 5000

    vector<json> all_proto_play;
    vector<json> all_fgs_proto_play;
    vector<json> all_bonus_proto_play;
    vector<json> all_bb_proto_play;

    vector<json> whole_proto_play; // un play es un vector que tiene el base y los fgs ;

    int json_proto_count = 0;
    int json_proto_bonus_count = 0;
    int json_proto_fgs_count = 0;
    int json_proto_bb_count = 0;
    int proto_logs_ready = 0;

    int total_win_this_spin = 0;
    int proto_remaining_fs = -1;

    bool proto_fgs_triggered = false;
    bool proto_last_cascade = false;
    int proto_n_scatters = 0;

    vector<vector<int>> proto_screen;

#endif // LOG_PROTO


};
