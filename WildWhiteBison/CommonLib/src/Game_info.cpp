#include "..\include\Game_info.h"

void Game_info::load_info(string path)
{
    //Por defecto si no encuentra el starting balance en info.csv (se mantiene en 0) luego de cargar todos los campos lo inicializo en 100x
    starting_balance = 0;

    ifstream archivo(path + "\\info.csv");

    //Leer linea por linea y guardar los campos 

    if (!archivo.good()) //Verifico si el archivo fue bien levantado.
    {
        cout << "Warning: info.csv not found...\n";
        cout << path<<endl;
        system("PAUSE");
        //return 0LLU;
    }

    string str; //Acá voy a guardar lo que leo del archivo.
    getline(archivo, str, '\n');//La función getline lee con tres parámetros(de que archivo,donde lo guardo,hasta donde leo).

    //La función str.find(",") lee el str y guarda el lugar donde encuentre una coma.
    string campo = str.substr(0, str.find(",")); //Genero un substr, asta antes de la coma. Ese es el campo.


    while (!archivo.eof())
    {
        if (campo == "Type")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            type = str;
        }

        if (campo == "Game name")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            game_name = str;
        }

        if (campo == "Version")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            version = str;
        }

        if (campo == "Mode")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            mode = str;
        }

        if (campo == "Timestamp")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            timestamp = stoull(str);
        }

        if (campo == "Spins base")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            spins_base = stoul(str);
        }

        if (campo == "Spins purchase")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            spins_purchase = stoul(str);
        }

        if (campo == "Slingo numbers")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            slingo_numbers = stoul(str);
        }

        if (campo == "Simulation plays")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            number_of_plays = stoull(str);
        }

        if (campo == "Number of reels")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            n_reels = stoull(str);
        }

        if (campo == "Number of trackers")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            number_of_trackers = stoull(str);
        }

        if (campo == "Number of rows")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            number_of_rows = stoul(str);
        }

        if (campo == "Stake")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            bet = stoul(str);
        }

        if (campo == "Bet for cap")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            bet_for_cap = stoul(str);
        }

        if (campo == "Cap")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            cap = stoull(str);
        }

        if (campo == "Seed")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            seed = stoull(str);
        }

        if (campo == "Plays to print")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            plays_to_print = stoull(str);
        }

        if (campo == "Starting balance")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo
            starting_balance = stoull(str);
        }

        if (campo == "Reels heights")
        {
            str.erase(0, str.find(",") + 1); //Borro el campo y me quedo solo con el valor del campo

            string temp;
            stringstream ss(str);
            while (getline(ss, temp, ',')) {
                if (!temp.empty()) {  // Importante para evitar el ultimo valor vacio
                    reel_heights.push_back(stoi(temp));
                }
            }
            
        }

        getline(archivo, str, '\n');
        campo = str.substr(0, str.find(","));
    }

    if (starting_balance == 0)
        starting_balance = 100 * bet;

    json_info["type"] = type;
    json_info["cap"] = cap;
    json_info["seed"] = seed;
    json_info["bet"] = bet;
    json_info["bet_for_cap"] = bet_for_cap;
    json_info["n_rows"] = number_of_rows;
    json_info["n_reels"] = n_reels;
    json_info["n_trackers"] = number_of_trackers;
    json_info["game_name"] = game_name;
    json_info["version"] = version;

    json_info["mode"] = mode;
    json_info["reels_heights"] = reel_heights;

    json_info["starting_balance"] = starting_balance;

    archivo.close();

}
