#include "..\include\Weighted_values.h"


void Weighted_values::init(string path)
{
	//Load values and weights
	load_params(path);

	//Calculate the rest of parameters (only the first time, these parameters optimize the performance)
	calculate_params();

	//If name was no set then load a name by default:
	if (name.empty())
		this->name = path;

	//Warning: max sum of weights 
	/*if (total_weight > 30000)
	{
		cout << "***********  Warning:  **********************\n "; 
		cout << "Total weight for " << name << " is more than 30000" << endl;
		system("PAUSE");
	}*/


}

void Weighted_values::init(vector<int> values, vector<unsigned> weights, string name)
{
	this->values = values;
	this->weights = weights;

	int x = 0;
	for (int i = 0; i < (int)weights.size(); i++)
	{
		if (weights[i] > 0)
			x++;
	}
	drawn_without_replacement.resize(x); //only values with positive weight can be picked

	cumulative_weight.resize(0);

	//Calculate the rest of parameters (only the first time, these parameters optimize the performance)
	calculate_params();

	//Set length
	aux_weights.resize(static_cast<int>(weights.size()));
	last_random_without_replacement.resize(static_cast<int>(weights.size()));
	last_total_weights_without_replacement.resize(static_cast<int>(weights.size()));
}

int Weighted_values::sample(mt19937_64 &rng)
{
	return values[sample_index(rng)];
}

int Weighted_values::sample_index(mt19937_64 &rng)
{
	int res = 0;
	uniform_int_distribution<int> ui(0, total_weight - 1);
	int r = ui(rng);

	last_random = r;

	for (int j = 0; j < length; j++) {
		if (r < (int)cumulative_weight[j]) {
			res = j;
			break;
		}
	}

	return res;
}

void Weighted_values::sample_without_replacement( mt19937_64& rng)
{
	i_value = 0; //first value

	//Copy original weights
	for (int i = 0; i < (int)aux_weights.size(); i++)
		aux_weights[i] = weights[i];

	unsigned w = total_weight;

	for (int x = 0; x < length; x++)
	{
		if (w == 0)
			break;
		int res = 0;
		uniform_int_distribution<int> ui(0, w - 1);
		int r = ui(rng);

		last_random_without_replacement[x] = r;
		last_total_weights_without_replacement[x] = w;

		unsigned cumulative = aux_weights[0];

		for (int j = 0; j < length; j++) {
			if (r < cumulative) {
				res = j;
				w -= aux_weights[j];
				aux_weights[j] = 0; //do not choose again the same value
				drawn_without_replacement[x] = values[j];

				break;
			}
			else
			{
				cumulative += aux_weights[j + 1];
			}
		}

	}



}

int Weighted_values::next_value()
{
	if (i_value < (int)drawn_without_replacement.size() )
	{
		i_value++;
		return drawn_without_replacement[i_value - 1];
	}
	else
	{
		cout << "Error in draw without replacement: values should be shuffled/drawn again, last value was reached...  (" 
			<< name << ")"  << endl;
		system("PAUSE");
	}

	return 0;
}


json Weighted_values::to_json()
{
	json resp;

	resp["values"] = values;
	resp["weights"] = weights;

	return resp;
}

string Weighted_values::log()
{
	string response = name +":" +
		std::to_string(last_random) + ":" +
		std::to_string(total_weight) ;
	return response;
}

string Weighted_values::log_without_replacement(unsigned n)
{
	string response = name + ":" +
		std::to_string(last_random_without_replacement[0]) + ":" +
		std::to_string(last_total_weights_without_replacement[0]);

	for (int i = 1; i < n; i++)
	{
		response += ","  + name + ":" +
			std::to_string(last_random_without_replacement[i]) + ":" +
			std::to_string(last_total_weights_without_replacement[i]);
	}

	return response;
}

void Weighted_values::load_params(string file_path)
{
	bool file_exists;

	std::ifstream infile(file_path);

	file_exists = infile.good();

	if (!file_exists)
	{
		cout << "***********  Warning: file " << file_path << " not found\n";
		return;
	}
		
	string line;
	ifstream file(file_path);

	//Read values. Values are in the first line separated by commas
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
		values.push_back(stoi(s));
		i++;
	}

	//Read weights. Weights are in the second line separated by commas.
	getline(file, line, '\n'); //read reel r
	i = 0;
	while (i < (int)line.size())
	{
		string s;
		while (line[i] != ',' && i < (int)line.size())
		{
			s = s + line[i];
			i++;
		}
		weights.push_back(stoi(s));

		if (stoi(s) > 0)
			drawn_without_replacement.push_back(0); //only values with positive weight can be picked
		i++;
	}

	//Set length
	aux_weights.resize(static_cast<int>(weights.size()));
	last_random_without_replacement.resize(static_cast<int>(weights.size()));
	last_total_weights_without_replacement.resize(static_cast<int>(weights.size()));

	//Check values and weights lengths..
	if (static_cast<int>(weights.size()) != static_cast<int>(values.size()))
	{
		cout << "Warning: values and weights have different lengths\nValues loaded from: " << file_path << endl ;
		return;
	}

}

void Weighted_values::calculate_params()
{
	total_weight = 0;

	for (int i = 0; i < static_cast<int>(weights.size()); i++) {
		total_weight += weights[i];
		cumulative_weight.push_back(total_weight);
	}

	length = static_cast<int>(weights.size());

}


int Weighted_values::sample_WO_cumulative(mt19937_64& rng)
{
	return values[sample_index_WO_cumulative(rng)];
}

int Weighted_values::sample_index_WO_cumulative(mt19937_64& rng)
{
	int res = 0;
	uniform_int_distribution<int> ui(0, total_weight - 1);
	int r = ui(rng);

	last_random = r;

	for (int j = 0; j < length; j++)
	{
		if (r < weights[j])
		{
			res = j;
			break;
		}
		r -= weights[j];
	}

	return res;
}

Weighted_values Weighted_values::make_copy()
{
	Weighted_values to_return;
	to_return.total_weight = total_weight;

	to_return.length = length;
	for (int i = 0; i < length; i++)
	{
		to_return.cumulative_weight.push_back(cumulative_weight[i]);
		to_return.values.push_back(values[i]);
		to_return.weights.push_back(weights[i]);
	}
	return to_return;
}