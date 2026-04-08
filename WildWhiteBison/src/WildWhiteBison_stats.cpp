#include "WildWhiteBison_stats.h"

json WildWhiteBison_stats::stats_to_json(unsigned stake)
{
	json resp;
	resp = Game_statistics::stats_to_json(stake);
	resp["base_win"] = base_win;
	resp["bison_spin_win"] = bison_spin_win;
	resp["bonus_win"] = bonus_win;
	resp["free_spins_win"] = free_spins_win;

	resp["bonus_hits"] = bonus_hits;
	resp["bison_spin_hits"] = bison_spin_hits;
	resp["free_spins_hits"] = free_spins_hits;

	resp["super_symbols_count"] = super_symbols_count;
	resp["multipliers_count"] = multipliers_count;
	resp["n_fs_count"] = n_spins_fs_count;
	resp["n_scatters_count"] = n_scatters_count;
	resp["n_ss_count"] = n_ss_count;
	resp["n_bonus_spins_count"] = n_bonus_spins_count;
	resp["n_bison_spin_fs_count"] = n_bison_spin_fs_count;

	resp["n_spins_fs"] = n_spins_fs;

	resp["wheel_win"] = wheel_win;
	resp["jackpot_win"] = jackpot_win;
	resp["wheel_hits"] = wheel_hits;
	resp["jackpot_hits"] = jackpot_hits;

	resp["hits_cap"] = hits_cap;
	resp["win_distr_base"] = win_distr_base.to_json();
	resp["win_distr_fg"] = win_distr_fg.to_json();
	resp["win_distr_bison_spin"] = win_distr_bison_spin.to_json();
	resp["win_distr_bonus"] = win_distr_bonus.to_json();
	return resp;
}

void WildWhiteBison_stats::print_stats(unsigned apuesta)
{

	Game_statistics::print_stats(apuesta);

	cout << "Frecuencia del CAP: " << plays * 1.0 / hits_cap << endl;
	cout << endl;

	// Imprimir encabezado de la tabla
	std::cout << std::setw(15) << ""
		<< std::setw(15) << "RTP"
		<< std::setw(15) << "AvgPay"
		<< std::setw(15) << "Freq" << std::endl;

	// Imprimir líneas de separación
	std::cout << std::setfill('-') << std::setw(60) << "" << std::setfill(' ') << std::endl;


	// Base game
	std::cout << std::setw(15) << "Base game"
		<< std::setw(15) << std::fixed << (base_win) * 1.0 / plays / apuesta
		<< std::setw(15) << (base_win) * 1.0 / (plays - bison_spin_hits) / apuesta
		<< std::setw(15) << plays * 1.0 / (plays - bison_spin_hits) << std::endl;

	// Bison spin
	std::cout << std::setw(15) << "Bison spin"
		<< std::setw(15) << (bison_spin_win) * 1.0 / plays / apuesta
		<< std::setw(15) << (bison_spin_win) * 1.0 / (bison_spin_hits) / apuesta
		<< std::setw(15) << plays * 1.0 / (bison_spin_hits) << std::endl;

	// Hold and Win
	std::cout << std::setw(15) << "Hold and Spin"
		<< std::setw(15) << (bonus_win) * 1.0 / plays / apuesta
		<< std::setw(15) << (bonus_win) * 1.0 / (bonus_hits) / apuesta
		<< std::setw(15) << plays * 1.0 / (bonus_hits) << std::endl;

	// Free spins
	std::cout << std::setw(15) << "Free spins"
		<< std::setw(15) << (free_spins_win) * 1.0 / plays / apuesta
		<< std::setw(15) << (free_spins_win) * 1.0 / (free_spins_hits) / apuesta
		<< std::setw(15) << plays * 1.0 / (free_spins_hits) << std::endl;
	cout << endl;

	std::cout << std::setw(15) << "Total RTP:"
		<< std::setw(15) << (bison_spin_win + bonus_win + free_spins_win + base_win) * 1.0 / plays / apuesta << endl;
	cout << endl;
	cout << endl;

	cout << "Hold and win desglose: " << endl;

	std::cout << std::setw(15) << ""
		<< std::setw(15) << "RTP"
		<< std::setw(15) << "AvgPay"
		<< std::setw(15) << "Freq" << std::endl;
	cout << endl;

	// 1 in a Million
	std::cout << std::setw(15) << "1 in a Million"
		<< std::setw(15) << std::fixed << (bonus_win - wheel_win - jackpot_win) * 1.0 / plays / apuesta
		<< std::setw(15) << (bonus_win - wheel_win - jackpot_win) * 1.0 / (bonus_hits - wheel_hits - jackpot_hits) / apuesta
		<< std::setw(15) << plays * 1.0 / (bonus_hits - wheel_hits - jackpot_hits) << std::endl;

	// Super Wheel
	std::cout << std::setw(15) << "Super Wheel"
		<< std::setw(15) << std::fixed << (wheel_win) * 1.0 / plays / apuesta
		<< std::setw(15) << (wheel_win) * 1.0 / (wheel_hits) / apuesta
		<< std::setw(15) << plays * 1.0 / (wheel_hits) << std::endl;

	// Jackpot
	std::cout << std::setw(15) << "Jackpot"
		<< std::setw(15) << std::fixed << (jackpot_win) * 1.0 / plays / apuesta
		<< std::setw(15) << (jackpot_win) * 1.0 / (jackpot_hits) / apuesta
		<< std::setw(15) << plays * 1.0 / (jackpot_hits) << std::endl;

	cout << endl;
	cout << endl;
	// ------------ BASE -------------
	cout << "Frequency of number of scatters in base game: " << endl;
	for (int i = 0; i < n_scatters_count.size(); i++)
		cout << "\t" << i << " Scatters: " << (plays * 1.0) / n_scatters_count[i] << endl;
	cout << endl;

	cout << "Frequency of number of super symbols in base game: " << endl;
	for (int i = 0; i < n_ss_count.size() - 1; i++)
		cout << "\t" << i << " Super symbols : " << (plays * 1.0) / n_ss_count[i] << endl;
	cout << "\t7+ Super symbols : " << (plays * 1.0) / n_ss_count[7] << endl;
	cout << endl;
	cout << endl;

	// ------------ FREE SPINS -------------
	cout << "Avg number of free spins: " << (n_spins_fs * 1.0) / free_spins_hits << endl;
	cout << endl;

	cout << "Bison Spin in free spins: " << endl;
	cout << std::setw(15) << "Count" << std::setw(15) << "Percentage" << std::endl;
	cout << std::setfill('-') << std::setw(30) << "" << std::setfill(' ') << std::endl;
	for (const auto& count : n_bison_spin_fs_count)
		cout << std::setw(15) << count.first << std::setw(15) << (count.second * 1.0 / free_spins_hits) * 100 << "%" << std::endl;
	cout << endl;

	cout << "Number of total spins in free spins : " << endl;
	cout << std::setw(15) << "Count" << std::setw(15) << "Percentage" << std::endl;
	cout << std::setfill('-') << std::setw(30) << "" << std::setfill(' ') << std::endl;
	for (const auto& count : n_spins_fs_count)
		cout << std::setw(15) << count.first << std::setw(15) << (count.second * 1.0 / free_spins_hits) * 100 << "%" << std::endl;
	cout << endl;

	cout << "Total multiplier at the end of free spins: " << endl;
	cout << std::setw(15) << "Count" << std::setw(15) << "Percentage" << std::endl;
	cout << std::setfill('-') << std::setw(30) << "" << std::setfill(' ') << std::endl;
	for (const auto& multi : multipliers_count)
		cout << std::setw(15) << multi.first << std::setw(15) << (multi.second * 1.0 / free_spins_hits) * 100 << "%" << std::endl;
	cout << endl;
	cout << endl;


	// ------------ BONUS -------------
	cout << "Avg number of spins in bonus: " << (n_bonus_spins_count * 1.0) / bonus_hits << endl;
	cout << endl;

	cout << "Super symbols at the end of bonus: " << endl;
	cout << std::setw(15) << "Count" << std::setw(15) << "Percentage" << std::endl;
	cout << std::setfill('-') << std::setw(30) << "" << std::setfill(' ') << std::endl;
	for (int i = 0; i < super_symbols_count.size(); i++)
		cout << std::setw(15) << i << std::setw(15) << (super_symbols_count[i] * 1.0 / bonus_hits) * 100 << "%" << std::endl;
	cout << endl;
	cout << endl;





	
	win_distr_base.print_win_distr();
	win_distr_bison_spin.print_win_distr();
	win_distr_fg.print_win_distr();
	win_distr_bonus.print_win_distr();
	cout << endl;



	//std::setprecision(8)
}
