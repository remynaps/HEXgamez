#include <chrono>
#include <iostream>
#include <stack>
#include <regex> 
#include <math.h>
#include <sstream>
#include <string>
#include "include/player.h" 

using namespace std;

Player::Player(string color, bool isCPU, int number)
{
    this -> color = color;
    this -> isCPU = isCPU;
    this -> number = number;
}

//simple method to convert string to int
int stringToInt(string input)
{
    string myStream = input;
    istringstream buffer(myStream);
    int value;
    buffer >> value;

    return value;
}

/*
*generate a move by splitting up the input and performing numerous checks.
*the char is converted to int by ascii convertion
*the number is converted to int by using a buffer.
*/
pair<int, int>  Player::createMove(string input)
{
    pair<int, int> pair;
     //generate letter
    char inputChar = input[0];
    char basechar('a');
    if(inputChar < basechar) //97
    {
        basechar = 'A';
    }
    int letter(inputChar - basechar);

    // generate number
    std::string inputNumb = input.substr (1);
    int number = stringToInt(inputNumb);

    //combine to pair
    pair = make_pair(letter, number);

    return pair;
}

//return the input found in the terminal
pair<int, int> Player::getInput(string input)
{
    regex inputCheck("([A-Za-z])([0-9])([0-9])|([A-Za-z])([0-9])");

    if(regex_match(input, inputCheck))
    {
        return createMove(input);
    }
    //error hack
    return make_pair(-1,-1);
}

string Player::getColor()
{
    return color;
}

pair<int, int> Player::setRandomMove(std::vector< std::vector< int > > &map)
{
    int sizex = rand() % map.size();
    int sizey = rand() % map.size();

    return make_pair(sizex,sizey);
}


void Player::saveMove(pair<int, int> move)
{
    moves.push(move);
}

pair<int,int> Player::detMove(std::vector< std::vector< int > > &map)
{
    if(isCPU == true)
    {
        pair<int, vector< vector< int > >> bestMove = minMax(map,0,3);
        vector< vector< int > > calcMap = std::get<1>(bestMove);
        pair<int,int> move = extractMove(calcMap, map);
        return move;
    }
    return getInput(returnInput());
}

pair<int,int> Player::detMove(std::vector< std::vector< int > > &map, string input)
{
    if(isCPU == true)
    {
        pair<int, vector< vector< int > >> bestMove = minMax(map,0,3);
        vector< vector< int > > calcMap = std::get<1>(bestMove);
        pair<int,int> move = extractMove(calcMap, map);
        return move;
    }
    return getInput(input);
}

pair<int,int> Player::extractMove(vector< vector< int > > &map, vector< vector< int > > calcMap)
{
    for(int i = 0; i < map.size(); i++)
    {
        for(int j = 0; j < map.size(); j++)
        {
            if(map[i][j] != calcMap[i][j])
            {
                return make_pair(i,j);
            }
        }
    }
    return make_pair(-1,-1);
}

string Player::returnInput()
{
    string input;
    cout << color +" input move: ";
    cin >> input;

    return input;
}

int Player::getOtherPlayer()
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

vector<vector< vector< int > > >*  Player::getPosMoves(int playerNumber, vector< vector< int > > map)
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

pair<int, vector< vector< int > >> Player::minMax(vector< vector< int > > map, int depth, int maxDepth) {

    vector< vector< int > > bestMap;

    if (depth >= maxDepth)
    {
        return make_pair(evaluate(map), map);
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


//other player
pair<int, vector< vector< int > >> Player::minMin(vector< vector< int > > map, int depth, int maxDepth) {

    vector< vector< int > > bestMap;

    if (depth >= maxDepth)
    {
        return make_pair(evaluate(map), map);
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

int Player::evaluate(vector< vector< int > > map)
{
    int score = 0;
    for(int i = 0; i < map.size(); i ++)
    {
        for(int j = 0; j < map.size(); j++)
        {
            if(map[i][j] == number)
            {
                for(pair<int,int> connection: getConnections(i,j))
                {
                    int x = std::get<0>(connection);
                    int y = std::get<0>(connection);
                    if(x < map.size() && x >= 0 && y < map.size() && y >= 0)
                    {
                        if(map[x][y] == number)
                        {
                            score += 100;
                        }
                        else if(map[x][y] == getOtherPlayer())
                        {
                            score -= 100;
                        }
                    }
                }
            }
        }
    }
    return score;
}

vector<pair<int, int>> Player::getConnections(int x, int y)
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










