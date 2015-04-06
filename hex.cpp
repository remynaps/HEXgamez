#include <chrono>
#include <iostream>
#include <math.h>  
#include <sstream>
#include <string>
#include "include/map.h"
#include "include/hex.h"
#include "include/view.h"

using namespace std;

Game::Game(int width, int height)
{
    this->width = width;
    this->height = height;
}

void Game::setValues (int x, int y) {
}

void Game::getWinner(){
}

string Game::generateMenu()
{
    //int gameMode;

    string input;

    cout << "__HEX GAME__" << endl;
    cout << endl << endl;

    cout << "availiable game modes:" << endl;

    cout << "PLAYER VS PLAYER" << endl;

    cout << "PLAYER VS CPU" << endl << endl;

    cout << "select 1 for player vs player" << endl << endl;

    cout << "select 2 for player vs cpu" << endl << endl;

    cout << "select gameMode: ";

    cin >> input;

    return input;
}

int Game::stringToInt(string input)
{
    string myStream = input;
    istringstream buffer(myStream);
    int value;
    buffer >> value;

    return value;
}

int Game::selectGameMode(string input)
{
    cout << "\x1b[2J\x1b[1;1H" << std::flush;
    return stringToInt(input);
}

int Game::startGame()
{
    bool dontStart = true;

    int gameMode;

    while(dontStart)
    {
        gameMode = selectGameMode(generateMenu());

        if(gameMode == 1 || gameMode == 2)
        {
            dontStart = false;
        }
    }
    return gameMode;
}

int Game::startingPLayer()
{

    bool dontStart = true;
    int startPlayer;
    while(dontStart)
    {
        string input;

        cout << "            __HEX GAME__" << endl;
        cout << endl << endl;

        cout << "select starting Player :";

        cin >> input;

        startPlayer = stringToInt(input);
        if(startPlayer == 1 || startPlayer == 2)
        {
            dontStart = false;
        }
    }
    return startPlayer;
}

void Game::update()
{
    int gameMode = startGame();

    int startingPlayer = startingPLayer();

    map = new Map(height,width, gameMode, 1, startingPlayer);

    while(!(map->hasWinner))
    {   
        map->update();
    }
}











