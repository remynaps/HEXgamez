#include <chrono>
#include <iostream>
#include <math.h>
#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include <future>
#include "include/player.h" 
#include "include/ai.h"

using namespace std;

Ai::Ai(int number)
{
     globalSeed = time(0);
     this->number = number;
     aiMenu();
}

void Ai::aiMenu()
{
    int setting;
    bool valid = false;
    cout << "Set the ai mode" << endl << endl;
    cout << "1: tournament mode(10 monte carlo per thread and 2 depth)" << endl;
    cout << "2: new mode(250 monte carlo per thread and 1 depth)" << endl;
    while(!valid)
    {
        cin >> setting;
        switch(setting)
        {
            case 1:
                monteRepeat = 10;
                minax = 2;
                valid = true;
                break;
            case 2:
                monteRepeat = 250;
                minax = 1;
                valid = true;
                break;
            default:
                cout << "nope" << endl;
                break;
        }
    }
}

//return the other player.
//this is needed for the montecarlo simulation to not have to use multiple if statements.
int Ai::getOtherPlayer()
{
    int otherNumber;
    switch(number)
    {
        case 1:
            otherNumber = 2;
            break;
        case 2:
            otherNumber = 1;
            break;
    }
    return otherNumber;
}

//return all possible connection a 'node' can have.
vector<pair<int, int>> Ai::getConnections(int x, int y)
{
    std::vector<pair<int, int>> moves;
    moves.push_back(make_pair(x-1,y));
    moves.push_back(make_pair(x-1,y+1));
    moves.push_back(make_pair(x,y-1));
    moves.push_back(make_pair(x,y+1));
    moves.push_back(make_pair(x+1,y-1));
    moves.push_back(make_pair(x+1,y));

    return moves;
}

//count the empty 'nodes' on a given map and return that value
int Ai::getNumberMoves(vector< vector< int > > &map)
{
    int movesCount = 0;
    for(int i = 0; i < map.size(); i++)
    {
        for(int j = 0; j < map.size(); j++)
        {
            if(map[i][j] == 0)
            {
                movesCount++;
            }
        }
    }
    return movesCount;
}

/*
a heavier version of the getNumberMoves() method.
this method initially gets all the moves that dont have a player connected to them
and returns them in an array consisting of the corresponding gamestate.
*/
vector<vector< vector< int > > >*  Ai::getPosMoves(int playerNumber, vector< vector< int > > map)
{
    vector<vector< vector< int > > > *moves = new vector<vector< vector< int > > >();
    vector< vector< int > > temp;

    for(int i = 0; i < map.size(); i++)
    {
        for(int j = 0; j < map.size(); j++)
        {
            if(map[i][j] == 0)
            {
                temp = map;
                temp[i][j] = playerNumber;
                moves->push_back(temp);
            }
        }
    }
    return moves;
}

//returns the position that is required for the player to win the game.
pair<int,int> Ai::getEndPosition(int playerNumber, int position, vector< vector< int > > &map)
{
    pair<int,int> redEnd;
    pair<int,int> blueEnd;
    switch(playerNumber)
    {
        case 1:
            redEnd = make_pair(map[position].size()-1,position);
            return redEnd;
        case 2:
            blueEnd = make_pair(position,map[position].size()-1);
            return blueEnd;
    }
    throw invalid_argument("playerNumber does not exist");
}

/*
---------------------------------------------------------------------------------------------------
the next part of the ai class covers the minimax algorithm and the monte carlo simulation.
it also covers the multithread implemntation of monte carlo.
---------------------------------------------------------------------------------------------------
*/


//the maximiser of minimax. it uses alpha beta pruning to not have to search nodes that wont meet 
//expectations anyway.
//the algorithm itself is modified to return a pair of the score of the move, and the move itself.
pair<int, vector< vector< int > >> Ai::minMax(vector< vector< int > > map, int depth, int maxDepth) {

    vector< vector< int > > bestMap;

    if (depth >= maxDepth)
    {
        return make_pair(monteThread(map), map);
    }
    int max = -100000;

    vector<vector< vector< int > > > *moves = getPosMoves(number, map);

    for (vector<vector< vector< int > > >::iterator it = moves->begin() ; it != moves->end(); ++it)
    {
        pair<int, vector< vector< int > >> mini = minMin(*it, depth+1, maxDepth);
        int min = std::get<0>(mini);

        if (min > max)
        {
            max = min;
            bestMap = *it;
        }
    }
    return make_pair(max, bestMap);
}


//same as the above. this is the minimizer
pair<int, vector< vector< int > >> Ai::minMin(vector< vector< int > > map, int depth, int maxDepth) {

    vector< vector< int > > bestMap;

    if (depth >= maxDepth)
    {
        return make_pair(monteThread(map), map);
    }
    int min = 100000;

    vector<vector< vector< int > > > *moves = getPosMoves(getOtherPlayer(), map);

    for (vector<vector< vector< int > > >::iterator it = moves->begin() ; it != moves->end(); ++it)
    {
        pair<int, vector< vector< int > >> maxi = minMax(*it, depth+1, maxDepth);
        int max = std::get<0>(maxi);

        if (max < min)
        {
            min = max;
            bestMap = *it;
        }
    }
    return make_pair(min, bestMap);
}

/*
    the monte carlo method.
    the method runs x monte carlo simulations.
    this is because the minimax algorithm in combination with this method takes a long time to complete.
*/
int Ai::monteCarlo(vector< vector< int > > &map)
{
    int movesCount = getNumberMoves(map);
    //start montecarlo simulation and keep track of the number of times you win
    int timesWon = 0;
    int playerNumber = number;
    for(int k = 0; k < monteRepeat; k++)
    {
        vector< vector< int > > mapCopy = map;
        int count = 0;
        while(count < movesCount)
        {
            playerNumber = number;
            if(count % 2 == 0)
            {
                playerNumber = getOtherPlayer();
            }

            int x = rand_r(&globalSeed) % map.size();
            int y = rand_r(&globalSeed) % map.size();
            if(mapCopy[x][y] == 0)
            {
                mapCopy[x][y] = playerNumber;
                count++;
            }
        }
        if(checkWinner(mapCopy))
        {
            timesWon++;
        }
        else
        {
            timesWon--;
        }
    }
    return timesWon;
}

/*
    method to multithread the monte carlo simulation.
    resulting in a totoal of x simulations run for every possible move the minimax algorithm comes up with.
*/
int Ai::monteThread(vector< vector< int > > &map)
{
    int score = 0;
    auto result1( std::async(launch::async,&Ai::monteCarlo,this, ref(map)));
    auto result2( std::async(launch::async,&Ai::monteCarlo,this, ref(map)));
    auto result3( std::async(launch::async,&Ai::monteCarlo,this, ref(map)));
    auto result4( std::async(launch::async,&Ai::monteCarlo,this, ref(map)));

    score += result1.get();
    score += result2.get();
    score += result3.get();
    score += result4.get();

    return score;
}




//-------------------------------------------------------------------------------------
// Modified version of the pathfinding found in the map method to work with the monte carlo simulation
//-------------------------------------------------------------------------------------

bool Ai::checkWinner(vector< vector< int > > &map)
{
    bool winner = false;
    bool winner2 = false;
    for(int i = 0; i < map.size(); i++)
    {
        if(map[i][map[i].size() -1 ] == 1)
        {
            winner = startPath(i, 1, map);
            if(winner)
            {
                break;
            }
        }
        if(map[map[i].size() -1 ][i] == 2)
        {
            winner2 = startPath(i, 2, map);
            if(winner2)
            {
                break;
            }
        }
        //cout << winner2 << ":::::" << winner1 << endl;
    }
    if((winner && 1 == number) || (winner2 && 2 == number))
    {
        return true;
    }
    return false;
}

//start calculating a [ath when a point is located on the end position and on the start position
bool Ai::startPath(int position, int playerNumber, vector< vector< int > > &map)
{
    vector<pair<int, int>> path;
    vector<thread> workers;

    pair<int, int> endPosition = getEndPosition(playerNumber, position, map);
    int endX = std::get<0>(endPosition);
    int endY = std::get<1>(endPosition);

    path.push_back(endPosition);

    bool winner = buildPath(endY,endX, playerNumber, path, workers,map);

    return winner;
}

//actually build the path recursivly
bool Ai::buildPath(int currentX, int currentY, int playerNumber, vector<pair<int, int>> &path, vector<thread> &workers, vector< vector< int > > &map)
{
    vector<pair<int, int>> connections = getConnections(currentX,currentY);
    int numberConnections = 0;
    for(int i = 0; i < connections.size(); i++)
    {
        int x = std::get<0>(connections[i]);
        int y = std::get<1>(connections[i]);
        if(x < map.size() && x >= 0 && y < map.size() && y >= 0)
        {
            bool hasWinner =  determineWinner(x,y,playerNumber,path, numberConnections, workers,map);
            if(hasWinner)
            {
                return true;
            }
        }
    }
    return false;
}

//determine the winner of the game and quit the game.
bool Ai::determineWinner(int x, int y, int playerNumber, vector<pair<int, int>> &path, int &numberConnections, vector<thread> &workers ,vector< vector< int > > &map)
{
    if(map[x][y] == playerNumber && !stepTaken(x,y,path,map))
    {
        path.push_back(make_pair(x,y));

        if((playerNumber == 1 && y == 0) || (playerNumber == 2 && x == 0))
        {
            return true;
        }
        else
        {
            return buildPath(x, y, playerNumber, path, workers,map);
        }
    }
    return false;
}

bool Ai::stepTaken(int x, int y, vector<pair<int,int>> &path, vector< vector< int > > &map)
{
    for(int j = 0; j < path.size(); j++)
    {
        pair<int,int> takenStep = path[j];

        int takenX = std::get<0>(takenStep);
        int takenY = std::get<1>(takenStep); 

        if(takenY == y && takenX == x)
        {
            return true;
        }
    }
    return false;
}
