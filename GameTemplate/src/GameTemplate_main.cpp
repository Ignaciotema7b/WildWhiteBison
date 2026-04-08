//  ***************************************************************
//  GameTemplate - Creation date: 25/03/2026 13:36:03
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Company:  7 Bridges Maths
//  ***************************************************************

//  ***************************************************************
//  Defines used:
//  Log      --> #define LOG_PLAYS
//  Proto    --> #define LOG_PROTO
//  Synchro  --> #define WINS_SYNCHRO
//  
// This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <filesystem>
#include "../CommonLib/include/Game_info.h"

using namespace std;

#include "GameTemplate.h"


int main()
{
    GameTemplate* game = new GameTemplate() ;

    string game_name = "GameTemplate";

    clock_t begin, end;
    double time_spent;
    begin = clock();

    string folder_config = "game\\config";
    string folder_results = "game\\results";

    ifstream archivo(folder_config + "\\info.csv");

    //Verifico si encontro el archivo. Si los csv no estan en la carpeta config dentro del directorio de trabajo entonces los levanto de "\\config"
    //Localmente tengo una carpeta con todos los juegos, pero suele ser mas comodo correr el ejecutable desde la carpeta del juego.
    if (!archivo.good())
    {
        folder_config = "config";
        folder_results = "results";
    }
    
    Game_info info;

    info.load_info(folder_config);
    info.folder_config = folder_config;
    info.folder_results = folder_results;

    game->init(info);

    if (info.seed != 0)
    {
        game->rng.seed(info.seed);
        game->rng_seed = info.seed;
    }

    game->info = info;
    game->cap = info.cap;


    //Simular
    game->play(info.number_of_plays);


    //Execution time:
    end = clock();
    time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
    cout << "\n\nExecution time: " << time_spent << endl << endl << endl;


    cout << __DATE__ << endl;
    cout << __TIME__ << endl;

    system("PAUSE");

    return 0;
}


