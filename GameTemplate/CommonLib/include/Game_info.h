#pragma once
#include <iostream>
#include <vector>
#include <fstream> 
#include <string>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

/**
 * @class Game_info
 * \~english
 * @brief The Game class implements methods to do random drawings, cap winnings, etc.
 * \~spanish
 * @brief La clase Game implementa m&eacute;todos para realizar sorteos aleatorios, cappear premios, etc.
 */
class Game_info
{
public:


	/**
	* \~english
	* @brief Type of game, such as Paylines, Ways, Cluster, etc., which determines the mechanics used to evaluate winning combinations.
	* 
	* \~spanish
	* @brief Tipo de juego, tal como Paylines, Ways, Cluster, etc., el cual determina la mec&aacute;nica usada para evaluar las combinaciones pagadoras.
	*/
	string type;

	/**
	* \~english
	* @brief Folder path for the game configuration files.
	*
	* The files in this folder are csv files with the configuration parameters for the game, such as reel sets, paytable, paylines, 
	*	weights and values for random drawings, etc. In this folder there is also a file named info.csv with basic information 
	*	such as game name, number of reels, number of rows, number of plays for simulation, etc.
	* 
	* @warning If info.csv is not found then the simulation fails to start.
	* 
	* \~spanish
	* @brief Directorio para los archivos de configuraci&oacute;n del juego.
	* 
	* Los archivos en esta carpeta son archivos csv con los par&aacute;metros de configuraci&oacute;n del juego, tales como reel sets, 
	*	tabla de pagos, l&iacute;neas de pago, pesos y valores para sorteos aleatorios, entre otros. 
	*	En esta carpeta tambi&eacute;n hay un archivo llamado info.csv con informaci&oacute;n b&aacute;sica, 
	*	como el nombre del juego, el n&uacute;mero de ruedas, el n&uacute;mero de filas, el n&uacute;mero de jugadas para la simulaci&oacute;n, etc.
	* 
	* @warning El archivo info.csv es necesario para comenzar la simulaci&oacute;n.
	*/
	string folder_config;

	/**
	* \~english
	* @brief Folder path for the game simulation results files.
	*
	* The simulation results files are JSON files.
	* The generated file should be named as follows: 
	*	_rtp_[rtp]__[finish_time]__[info.game_name]_v[info.version].json. 
	*	This naming convention ensures that the file contains information about the return to player (RTP) value, 
	*	the game name, and the version of the game, as well as a timestamp for the simulation finish time. 
	*	The resulting file is saved in the info.folder_results directory.
	* 
	* The files should contain all the information about game configuration parameters, basic information and simulation results.
	
	* \~spanish
	* @brief Directorio para los archivos de resultados de simulaci&oacute;n del juego.
	* 
	* Los archivos de resultados son archivos JSON.
	* Los archivos generados deben nombrarse de la siguiente forma:
	*	_rtp_[rtp]__[finish_time]__[info.game_name]_v[info.version].json. 
	*	Esta convenci&oacute;n garantiza que el nombre del archivo contenga informaci&oacute;n sobre el valor del RTP,
	*	el nombre y versi&oacute;n del juego como as&iacute; tambi&eacute;n timestamp del tiempo de finalizaci&oacute;n de la simulaci&oacute;n.
	* 
	* Los archivos deben contener toda la informaci&oacute;n acerca de los par&aacute;metros de configuraci&oacute;n, informaci&oacute;n b&aacute;sica
	*	y resultados de la simulaci&oacute;n.
	*/
	string folder_results;
	
	/**
	* \~english
	* @brief String to store the game name.
	* 
	* The content of this string is used for naming the JSON file containing the simulation's outcomes. 
	*	It is also stored in the field info["game_name"] of the results file.
	* 
	* \~spanish
	* @brief  El contenido de esta cadena se utiliza para dar nombre al archivo JSON que contiene los resultados de la simulaci&oacute;n.
	* Adem&aacute;s, es guardado en el campo info["game_name"] del archivo de resultados.
	*
	*
	*/
	string game_name;

	/**
	* \~english
	* @brief Version number of game configuration.
	* 
	* Its value is set in xlsm file and it is loaded through game_info.csv file and
	*	it is stored in the field info["version"] of the results file.
	* 
	* The content of this string is used in the name of the JSON file containing the simulation's outcomes. 
	*	
	*
	* \~spanish
	* @brief N&uacute;mero de versi&oacute;n de configuraci&oacute;n del juego.
	*
	* Su valor es establecido en el archivo xlsm y cargado a trav&eacute;s del archivo game_info.csv
	* Se guarda en el campo info["version"] del archivo de resultados.
	*  El contenido de esta cadena se utiliza en el nombre al archivo JSON que 
	*	contiene los resultados de la simulaci&oacute;n.
	* 
	*/
	string version;

	/**
	* \~english
	* @brief Stores the UNIX timestamp of the moment when simulation finishes
	* 
	* @details Captures the simulation's completion time as seconds 
	*          elapsed since the UNIX epoch (January 1, 1970, 00:00:00 UTC), 
	*          using a 64-bit unsigned integer to ensure wide time range.
	* 
	* @note It is not in the JSON file with results, it is only in the name of the file. 
	* \~spanish
	* 
	* @brief Almacena el timestamp UNIX del momento en que finaliza la simulaci&oacute;n
	* @details Captura el tiempo de finalizaci&oacute;n de la simulaci&oacute;n como segundos 
	*          transcurridos desde la &eacute;poca UNIX (1 de enero de 1970, 00:00:00 UTC), 
	*          utilizando un entero sin signo de 64 bits para garantizar un amplio rango temporal.
	* @note Este valor no est&aacute; en el archivo JSON con los reusltados, est&aacute; solo en el nombre del archivo.
	*/
	unsigned long long timestamp;

	/**
	* \~english
	* @brief Number of plays to simulate.
	* 
	* @details Specifies the number of plays or iterations to be executed 
	*          during the simulation, determining when the simulation should end. 
	*          The simulation stops once this number of plays is reached.
	* \~spanish
	* @brief N&uacute;mero de juegos a simular.
	* 
	* @details Especifica el n&uacute;mero de jugadas o iteraciones que se ejecutar&aacute;n 
	*          durante la simulaci&oacute;n, determinando cu&aacute;ndo la simulaci&oacute;n debe finalizar. 
	*          La simulaci&oacute;n se detiene una vez que se alcanza este n&uacute;mero de jugadas. 
	* 
	*
	*/
	unsigned long long number_of_plays;

	/**
	* \~english
	* @brief Number of reels. It does not include horizontal reels (trackers).
	* 
	* \~spanish
	* @brief N&uacute;mero de reels. No tiene en cuenta los reels horizontales (trackers).
	*
	*/
	unsigned n_reels;

	/**
	* \~english
	* @brief Maximum number of rows in the screen.
	*
	* @details If the screen is not rectangular this number represents the maximum number of symbols visible
	* in a reel. Internally, the screen is represented as a rectangular array named \p screen, its height
	*	is \p number_of_rows and the width is \p n_reels.
	* 
	* For each reel the number of cells used is the number of visible symbols in the reel starting from row 0.
	* 
	* \~spanish
	* @brief M&aacute;ximo n&uacute;mero de filas en la pantalla.
	* 
	* @details Si la pantalla no es rectangular este n&uacute;mero representa la m&aacute;xima cantidad de 
	*	s&iacute;mbolos visibles en un reel. Internamente la pantalla se representa como un arreglo rectangular
	*	llamado \p screen, la altura de \p screen es \p number_of_rows y el ancho es \p n_reels. 
	* 
	*	Para cada reel se usan tantas celdas como s&iacute;mbolos visibles aparezcan en el reel empezando desde
	*	la fila 0.
	*
	*/
	unsigned number_of_rows;


	unsigned number_of_trackers;
	unsigned bet;
	unsigned bet_for_cap;

	unsigned long long cap;
	unsigned long long seed = 0;

	unsigned plays_to_print;

	string mode;

	//Sessions analysis
	unsigned starting_balance;

	//Reels sizes
	vector<int> reel_heights;


	//Slingos
	unsigned spins_base = 0;
	unsigned spins_purchase = 0;
	unsigned long slingo_numbers = 75;


	//Read info.csv
	void load_info(string path);

	//JSON to print info in output file
	json json_info;

};

