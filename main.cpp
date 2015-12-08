#include <chrono>
#include <iostream>
#include <math.h>
#include "include/hex.h"

using namespace std;



int main()
{
    srand(time(0));
    Game *game = new Game(11,11);
    game -> update();
    delete game;
}
