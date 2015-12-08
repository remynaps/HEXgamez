#include <chrono>
#include <iostream>
#include <stack>
#include <regex> 
#include <math.h>
#include <sstream>
#include <string>
#include <thread>
#include <future>
#include "include/player.h" 
#include "include/ai.h"

using namespace std;

Player::Player(string color, bool isCPU, int number)
{
    this -> color = color;
    this -> isCPU = isCPU;
    this -> number = number;
    if(isCPU)
    {   
        ai = new Ai(number);
    }
}

Player::~Player()
{
    // delete ai;
}

//simple method to convert string to int
int stringToInt(string input)
{
    string myStream = input;
    istringstream buffer(myStream);
    int value;
    buffer >> value;

    return value - 1;
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
    int sizex = rand() % (map.size());
    int sizey = rand() % (map.size());

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
        pair<int, vector< vector< int > >> bestMove = ai->minMax(map,0,ai->minax);
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
        pair<int, vector< vector< int > >> bestMove = ai->minMax(map,0,ai->minax);
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
                return make_pair(j,i);
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
























