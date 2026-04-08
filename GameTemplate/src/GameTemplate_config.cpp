#include "GameTemplate_config.h"

void GameTemplate_config::init(Game_info info)
{
	Slot_config::init(info);


	// Reel sets
	w_reel_sets_main.init(info.folder_config + "\\w_reel_sets_main.csv");


	//JSON
	//Reel set
	json_config["w_reel_sets_main"] = w_reel_sets_main.to_json();
}
