/**
 * \file Weighted_values.h
 * \author [Your Name]
 * \date [Date]
 * \version [Version]
 *
 * \brief Header file for the Weighted_values class.
 */

#pragma once
#include <random>
#include <fstream>
#include <string>
#include <iostream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

using namespace std;


/**
 * \class Weighted_values
 * \brief Class for generating random values with weights.
 */
class Weighted_values
{
public:

	/**
	 * Table's name. By default is the name of the .csv file from which the values were loaded.
	 */
	string name;


	/**
	*  \~english @brief Loads values and their weights from \p file_path.
	* 
	 * Initialize values, weights and other constant values that are frequently used. Values and their weights are read from file \p path.
	 * 
	 * \param file_path 
	 * 
	 * \~spanish @brief Carga valores y sus pesos desde \p file_path.
	 * 
	 * Inicializa valores, pesos y otros valores constantes que se usan frecuentemente. Los valores y sus pesos se leen desde \p path.
	 * 
	 * \param file_path 
	 */
	void init(string file_path);

	/// <summary>
	/// Alternative way to initialize with values and weights
	/// </summary>
	/// <param name="values">Vector of integer values to be sampled.</param>
	/// <param name="weights">Vector of weights for each value.</param>
	/// <param name="name">Name to use in logs.</param>
	void init(vector<int> values, vector<unsigned> weights, string name);

	/**
	 * Returns a random value using weights. Values and weights are loaded and stored in private members.
	 * 
	 * \return 
	 */
	int sample(mt19937_64 &rng);

	/**
	 * Returns a random index using weights. Weights are loaded and stored in the object in private members.
	 * Index returned is between 0 and size of weights vector minus 1.
	 * 
	 * \return 
	 */
	int sample_index(mt19937_64& rng);

	vector<int> values ;

	void sample_without_replacement(mt19937_64& rng);
	int next_value(); 


	//Volver a pasar a modo privado:
	vector<unsigned> weights;
	vector<unsigned> cumulative_weight;
	unsigned total_weight = 0;

	/// <summary>
	/// Returns values and weights in json format.
	/// </summary>
	/// <returns></returns>
	json to_json();

	/// <summary>
	/// To store the last random value for log file.
	/// </summary>
	int last_random;

	/// <summary>
	/// It returns a string to log the result for a random value.
	/// </summary>
	/// <returns>A string in format: Mask:last_random:range. Mask is an identifier for random value, last_random is the last random result and range is the total weight.  </returns>
	string log();

	/// <summary>
	/// It returns a string to log the result for a random values without replacement.
	/// </summary>
	/// <param name="n">Number of items drawn</param>
	/// <returns></returns>
	string log_without_replacement(unsigned n);


	int sample_WO_cumulative(mt19937_64& rng);
	int sample_index_WO_cumulative(mt19937_64& rng);
	Weighted_values make_copy();

private:

	//After values and weights were laoded they should not be modified.
	

	//Draw without replacement...
	int i_value;
	vector<int> drawn_without_replacement;
	vector<unsigned> aux_weights;

	vector<int> last_random_without_replacement;
	vector<unsigned> last_total_weights_without_replacement;


	int length = 0; //initialized through calculate_params method.

	//Load weights and values, everything else is calculated. 
	void load_params(string file_path);
	void calculate_params();

};

