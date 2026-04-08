#pragma once
#include "../CommonLib/include/Slot_config.h"


class WildWhiteBison_config :
    public Slot_config

{
public:
    void init(Game_info info);

    Weighted_values w_reel_sets_main;;

};

