#pragma once
#include "../CommonLib/include/Slot.h"
#include "../CommonLib/include/rnd_log.h"
#include "GameTemplate_config.h"
#include "GameTemplate_stats.h"

class GameTemplate :
    public Slot
{
public:
    void generate_json(Game_info info);
    GameTemplate_config config;
    GameTemplate_stats stats;

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

