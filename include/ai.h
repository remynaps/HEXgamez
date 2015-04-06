#ifndef __AI_H_INCLUDED__
#define __AI_H_INCLUDED__

#include <chrono>
#include <iostream>
#include <math.h>
#include <vector> 

using namespace std;

class Ai {
    std::vector< std::vector< int > > &map;
public:
    Ai(std::vector< std::vector< int > > &map);
    
    int getRemainingMoves(pair<int, int> coords);
    void calculateRoute();
    void calculateMove(pair<int, int> coords, std::vector<pair<int, int>> connections);
    void getSurrounding();
};


#endif // __AI_H_INCLUDED__ 
