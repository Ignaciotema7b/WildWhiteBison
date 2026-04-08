#include "..\include\Win_and_RTP_distribution.h"

Win_and_RTP_distribution::Win_and_RTP_distribution(string name)
{
	feature_name = name;

	json_win_distr["feature_name"] = feature_name;
}


json Win_and_RTP_distribution::to_json()
{
	json_win_distr["non_winning_plays"] = non_winning_plays;
	json_win_distr["bigger_wins"] = bigger_wins;
	json_win_distr["coinout"] = coinout;

	return json_win_distr;
}

void Win_and_RTP_distribution::count_stats_play(unsigned long long int win, unsigned stake)
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

	//Counts how many times the feature was triggered
	total_hits++;

	total_win += win;
	total_payout_sq += (unsigned long long int) pow(win, 2);
	add_win_distr_value((double)win, stake);

}

void Win_and_RTP_distribution::print_ranks(double rank[], unsigned size, unsigned wins[], unsigned long long hits, double rtp_distribution[], unsigned long long win)
{
	//printf("\n Win and RTP Distribution for %s \n", feature_name);
	cout << "\n Win and RTP Distribution for " << feature_name << endl;
	printf("      Rank            Coin(%%)    RTP(%%)\n");
	printf("[%6.2f,%6.2f]      %6.4f     %6.4f\n", rank[0], rank[0], wins[0] * 1.0 / hits, rtp_distribution[0]  / win);
	for (unsigned k = 1; k < size; k++) {
		printf("(%6.2f,%6.2f]      %6.4f     %6.4f\n", rank[k - 1], rank[k], wins[k] * 1.0 / hits, rtp_distribution[k]  / win);
	}
	printf("(%6.2f,...)      %6.4f     %6.4f\n\n", rank[size - 1], wins[size] * 1.0 / hits, rtp_distribution[size]  / win);
}

void Win_and_RTP_distribution::add_win_distr_value(double win, unsigned stake)
{
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

void Win_and_RTP_distribution::print_win_distr(void)
{
	print_ranks(ranks, n_ranks, wins, total_hits, rtp_distribution, total_win);
}

void Win_and_RTP_distribution::print_hits_by_ranges(vector<double> intervals_xbet, unsigned stake, bool open_closed)
{
	cout << "\n Hits by ranges for " << feature_name << endl;
	cout << "Range\tHit_number\n";
	//Hits 0x:
	cout << "0x\t" << non_winning_plays << endl;

	//Intervals...
	if (open_closed)
	{
		unsigned iter = 0;
		unsigned long long hits;
		for (int interval = 1; interval < intervals_xbet.size(); interval++)
		{
			hits = 0;
			while (iter <= intervals_xbet[interval] * stake && iter < PAYOUTS_LIST_SIZE)
			{
				hits += coinout[iter];
				iter++;
			}

			//Bigger wins
			for (int i = 0; i < (int)bigger_wins.size(); i++)
			{
				if (bigger_wins[i] <= intervals_xbet[interval] * stake && bigger_wins[i] > intervals_xbet[interval - 1] * stake)
					hits++;
			}


			cout << "(" << intervals_xbet[interval - 1] << "x-"
				<< intervals_xbet[interval] << "x]\t" << hits << "\t" <<
				total_hits *1.0 / hits << endl;

		}

		//Last interval: greater than...
		hits = 0;
		while ( iter < PAYOUTS_LIST_SIZE)
		{
			
			hits += coinout[iter];
			iter++;
		}

		//Bigger wins
		for (int i = 0; i < (int)bigger_wins.size(); i++)
		{
			if (bigger_wins[i] > intervals_xbet[(int)intervals_xbet.size() - 1] * stake )
				hits++;
		}


		cout  << intervals_xbet[(int)intervals_xbet.size() - 1] << "x+\t"
			<< hits << "\t" << total_hits * 1.0 / hits << endl;


	}
	else
	{
		unsigned iter = 0;
		unsigned long long hits;
		for (int interval = 1; interval < intervals_xbet.size(); interval++)
		{
			hits = 0;
			while (iter < intervals_xbet[interval] * stake && iter < PAYOUTS_LIST_SIZE)
			{
				hits += coinout[iter];
				iter++;
			}

			//Bigger wins
			for (int i = 0; i < (int)bigger_wins.size(); i++)
			{
				if (bigger_wins[i] < intervals_xbet[interval] * stake && bigger_wins[i] >= intervals_xbet[interval - 1] * stake)
					hits++;
			}


			cout << "[" << intervals_xbet[interval - 1] << "x-"
				<< intervals_xbet[interval] << "x)\t" << hits << "\t" << total_hits * 1.0 / hits << endl;

		}

		//Last interval: greater than...
		hits = 0;
		while (iter < PAYOUTS_LIST_SIZE)
		{
			hits += coinout[iter];
			iter++;
		}

		//Bigger wins
		for (int i = 0; i < (int)bigger_wins.size(); i++)
		{
			if (bigger_wins[i] >= intervals_xbet[(int)intervals_xbet.size() - 1] * stake)
				hits++;
		}


		cout << intervals_xbet[(int)intervals_xbet.size() - 1] << "x+\t"
			<< hits << "\t" << total_hits * 1.0 / hits << endl;


	}

}

unsigned Win_and_RTP_distribution::median()
{
	unsigned m = 0;
	unsigned long long acum = 0;
	acum = non_winning_plays;

	for (int i = 0; i < PAYOUTS_LIST_SIZE; i++)
	{
		acum += coinout[i];

		if (acum >= total_hits * 1.0 / 2)
		{
			m = i;
			break;
		}
			
	}

	return m;
}
