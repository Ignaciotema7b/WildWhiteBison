#include "WildWhiteBison_config.h"

void WildWhiteBison_config::init(Game_info info)
{
	Slot_config::init(info);

	// Play mode
	play_mode.init(info.folder_config + "\\play_mode.csv");

	//w reel sets 
	w_reel_sets_main[0].init(info.folder_config + "\\w_reel_sets_main.csv");
	w_reel_sets_main[1].init(info.folder_config + "\\w_reel_sets_main_bison_spin.csv");
	w_reel_sets_main[2].init(info.folder_config + "\\w_reel_sets_main_h_y_s.csv");

	w_reel_sets_fs.init(info.folder_config + "\\w_reel_sets_fs.csv");

	//Activate features
	w_activate_bison_spin.init(info.folder_config + "\\w_activate_bison_spin.csv");
	w_activate_bison_spin_fs.init(info.folder_config + "\\w_activate_bison_spin_fs.csv");
	w_activate_HyS.init(info.folder_config + "\\w_activate_HyS.csv");

	w_wild_mult.init(info.folder_config + "\\w_wild_mult.csv");

	// Hold and spin
	prob_add_symbol.init(info.folder_config + "\\prob_add_symbol.csv");
	wheel_prizes.init(info.folder_config + "\\wheel_prizes.csv");
	bonus_prizes.init(info.folder_config + "\\bonus_prizes.csv");

	add_super_symbols.init(info.folder_config + "\\add_super_symbols.csv");


	Weighted_values fs;
	fs.init(info.folder_config + "\\n_free_spin.csv");
	n_free_spins = fs.values[0];

	//JSON
	//Reel set
	json_config["w_reel_sets_main"] = w_reel_sets_main[0].to_json();
	json_config["w_reel_sets_main_bison_spin"] = w_reel_sets_main[1].to_json();
	json_config["w_reel_sets_main_bonus"] = w_reel_sets_main[2].to_json();
	json_config["w_reel_sets_fs"] = w_reel_sets_fs.to_json();
	

	json_config["w_activate_bison_spin"] = w_activate_bison_spin.to_json();
	json_config["w_activate_bison_spin_fs"] = w_activate_bison_spin_fs.to_json();
	json_config["w_activate_HyS"] = w_activate_HyS.to_json();

	json_config["w_wild_mult"] = w_wild_mult.to_json();

	json_config["prob_add_symbol"] = prob_add_symbol.to_json();
	json_config["wheel_prizes"] = wheel_prizes.to_json();
	json_config["bonus_prizes"] = bonus_prizes.to_json();

	json_config["add_super_symbols"] = add_super_symbols.to_json();
	json_config["n_free_spins"] = n_free_spins;


	json_config["play_mode"] = play_mode.to_json();

}
