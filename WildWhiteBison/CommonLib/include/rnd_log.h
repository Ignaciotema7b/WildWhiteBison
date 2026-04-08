#pragma once
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include "Game_info.h"
#include "Weighted_values.h"
#include "Reel_set.h"
#include "Slot_config.h"


using namespace std;

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

class rnd_log
{
public:

	void init(Game_info info);



	void log_rnd(Weighted_values w);

	/// <summary>
	/// Agrega logs de stops de reel set. Si tiene tracker supone un maximo de 4 celdas de tracker
	/// Usar esta funcion despues de llenar pantalla para que los stops esten actualizados.
	/// </summary>
	/// <param name="reel_set"></param>
	void log_stops(Reel_set reel_set);


	void log_stops(Reel_set reel_set, string mask, vector<int> offsets);

	//Loguea solo el stop para "reel", (no sirve para tracker)
	void log_stops(Reel_set reel_set, int reel);

	void log_spin_win(unsigned long long spin_win, vector<vector<int>> screen, string additional_info = "-");

	void log_total_win(unsigned long long total_win, vector<vector<int>> screen, string additional_info = "-");

	void log_rnd(string mask, unsigned result, unsigned range);

	void log_rnd_without_replacement(Weighted_values w, unsigned sample_size);

	void resume();

	unsigned long long number_of_logs = 10'000;

private:

	FILE* file_rnd_log;
	FILE* file_log_plays;

	string str_log_rnd;
	string str_log_play;

	long log_count = 0;
	long log_spin_count = 0;
	long log_freespin_count = 0;

	string randomLog_path;
	string gameDetails_path;

	Slot_config config;

	string log_screen( vector<vector<int>> screen, string additional_info = "");

};


