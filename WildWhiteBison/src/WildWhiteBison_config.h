#pragma once
#include "../CommonLib/include/Slot_config.h"

class WildWhiteBison_config :
    public Slot_config

{
public:
    void init(Game_info info);

    // Play mode
    Weighted_values play_mode;

    // Reel Sets
    array<Weighted_values, 3> w_reel_sets_main;
    Weighted_values w_reel_sets_fs;

    //Activate features
    Weighted_values w_activate_bison_spin;
    Weighted_values w_activate_bison_spin_fs;
    Weighted_values w_activate_HyS;

    // Wild multipliers
    Weighted_values w_wild_mult;

    // Hold and spin
    Weighted_values prob_add_symbol;
    Weighted_values wheel_prizes;
    Weighted_values bonus_prizes;

    Weighted_values add_super_symbols;

    int n_free_spins;

};

