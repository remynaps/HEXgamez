//=================================
// include guard
#ifndef __PLAYER_H_INCLUDED__
#define __PLAYER_H_INCLUDED__

//=================================
// included dependencies
#include <chrono>
#include <iostream>
#include <math.h>  
#include <stack>
#include "ai.h"

using namespace std;

//=================================
// the actual class
class Player
{
public:
    Player(string color, bool isCPU, int number);
    int number;
    int turn;
    bool isPlay;
    bool isCPU;
    bool isWinner; 
    Ai *ai;
    string color;
    stack<pair<int, int> > moves; 
    void makeMove();
    string getColor();
    pair<int, int> getInput(string input);
    pair<int, int> checkInput(string input);
    pair<int, int> createMove(string input);
    pair<int, int> setRandomMove(std::vector< std::vector< int > > &map);
    void saveMove(pair<int, int> move);
    string returnInput();
    pair<int,int> detMove(std::vector< std::vector< int > > &map);
    pair<int,int> detMove(std::vector< std::vector< int > > &map, string input);
    pair<int,int> setMinMax();
    vector<vector< vector< int > > >* getPosMoves(int playerNumber, vector< vector< int > > map);
    pair<int, vector< vector< int > >> minMax(vector< vector< int > > map, int depth, int maxDepth);
    pair<int, vector< vector< int > >> minMin(vector< vector< int > > map, int depth, int maxDepth);
    int evaluate(vector< vector< int > > map);
    int getOtherPlayer();
    vector<pair<int, int>> getConnections(int x, int y);
    pair<int,int> extractMove(vector< vector< int > > &map, vector< vector< int > > calcMap);
    int scorePath(vector< vector< int > > &map, vector<pair<int, int>> path);
    vector<pair<int, int>> getPath(pair<int,int> move, vector< vector< int > > &map);
    bool pathThread(int x, int y, int playerNumber, vector<pair<int, int>> &path,  vector<thread> &workers, vector< vector< int > > &map);
    pair<int,int>getEndPosition(int playerNumber, int position, vector< vector< int > > &map);
    bool startPath(int position, int playerNumber, vector< vector< int > > &map);
    bool buildPath(int x,int y, int playerNumber, vector<pair<int, int>> &path,  vector<thread> &workers, vector< vector< int > > &map);
    int monteCarlo(vector< vector< int > > &map, int seed);
    int monteThread(vector< vector< int > > &map);
    bool stepTaken(int x, int y, vector<pair<int,int>> &path, vector< vector< int > > &map);
    bool checkWinner(vector< vector< int > > &map);
    int getNumberMoves(vector< vector< int > > &map);
    void loader(int &number);
    bool determineWinner(int x, int y, int playerNumber, vector<pair<int, int>> &path, int &numberConnections, vector<thread> &workers ,vector< vector< int > > &map);
};

#endif // __PLAYER_H_INCLUDED__ 