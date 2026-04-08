#pragma once
#include <iostream>
#include <random>
#include "Game_statistics.h"
#include "Game_info.h"

using namespace std;


 /**
  * @class Game
  * \~english
  * @brief The Game class implements methods to do random drawings, cap winnings, etc.
  * \~spanish
  * @brief La clase Game implementa m&eacute;todos para realizar sorteos aleatorios, cappear premios, etc.
  */
class Game
{
	public:
	/**
	* \~english
	* @brief Default constructor for creating an instance of Game with default values.
	* 
	* By default stake is initialized in 100.
	* \~spanish
	* @brief Constructor predeterminado para crear una instancia de Game con valores predeterminados.
	* 
	* Por defecto stake se inicializa en 100.
	*/
	Game();
	/**
	* \~english
	* @brief Destructor which cleans up the resources held by a Game object when it's destroyed.
	* \~spansih
	* @brief Destructor que limpia los recursos retenidos por un objeto Game cuando se destruye.
	*/
	~Game();


	/**
	* \~english 
	* @brief Initializes RNG and sets the stake for simulations.
	* @param stake Bet to use in Monte Carlo simulation.
	* \~spanish
	* @brief Inicializa el generador de n&uacute;meros aleatorios (RNG) y apuesta (stake) para usar en las simulaciones.
	* @param stake Apuesta para usar en simulaci&oacute;n de Monte Carlo.
	*/
	void init(unsigned stake);


	void saveRNGstate();

	void loadRNGstate();

	/**
	* \~english
	* @brief Game name.
	* \~spanish
	* @brief Nombre del juego.
	*/
	string game_name;

	/**
	* \~english
	* @brief Mersenne Twister pseudo-random number generator with 64 bits of state to use in simulations.
	* \~spanish
	* @brief Generador de n&uacute;meros pseudo-aleatorio de 64 bits de estado para usar en las simulaciones.
	*/
	mt19937_64 rng; 

	/**
	* \~english
	* @brief Returns a pseudo-random number in the range of 0 to \p n - 1.
	* 
	* @param n The upper bound for the random number generation.
	* 
	* This function generates a pseudo-random number using the Mersenne Twister pseudo-random number generator with 64 bits of state (mt19937_64).
	* The returned value will be an integer in the range [0, \p n - 1).
	* 
	* \~spanish
	* @brief Devuelve un n&uacute;mero pseudo-aleatorio en el rango de 0 a \p n - 1.
	* @param n El l&iacute;mite superior para la generaci&oacute;n de n&uacute;meros aleatorios.
	* 
	* Esta funci&oacute;n genera un n&uacute;mero pseudo-aleatorio utilizando el RNG Mersenne Twister con 64 bits de estado (mt19937_64).
	* El valor devuelto ser&aacute; un entero en el rango [0, \p n - 1).
	*/
	unsigned random(unsigned n);

	/**
	* \~english
	* @brief Performs a weighted random sampling of values based on their probabilities.
	* 
	* @details This method implements a weighted random sampling, selecting an outcome
	*          from a set of values according to their associated probabilities.
	* 
	* @note It is recommended to use methods from the Weighted_values class for
	*       better performance in extensive sampling operations.
	* 
	* @param x Vector of possible outcomes (integer values)
    * @param weights Vector of weights corresponding to each outcome. Must have the same length as x
	* 
	* @pre The x and weights vectors must have the same length
	* @pre All values in weights must be greater than zero
	* 
	* @return Randomly selected sampled value according to the weight distribution
	* 
	* @see Weighted_values
	* \~spanish 
	* @brief Genera un muestreo aleatorio de valores basado en sus probabilidades.
	* @details Este m&eacute;todo implementa un muestreo aleatorio ponderado, seleccionando un resultado de un conjunto de valores seg&uacute;n sus probabilidades asociadas.
	* 
	* @note Se recomienda utilizar los m&eacute;todos de la clase Weighted_values para obtener un mejor rendimiento en operaciones de muestreo extensivas.
	* 
	* @param x Vector de resultados posibles (valores enteros)
    * @param weights Vector de ponderaciones correspondientes a cada resultado
	*				Debe tener la misma longitud que x
    *
	*  @pre Los vectores x y weights deben tener la misma longitud
	* @pre Todos los valores en weights deben ser mayores que cero
	* 
    * @return Valor muestreado seleccionado aleatoriamente seg&uacute;n la distribuci&oacute;n de ponderaciones
	* 
	* @see Weighted_values
	*/
	int sample(std::vector<int> x, std::vector<unsigned> weights);

	/**
	* \~english
	* @brief Generates a random index based on a weight distribution
	* @details This method selects a random index from a vector of weights,
	*          where the probability of selecting each index is proportional to its weight.
	* 
	*  @note Unlike sample(std::vector<int> x, std::vector<unsigned> weights), this method returns the index of the weights vector
	*       instead of a specific value.
	*  @param weights Vector of weights determining the selection probability of each index
	* 
	* @pre All values in weights must be greater than zero
	* @pre The weights vector must not be empty
	* 
	* @return Randomly selected index, based on the weight distribution
	* 
	* @see sample()
	* 
	* \~spanish
	* @brief Genera un &iacute;ndice aleatorio basado en una distribuci&oacute;n de ponderaciones (pesos)
	* @details Este m&eacute;todo selecciona un &iacute;ndice aleatorio de un vector de pesos,
	*          donde la probabilidad de selecci&oacute;n de cada &iacute;ndice es proporcional a su peso.
	* @note A diferencia de sample(std::vector<int> x, std::vector<unsigned> weights), este m&eacute;todo devuelve el &iacute;ndice del vector de pesos
	*       en lugar de un valor espec&iacute;fico.
	* 
	* @param weights Vector de pesos que determina la probabilidad de selecci&oacute;n de cada &iacute;ndice
	* 
	* @pre Todos los valores en weights deben ser mayores que cero
	* @pre El vector weights no debe estar vac&iacute;o
	* @return &iacute;ndice seleccionado aleatoriamente, basado en la distribuci&oacute;n de pesos.
	* @see sample()
	*/
	int sample_index(std::vector<unsigned> weights);

	/**
	* \~english 
	*  @brief Generates a random real number in the interval [0, t]
	* @details This method produces a uniformly distributed floating-point number
	*          in the range from zero up to the specified value.
	* 
	* @note Uses a continuous uniform distribution for generation
	* 
	* @param t Upper limit of the range of generated random numbers
	* 
	* @pre t must be greater than or equal to zero
	* 
	* @return A random floating-point number in the interval [0, t]
	* 
	* @warning If t is negative, the behavior is undefined
	* 
	* \~spanish
	* @brief Genera un n&uacute;mero aleatorio real en el intervalo [0, t]
	* @details Este m&eacute;todo produce un n&uacute;mero flotante aleatorio uniformemente distribuido
	*          en el rango desde cero hasta el valor especificado.
	* 
	* @note La generaci&oacute;n utiliza una distribuci&oacute;n uniforme continua.
	* 
	* @param t L&iacute;mite superior del rango de n&uacute;meros aleatorios generados
	* 
	* @pre t debe ser mayor o igual a cero
	* 
	* @return Un n&uacute;mero flotante aleatorio en el intervalo [0, t]
	* 
	* @warning Si t es negativo, el comportamiento es indefinido
	*/
	float random_real(float t);

	/**
	* \~english
	* @brief Randomly selects a row and column based on a weight matrix
	* @details This method generates a random row and column using a two-dimensional weight matrix, 
	*          where the probability of selecting each cell is proportional to its corresponding weight.
	*
	* @note Row and column parameters are modified by reference to return the result
	* 
	* @param weights Matrix of weights determining the selection probability of each cell
	* @param[out] row Reference to the variable that will receive the selected row index
	* @param[out] col Reference to the variable that will receive the selected column index
	* 
	* @pre The weights matrix must not be empty
	* @pre All weights in the matrix must be greater or equal to zero
	* 
	* @warning If the weights matrix is empty, the behavior is undefined
	* 
	* \~spanish
	* @brief Selecciona aleatoriamente una fila y columna bas&aacute;ndose en una matriz de pesos
	* @details Este m&eacute;todo genera una fila y columna aleatorias utilizando una matriz bidimensional 
	*			de pesos, donde la probabilidad de selecci&oacute;n de cada celda es proporcional
	*			a su peso correspondiente.
	* 
	* @note Los par&aacute;metros de fila y columna se modifican por referencia para devolver el resultado
	* 
	* @param weights Matriz de pesos que determina la probabilidad de selecci&oacute;n de cada celda
	* @param[out] row Referencia a la variable que recibir&aacute; el &iacute;ndice de fila seleccionado
	* @param[out] col Referencia a la variable que recibir&aacute; el &iacute;ndice de columna seleccionado
	* 
	* @pre La matriz weights no debe estar vac&iacute;a
	* @pre Todos los pesos en la matriz deben ser mayores o iguales a cero
	*/
	void sample_rc(vector<vector<unsigned>> weights, unsigned& row, unsigned& col);

	/**
	* \~english
	* @brief Stake value in credits. by default is set to 100.
	* \~spanish
	* @brief Valor de la apuesta en cr&eacute;ditos. Por defecto se inicializa en 100.
	*/
	unsigned stake = 100;

	/**
	* \~english
	* @brief Represents a maximum possible win in credits
	* @details Variable defining the upper limit of winnings, 
	*          using an unsigned 64-bit integer type to handle 
	*          a wide range of values.
	* \~spanish
	*  @brief Representa la m&aacute;xima ganancia posible en cr&eacute;ditos
	* @details Variable que define el l&iacute;mite m&aacute;ximo de ganancias, 
	*          utilizando un entero sin signo de 64 bits para manejar 
	*          un amplio rango de valores.
	*/
	unsigned long long cap;

	/**
	 * @brief Almacena la semilla para inicializar el generador de n&uacute;meros aleatorios (RNG)
	 * @details Valor utilizado para inicializar el generador de n&uacute;meros pseudoaleatorios,
	 *          lo que permite la reproducibilidad de las secuencias aleatorias generadas.
	 *
	 * \~english
	 * @brief Stores the seed that initializes the RNG
	 * @details Value used to initialize the pseudo-random number generator,
	 *          enabling reproducibility of generated random sequences.
	 */
	unsigned long long rng_seed;


	/**
	* @brief Funci&oacute;n para ajustar las ganancias de un juego.
	*
	* @details Esta funci&oacute;n es "variadic" (n&uacute;mero variable de argumentos), ajusta la suma de todos los valores pasados como argumentos por referencia a partir del primero.
	*
	* Si el primer valor es mayor que cap (Game::cap) por la apuesta (Game::stake) entonces cambia este valor por cap por la apuesta y el resto de los argumentos se establecen en 0. De lo contrario,
	* verifica si la suma del primer valor m&aacute;s el segundo es mayor que cap por la apuesta, si es as&iacute; 
	* entonces mantiene el valor para el primer argumento, cambia el segundo valor a cap por la apuesta menos el primer valor y establece el resto de los valores en 0.
	* Y as&iacute; sucesivamente.
	*
	* @warning Es muy importante el orden de los argumentos ya que capear&aacute; los primeros argumentos que juntos suman m&aacute;s que cap por la apuesta y establecer&aacute; el resto en 0.
	*
	* @param first El primer valor de las ganancias.
	* @param ... Los restantes valores de las ganancias.
 
	* 
	* 
	* \~english
	* @brief Function to cap the wins of a game.
	*
	* @details This functions is "variadic" (variable number of arguments), caps the sum of all the values passed as arguments 
	* by reference starting for the first one. 
	* If the first value is greater than the cap (Game::cap) times stake (Game::stake) then it changes this value by cap times stake and 
	* the rest of the arguments are set to 0. Otherwise, it checks if the sum of the first value plus the second is greater than cap times stake, 
	* if this is the case then it keeps the value for the first argument, changes the second value to cap * stake minus the first value, and sets the rst of the values to 0.
	* And so on.
	* 
	* @warning It is very important the order of the arguments as it will cap the first arguments that together add more than cap * stake and will set the rest to 0.
	* 
	* @param first The first win value.
	* @param rest The rest of win values.
	*/
	template <typename T, typename... Ts>
	void cap_wins(T& first, Ts&... rest) {
		adjust_values_impl(first, rest...);
	}





	Game_info info;

private:

	template <typename T, typename unsigned>
	void adjust_values_impl(T& first) {
		if (first > cap * info.bet_for_cap) {
			first = cap * info.bet_for_cap;
		}
	}

	template <typename T, typename... Ts>
	void adjust_values_impl(T& first, Ts&... rest) {
		if (first > cap * info.bet_for_cap) {
			first = cap * info.bet_for_cap;
			(..., (rest = 0)); // Sets the rest of values in 0
		}
		else {
			T sum = first;
			bool cap_exceeded = false;
			auto adjust_rest = [&](T& val) {
				if (cap_exceeded) {
					val = 0;
				}
				else {
					sum += val;
					if (sum > cap * info.bet_for_cap) {
						val = cap * info.bet_for_cap - (sum - val);
						cap_exceeded = true;
					}
				}
				};
			(..., adjust_rest(rest));
		}
	}


};

