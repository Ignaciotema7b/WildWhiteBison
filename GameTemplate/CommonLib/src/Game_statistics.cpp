#include "..\include\Game_statistics.h"
#include <random>



Game_statistics::Game_statistics()
{
	for (int i = 0; i < PAYOUTS_LIST_SIZE; i++)
		coinout[i] = 0;
}

Game_statistics::~Game_statistics()
{
}


void Game_statistics::print_ranks(double rank[], unsigned size, unsigned wins[], unsigned long long hits, double rtp_distribution[], unsigned long long win, const char title[])
{
	printf("\n Win and RTP Distribution for %s \n", title);
	printf("      Rank            Coin(%%)    RTP(%%)\n");
	printf("[%6.2f,%6.2f]      %6.4f     %6.4f\n", rank[0], rank[0], wins[0] * 1.0 / hits, rtp_distribution[0]  / win);
	for (unsigned k = 1; k < size; k++) {
		printf("(%6.2f,%6.2f]      %6.4f     %6.4f\n", rank[k - 1], rank[k], wins[k] * 1.0 / hits, rtp_distribution[k]  / win);
	}
	printf("(%6.2f,...)      %6.4f     %6.4f\n\n", rank[size - 1], wins[size] * 1.0 / hits, rtp_distribution[size]  / win);

}

void Game_statistics::add_win_distr_value(double win, unsigned stake)
{
	/* Para intervalos cerrados a izquierda y abiertos a derecha
	if (!win) { wins[0]++; }	//If there is no win, add 1 hit to wins[0]
	else if (win / stake >= ranks[n_ranks - 1]) { //If win divided by bet is greater than max number in rank...
		wins[n_ranks]++;
		rtp_distribution[n_ranks] += win;
	}
	else {
		for (unsigned k = 1; k < n_ranks; k++) { //Search for interval where lies "win divided by bet"..
			if (ranks[k - 1] <= win / stake && win / stake < ranks[k]) {
				wins[k]++;
				rtp_distribution[k] += win;
				break;
			}
		}
	}*/

	/* Para intervalos abiertos a izquierda y cerrados a derecha */
	if (!win) { wins[0]++; }	//If there is no win, add 1 hit to wins[0]
	else if (win / stake > ranks[n_ranks - 1]) { //If win divided by bet is greater than max number in rank...
		wins[n_ranks]++;
		rtp_distribution[n_ranks] += win;
	}
	else {
		for (unsigned k = 1; k < n_ranks; k++) { //Search for interval where lies "win divided by bet"..
			if (ranks[k - 1] < win / stake && win / stake <= ranks[k]) {
				wins[k]++;
				rtp_distribution[k] += win;
				break;
			}
		}
	}
}

vector<vector<unsigned long long>> Game_statistics::summary_coinout()
{
	vector<vector<unsigned long long>> res;

	res.push_back({ 0,non_winning_plays });

	for (unsigned long long i = 1; i < PAYOUTS_LIST_SIZE; i++)
	{
		if(coinout[i] != 0)
			res.push_back({ i,coinout[i] });
	}

	vector<vector<unsigned long long>> big_wins = summary_big_wins();

	// Agrega los resultados a la matriz existente
	for (const auto& row : big_wins)
	{
		res.push_back(row);
	}

	return res;
}

vector<vector<unsigned long long>> Game_statistics::summary_coinout_no_fs()
{
	vector<vector<unsigned long long>> res;

	res.push_back({ 0,non_winning_plays_no_fs });

	for (unsigned long long i = 1; i < PAYOUTS_LIST_SIZE; i++)
	{
		if (coinout_no_fs[i] != 0)
			res.push_back({ i,coinout_no_fs[i] });
	}

	vector<vector<unsigned long long>> big_wins_no_fs = summary_big_wins_no_fs();

	// Agrega los resultados a la matriz existente
	for (const auto& row : big_wins_no_fs)
	{
		res.push_back(row);
	}

	return res;
}

vector<vector<unsigned long long>> Game_statistics::summary_coinout_fs()
{
	vector<vector<unsigned long long>> res;

	res.push_back({ 0,non_winning_plays_fs });

	for (unsigned long long i = 1; i < PAYOUTS_LIST_SIZE; i++)
	{
		if (coinout_fs[i] != 0)
			res.push_back({ i,coinout_fs[i] });
	}

	vector<vector<unsigned long long>> big_wins_fs = summary_big_wins_fs();

	// Agrega los resultados a la matriz existente
	for (const auto& row : big_wins_fs)
	{
		res.push_back(row);
	}

	return res;
}

vector<vector<unsigned long long>> Game_statistics::summary_big_wins()
{
	// Crear una copia del vector original para no modificarlo
	std::vector<unsigned long long> copiaVec = bigger_wins;

	// Ordenar el vector copia
	std::sort(copiaVec.begin(), copiaVec.end());

	// Crear un vector de vectores para almacenar el resultado
	std::vector<std::vector<unsigned long long>> resultado;

	// Iterar sobre el vector ordenado y contar las ocurrencias
	for (size_t i = 0; i < copiaVec.size();)
	{
		unsigned long long valor = copiaVec[i];
		size_t contador = 0;

		// Contar las ocurrencias del valor actual
		while (i < copiaVec.size() && copiaVec[i] == valor)
		{
			++contador;
			++i;
		}

		// Agregar el par valor-contador al resultado
		resultado.push_back({ valor, contador });
	}

	return resultado;
}

vector<vector<unsigned long long>> Game_statistics::summary_big_wins_no_fs()
{
	// Crear una copia del vector original para no modificarlo
	std::vector<unsigned long long> copiaVec = bigger_wins_no_fs;

	// Ordenar el vector copia
	std::sort(copiaVec.begin(), copiaVec.end());

	// Crear un vector de vectores para almacenar el resultado
	std::vector<std::vector<unsigned long long>> resultado;

	// Iterar sobre el vector ordenado y contar las ocurrencias
	for (size_t i = 0; i < copiaVec.size();)
	{
		unsigned long long valor = copiaVec[i];
		size_t contador = 0;

		// Contar las ocurrencias del valor actual
		while (i < copiaVec.size() && copiaVec[i] == valor)
		{
			++contador;
			++i;
		}

		// Agregar el par valor-contador al resultado
		resultado.push_back({ valor, contador });
	}

	return resultado;
}

vector<vector<unsigned long long>> Game_statistics::summary_big_wins_fs()
{
	// Crear una copia del vector original para no modificarlo
	std::vector<unsigned long long> copiaVec = bigger_wins_fs;

	// Ordenar el vector copia
	std::sort(copiaVec.begin(), copiaVec.end());

	// Crear un vector de vectores para almacenar el resultado
	std::vector<std::vector<unsigned long long>> resultado;

	// Iterar sobre el vector ordenado y contar las ocurrencias
	for (size_t i = 0; i < copiaVec.size();)
	{
		unsigned long long valor = copiaVec[i];
		size_t contador = 0;

		// Contar las ocurrencias del valor actual
		while (i < copiaVec.size() && copiaVec[i] == valor)
		{
			++contador;
			++i;
		}

		// Agregar el par valor-contador al resultado
		resultado.push_back({ valor, contador });
	}

	return resultado;
}



void Game_statistics::print_win_distr(void)
{
	//print_ranks(ranks, n_ranks, wins, plays, rtp_distribution, total_payout, "Total Game");
	print_ranks(ranks, n_ranks, wins, plays, rtp_distribution, total_payout, "Total Game");
}

void Game_statistics::count_stats_play(unsigned long long int win, unsigned stake)
{
	if (win == 0)
		non_winning_plays++;
	else
	{
		//Count value
		if (win < (unsigned long long) PAYOUTS_LIST_SIZE)
			coinout[win]++;
		else
			bigger_wins.push_back((unsigned)win);
	}
	

	plays++;

	if (win > 0)
		hits++;

	total_payout += win;
	total_payout_sq += (unsigned long long int) pow(win, 2);
	add_win_distr_value((double) win , stake);

	if (win > max_payout)
		max_payout = win;

	//Sessions stats
	current_balance = current_balance - stake + win;

	if (current_balance < stake || current_balance >= starting_balance * 2) {

		if (current_balance < stake) {
			sessions_loosing++;
		}
		else {
			sessions_duplicating++;
		}
		current_balance = starting_balance;
		total_sessions++;
	}
}

void Game_statistics::count_stats_play(unsigned long long int win, bool is_fs)
{
	if (is_fs)
	{
		if (win == 0)
			non_winning_plays_fs++;
		else
		{
			//Count value
			if (win < (unsigned long long) PAYOUTS_LIST_SIZE)
				coinout_fs[win]++;
			else
				bigger_wins_fs.push_back((unsigned)win);
		}
	}
	else
	{
		if (win == 0)
			non_winning_plays_no_fs++;
		else
		{
			//Count value
			if (win < (unsigned long long) PAYOUTS_LIST_SIZE)
				coinout_no_fs[win]++;
			else
				bigger_wins_no_fs.push_back((unsigned)win);
		}
	}
	
}

void Game_statistics::print_coinout()
{
	cout << "##################################################\n";
	cout << "Coin out: (printed in file out.csv)" << endl;
	cout << "##################################################\n" << endl;

	std::ofstream out("out.csv");
	std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

	std::cout << "Win,Hits   (WIP bigger wins not finished)," << std::endl;
	std::cout << 0 << "," << non_winning_plays << ",20" << std::endl;
	for (int i = 1; i < (int)PAYOUTS_LIST_SIZE; i++)
	{
		if (coinout[i])
		{
			std::cout << i << "," << coinout[i] << "," << std::endl;
		}

	}

	sort(bigger_wins.begin(), bigger_wins.end());

	//Contar e imprimir valores de bigger_wins
	if (!empty(bigger_wins))
	{
		unsigned value = bigger_wins[0];
		unsigned count = 1;
		unsigned i = 0;


		for (unsigned i = 0; i < (unsigned)(bigger_wins.size()); i++)
			cout << bigger_wins[i] << "," << 1 << endl;

		//WIP: este bloque no esta terminado 
		/*while (i+1 < (unsigned)(bigger_wins.size()))
		{
			i++;
			if (bigger_wins[i] == value)
				count++;
			else
			{
				cout << value << "," << count << endl;

			}

		}*/
	}

	std::cout.rdbuf(coutbuf); //reset to standard output again


	cout << "\n##################################################\n";


}

void Game_statistics::reset()
{
	plays = 0;
	total_payout = 0;
	hits = 0;
	max_payout = 0;
	total_payout_sq = 0;
}

unsigned Game_statistics::max_win_hits()
{
	unsigned res = 0;

	if (max_payout < PAYOUTS_LIST_SIZE)
		res = coinout[max_payout];
	else
	{
		for (int i = 0; i < (int)bigger_wins.size(); i++)
		{
			if (bigger_wins[i] == max_payout)
				res++;
		}
	}

	return res;
}

vector<vector<unsigned long long>> Game_statistics::get_wins_hits()
{
	vector<unsigned long long> wins;
	vector<unsigned long long> hits;

	vector<vector<unsigned long long>> res;

	wins.push_back(0);
	hits.push_back(non_winning_plays);

	for (int i = 1; i < (int)PAYOUTS_LIST_SIZE; i++)
	{
		if (coinout[i])
		{
			wins.push_back(i);
			hits.push_back(coinout[i]);
		}

	}



	if (!empty(bigger_wins))
	{
		for (unsigned i = 0; i < (unsigned)(bigger_wins.size()); i++)
		{
			wins.push_back(bigger_wins[i]);
			hits.push_back(1);
		}
	}

	res.push_back(wins);
	res.push_back(hits);

	return res;
}


double Game_statistics::standard_deviation(unsigned stake)
{
	//Var(X)=E(X^2)-E(X)^2 (dividing by n)
	double var = total_payout_sq *1.0 / plays - pow(total_payout * 1.0 / plays, 2);
	//Standard deviation (dividing by n-1) "standarized" by bet (RTP standard deviation)
	return sqrt(var * plays / (plays - 1)) / stake;
}

void Game_statistics::count_stats_play(unsigned long long int win, unsigned stake, unsigned using_reelset)
{
	count_stats_play(win, stake);
	while (total_win_by_reelset.size() <= using_reelset) {
		total_win_by_reelset.push_back(0);
		total_hits_by_reelset.push_back(0);
	}

	total_hits_by_reelset[using_reelset]++;
	total_win_by_reelset[using_reelset] += win;
}

void Game_statistics::print_rtp_n_this_per_reelset()
{
	cout << "******* Rtp by reel set" << endl;
	for (int i = 0; i < total_win_by_reelset.size(); i++) {
		cout << "Reel set " << i << " " << total_win_by_reelset[i] << endl;
	}

	cout << "******* Hits by reel set" << endl;
	for (int i = 0; i < total_hits_by_reelset.size(); i++) {
		cout << "Reel set " << i << " " << total_hits_by_reelset[i] << endl;
	}
}

json Game_statistics::stats_to_json(unsigned stake)
{
	json resp;

	resp["hits"] = hits;
	resp["plays"] = plays;
	resp["total_payout"] = total_payout;
	resp["max_payout"] = max_payout;
	resp["total_payout_sq"] = total_payout_sq;
	resp["hit_rate"] = hits * 1.0 / plays;
	resp["rtp_total"] = total_payout * 1.0 / plays / stake;
	resp["sd"] = standard_deviation(stake);

	//Win distribution
	//resp["win_distribution"]["non_winning_plays"] = non_winning_plays; 
	//resp["win_distribution"]["bigger_wins"] = bigger_wins;
	//resp["win_distribution"]["coinout"] = coinout;

	resp["coinOut"] = summary_coinout();

	resp["coinOut_no_fs"] = summary_coinout_no_fs();
	resp["coinOut_fs"] = summary_coinout_fs();

	//Sessions stats:
	resp["starting_balance"] = starting_balance;
	resp["current_balance"] = current_balance;

	resp["total_sessions"] = total_sessions;
	resp["sessions_loosing"] = sessions_loosing;
	resp["sessions_duplicating"] = sessions_duplicating;

	return resp;
}

void Game_statistics::test_rtp_confidence_95(double rtp_target, unsigned stake)
{
	double actual_rtp = total_payout * 1.0 / plays / stake ;
	double sd = standard_deviation(stake);
	double upper_limit = rtp_target + 1.959964 * sd / sqrt(1.0 * plays);
	double lower_limit = rtp_target - 1.959964 * sd / sqrt(1.0 * plays);

	if (actual_rtp < upper_limit && actual_rtp > lower_limit)
	{
		cout << "\n********************\n         RTP OK     \n********************\n";
		return;
	}
	else
	{
		if (actual_rtp > upper_limit)
		{
			cout << "\n********************\n         RTP ALTO     \n********************\n";
			cout << "RTP actual: " << actual_rtp << endl;
			cout << "Limite superior: " << upper_limit << endl;
			return;
		}
		else
		{
			cout << "\n********************\n         RTP BAJO     \n********************\n";
			cout << "RTP actual: " << actual_rtp << endl;
			cout << "Limite inferior: " << lower_limit << endl;
			return;
		}
	}
}

void Game_statistics::print_stats(unsigned stake)
{
	print_stats_WO_coin_out(stake);

	print_win_distr();

	cout << "************ SESSIONS INFORMATION *********************" << endl;
	cout << "total sessions " << total_sessions << endl;
	cout << "loosing " << sessions_loosing * 100.0 / total_sessions << "%" << endl;
	cout << "duplicating " << sessions_duplicating * 100.0 / total_sessions << "%" << endl << endl;
	cout << "*******************************************************" << endl;

}

void Game_statistics::print_stats_WO_coin_out(unsigned stake)
{
	cout << "##################################################\n";
	cout << "General results: " << endl;
	cout << "##################################################\n" << endl;
	cout << "Total amount of plays: " << plays << endl;
	cout << "RTP total: " << total_payout * 1.0 / plays / stake << endl;
	cout << "Hit ratio: " << hits * 1.0 / plays << endl;
	cout << "Standard Deviation: " << standard_deviation(stake) << endl << endl;
	cout << "Diff RTP without and with cap: " << (total_payout_without_cap - total_payout) * 100.0 / plays / stake << "%" << endl;
	cout << "Max exposure: " << max_payout <<
		" credits ( " << max_payout * 1.0 / stake << "x )" << endl;

}
