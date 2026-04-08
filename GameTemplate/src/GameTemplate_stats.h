#pragma once
#include "../CommonLib/include/Game_statistics.h"

class GameTemplate_stats :
    public Game_statistics
{
public:
    json stats_to_json(unsigned stake);
    
    void print_stats(unsigned apuesta);
    
   

};

