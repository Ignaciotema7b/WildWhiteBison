/**
@file Symbol.h
* \~english
@brief This header defines the 'Symbol' class which represents a symbol in the system.
* \~spanish
@brief Este encabezado define la clase 'Symbol' que representa un s&iacute;mbolo en el sistema.
*/

#pragma once
#include <random>
using namespace std;

/**
 * @class Symbol
 * \~english
 * @brief The Symbol class represents a symbol with an ID, short name, and full name.
 * \~spanish
 * @brief La clase Symbol representa un s&iacute;mbolo con un ID, nombre corto de 3 caracteres como m&aacute;ximo y un nombre largo.
 */
class Symbol
{
public:
	/**
	* \~english
	* @brief Default constructor for creating an instance of Symbol with default values.
	*
	* The symbol_ID is usually set to zero, short_name and symbol_name are initialized empty strings.
	* \~spanish
	* @brief Constructor predeterminado para crear una instancia de Symbol con valores predeterminados.
	* 
	* El symbol_ID generalmente se establece en cero, short_name y symbol_name son inicializadas como cadenas vac&iacute;as .
	*/
	Symbol();

	/**
	* \~english
	* Destructor which cleans up the resources held by a Symbol object when it's destroyed.
	* \~spansih
	* Destructor que limpia los recursos retenidos por un objeto Symbol cuando se destruye.
	*/
	~Symbol();


	unsigned symbol_ID;		///< \~english The unique identifier of the symbol, usually starting from zero. \~spanish El identificador &uacute;nico del s&iacute;mbolo, normalmente empezando desde cero
	char short_name[4];    ///< \~english A shorter representation for the name of the symbol (up to 3 characters). \~spanish Una representaci&oacute;n m&aacute;s corta para el nombre del s&iacute;mbolo (hasta 3 caracteres)
	string symbol_name;    ///< \~english The full name or description of the symbol. \~spanish El nombre completo o descripci&oacute;n del s&iacute;mbolo.

};

