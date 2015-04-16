#ifndef __AI_H_INCLUDED__
#define __AI_H_INCLUDED__

#include <chrono>
#include <iostream>
#include <math.h>
#include <vector> 

using namespace std;

class Ai
{
public:
    unsigned int globalSeed;
    int number;
    int minax;
    int monteRepeat;
    Ai(int number);

    void aiMenu();

    pair<int,int> detMove(std::vector< std::vector< int > > &map);
    vector<vector< vector< int > > >*  getPosMoves(int playerNumber, vector< vector< int > > map);
    pair<int, vector< vector< int > >> minMax(vector< vector< int > > map, int depth, int maxDepth);
    pair<int, vector< vector< int > >> minMin(vector< vector< int > > map, int depth, int maxDepth);
    int monteCarlo(vector< vector< int > > &map);
    int monteThread(vector< vector< int > > &map);
    bool checkWinner(vector< vector< int > > &map);
    pair<int,int> getEndPosition(int playerNumber, int position, vector< vector< int > > &map);
    bool startPath(int position, int playerNumber, vector< vector< int > > &map);
    bool buildPath(int currentX, int currentY, int playerNumber, vector<pair<int, int>> &path, vector<thread> &workers, vector< vector< int > > &map);
    bool determineWinner(int x, int y, int playerNumber, vector<pair<int, int>> &path, int &numberConnections, vector<thread> &workers ,vector< vector< int > > &map);
    bool stepTaken(int x, int y, vector<pair<int,int>> &path, vector< vector< int > > &map);
    int getNumberMoves(vector< vector< int > > &map);
    vector<pair<int, int>> getConnections(int x, int y);
    int getOtherPlayer();

};


#endif // __AI_H_INCLUDED__ 
