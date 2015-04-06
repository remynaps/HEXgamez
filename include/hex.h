
//=================================
// include guard
#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__

//=================================
// included dependencies
#include <chrono>
#include <iostream>
#include <math.h>
#include <vector> 
#include "map.h"
#include "view.h"

using namespace std;
//=================================
// the actual class
class Game
{
    Map *map;

    int width, height;
        public:
            Game(int width,int height);
            void setValues (int,int);
            int getArea(); 
            void createBoard();
            void setMove(pair<int, int> coords, std::vector< std::vector< int > > &map);
            void getWinner();
            string generateMenu();
            int startGame();
            int selectGameMode(string input);
            void update();
            int stringToInt(string input);
            int startingPLayer();
};

#endif // __GAME_H_INCLUDED__ 