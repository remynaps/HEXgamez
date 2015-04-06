#include <chrono>
#include <iostream>
#include <math.h>
#include <vector>
#include "include/ai.h"

using namespace std;

Ai::Ai(std::vector< std::vector< int > > &map): map(map)
{
}

void Ai::calculateMove(pair<int, int> coords, std::vector<pair<int, int>> connections)
{
    for(int i = 0; i < connections.size(); i++)
    {
        if(getRemainingMoves(connections.at(i)) < getRemainingMoves(coords))
        {
            
        }
    }
}

void Ai::getSurrounding()
{
    
}

int Ai::getRemainingMoves(pair<int, int> coords)
{
    return 0;
}
