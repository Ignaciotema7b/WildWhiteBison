#include "..\include\Game.h"

Game::Game()
{

}


Game::~Game()
{
}

void Game::init(unsigned stake)
{
	//Initialize RNG
	random_device rd;
	unsigned long long seed = rd();
	//seed = 4041974;
	rng_seed = seed;
	rng.seed( seed );

	printf("Game initialized...\n");

	this->stake = stake;
}

void Game::saveRNGstate()
{
	//If folder "results" does not exist then create it.
	if (!std::filesystem::exists(info.folder_results))
		std::filesystem::create_directory(info.folder_results);

	std::ofstream o(info.folder_results + "\\rng_state.txt");

	o << rng << std::endl;
	o.close();
}
void Game::loadRNGstate()
{

	std::ifstream  i(info.folder_results + "\\rng_state.txt");
	i >> rng;
	i.close();
}

//Returns a pseudo-random number in the range of 0 to n-1.
unsigned Game::random(unsigned n)
{
	uniform_int_distribution<int> ui(0, n - 1);
	return (unsigned) ui(rng);
}

//Returns a number from vector x weighted by "weights"
int Game::sample(std::vector<int> x, std::vector<unsigned> weights)
{
	int res = 0;

	//Check x and weights have same length
	if (x.size() == weights.size())
	{
		int acum = 0;
		int length = static_cast<int>(x.size());
		for (int i = 0; i < length; i++) {
			acum += weights[i];
		}
		uniform_int_distribution<int> ui(0, acum - 1);
		//int r = static_cast<int>(1.0 * acum * rand() / (RAND_MAX + 1.0)); //Int random between 0 and total weight
		int r = ui(rng);
		int v = 0;
		for (int j = 0; j < length; j++) {
			if (v <= r && r < v + (int)weights[j]) {
				res = x[j];
				break;
			}
			v = v + weights[j];
		}
	}
	else {
		printf("Error: x and weights have different lenghts");
	}

	return res;
}

//Returns a  number in the range of 0 to n-1 weighted by "weights"
int Game::sample_index(std::vector<unsigned> weights)
{
	int res = 0;

	int acum = 0;
	int length = static_cast<int>(weights.size());
	for (int i = 0; i < length; i++) {
		acum += weights[i];
	}
	uniform_int_distribution<int> ui(0, acum - 1);
	//int r = static_cast<int>(1.0 * acum * rand() / (RAND_MAX + 1.0)); //Int random between 0 and total weight
	int r = ui(rng);
	int v = 0;
	for (int j = 0; j < length; j++) {
		if (v <= r && r < v + (int)weights[j]) {
			res = j;
			break;
		}
		v = v + weights[j];
	}


	return res;
}

float Game::random_real(float t)
{
	uniform_real_distribution<float> ui(0, t);
	return ui(rng);
}

//Returns a random row and column (passed as parameters by ref) using a matrix of weights
void Game::sample_rc(vector<vector<unsigned>> weights, unsigned& row, unsigned& col)
{
	row = 0;
	col = 0;
	int acum = 0;
	int length = static_cast<int>(weights.size());
	for (int i = 0; i < length; i++) {
		int length_i = static_cast<int>(weights[i].size());
		for (int j = 0; j < length_i; j++)
		{
			acum += weights[i][j];
		}
	}
	uniform_int_distribution<int> ui(0, acum - 1);

	int r = ui(rng);
	int v = 0;

	for (int i = 0; i < length; i++) {
		int length_i = static_cast<int>(weights[i].size());
		for (int j = 0; j < length_i; j++)
		{
			if (v <= r && r < v + (int)weights[i][j]) {
				row = i;
				col = j;
				return;
			}
			v = v + weights[i][j];
		}
	}
}




