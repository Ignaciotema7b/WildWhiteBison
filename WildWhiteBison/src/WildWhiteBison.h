#pragma once
#include "../CommonLib/include/Slot.h"
#include "../CommonLib/include/rnd_log.h"
#include "WildWhiteBison_config.h"
#include "WildWhiteBison_stats.h"

class WildWhiteBison :
    public Slot
{
public:
    void generate_json(Game_info info);
    WildWhiteBison_config config;
    WildWhiteBison_stats stats;

    void init(Game_info info);

    void play(unsigned long long plays);

    
#ifdef LOG_PLAYS
    //Random logs
    rnd_log randomLogs;
#endif


private:

    unsigned bet_for_cap;
    unsigned plays_to_print;

};

