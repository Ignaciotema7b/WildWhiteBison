#include "..\include\Reel_set.h"

using namespace std;



void Reel_set::init(int n_reels, string file_path)
{
	bool file_exists;

	std::ifstream file(file_path);

	file_exists = file.good();

	if (!file_exists)
	{
		//TODO: handle error...
		return;
	}

	//Set number_of_reels
	number_of_reels = n_reels;

	//Initialize stops
	for (int i = 0; i < n_reels; i++)
		stops.push_back(0);

	//Load reel strips from file:
	for (int r = 0; r < n_reels; r++)
	{

		vector<int> reel_strip;
		string line;
		getline(file, line, '\n'); //read reel r
		int i = 0;
		while (i < (int)line.size())
		{
			string s;
			while (line[i] != ',' && i < (int)line.size())
			{
				s = s + line[i];
				i++;
			}
			reel_strip.push_back(stoi(s));
			i++;
		}
		reel_strips.push_back(reel_strip);

		reels_length.push_back((unsigned) reel_strip.size());
	}

	file.close();

}

void Reel_set::init(int n_reels, int n_trackers, string file_path)
{
	init(n_reels, file_path);

	//Add trackers....
	bool file_exists;

	std::ifstream file(file_path);

	file_exists = file.good();

	if (!file_exists)
	{
		//TODO: handle error...
		return;
	}

	//Set number_of_trackers
	number_of_trackers = n_trackers;

	//Initialize stops
	for (int i = 0; i < n_trackers; i++)
		stops_trackers.push_back(0);


	//Skip reels strips:
	string line;
	for (int r = 0; r < n_reels; r++)
		getline(file, line, '\n'); //read reel r


	for (int r = 0; r < n_trackers; r++)
	{
		vector<int> strip;
		getline(file, line, '\n'); //read reel r
		int i = 0;
		while (i < (int)line.size())
		{
			string s;
			while (line[i] != ',' && i < (int)line.size())
			{
				s = s + line[i];
				i++;
			}
			strip.push_back(stoi(s));
			i++;
		}
		trackers_strips.push_back(strip);

		trackers_length.push_back((unsigned)strip.size());
	}

	file.close();

}

void Reel_set::spin(mt19937_64 &rng)
{
	//Draw stops for each reel.
	for (int reel = 0; reel < (int)reel_strips.size(); ++reel)
	{
		uniform_int_distribution<int> ui(0, (int)reel_strips[reel].size() - 1);
		stops[reel] = ui(rng);

	}

	//Draw stops for each tracker.
	for (int reel = 0; reel < (int)trackers_strips.size(); ++reel)
	{
		uniform_int_distribution<int> ui(0, (int)trackers_strips[reel].size() - 1);
		stops_trackers[reel] = ui(rng);

	}

}

void Reel_set::force_stops(vector<unsigned> stops_param)
{
	if ((int)stops_param.size() != number_of_reels)
	{
		printf("Warning: forcing stops, size of parameter 'stops' doesn't match number of reels\n\n");
		system("PAUSE");
	}
	else
	{
		for (int i = 0; i < number_of_reels; ++i)
			stops[i] = stops_param[i];
	}
}

int Reel_set::calculate_min_dist(int symbol, int reel)
{
	int min_dist = -1;
	int dist_next;

	for (int i = 0; i < (int)reel_strips[reel].size(); i++)
	{
		if (reel_strips[reel][i] == symbol)
		{
			dist_next = 1;
			//Look for next symbols of the same class
			while (reel_strips[reel][(i + dist_next) % (int) reel_strips[reel].size()] != symbol)
				dist_next++;
			
			if(dist_next < min_dist || min_dist == -1)
			{
				min_dist = dist_next;
			}
		}
	}

	return min_dist;
}

string Reel_set::log_stops()
{
	string response;
	
	//Stop for first reel
	response += "ST:" +
		std::to_string(stops[0]) + ":" +
		std::to_string(reels_length[0]) ;

	//Stop for reels 2 
	for (int i = 1; i < number_of_reels; i++)
	{
		response += ",ST:" +
			std::to_string(stops[i]) + ":" +
			std::to_string(reels_length[i]) ;
	}

	//Trackers stops
	for (int i = 0; i < number_of_trackers; i++)
	{
		//Adjust stop: it is logged after stop is updated for next spin
		//Here the adjustment is made cosidering a tracker of 4 symbols
		response += ",ST:" +
			std::to_string( ( stops_trackers[i]+ (int) trackers_strips[i].size() -3 ) % (int)trackers_strips[i].size()) + ":" +
			std::to_string(trackers_length[i]) ;
	}

	return response;
}

void Reel_set::count_symbols_by_reel(int number_of_symbols)
{
	//Initialize size...
	for (int symbol = 0; symbol < number_of_symbols; symbol++)
	{
		vector<unsigned> zeros(number_of_reels, 0);
		symbols_distribution.push_back(zeros);
	}


	//Iterate through reels' positions...
	for (int reel = 0; reel < number_of_reels; reel++)
	{
		for (int pos = 0; pos < reels_length[reel]; pos++)
		{
			int symbol = reel_strips[reel][pos];

			symbols_distribution[symbol][reel]++;
		}

	}

}

