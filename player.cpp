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
        pair<int, vector< vector< int > >> bestMove = minMax(map,0,2);
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
        pair<int, vector< vector< int > >> bestMove = minMax(map,0,2);
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
        return make_pair(monteCarlo(map), map);
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
        return make_pair(monteCarlo(map), map);
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

    //pair<int,int> move = extractMove(map);
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
                    int y = std::get<1>(connection);
                    if(x < map.size() && x >= 0 && y < map.size() && y >= 0)
                    {
                        if(map[x][y] == number)
                        {
                            score += 1000;
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

// int Player::scorePath(vector< vector< int > > &map, vector<pair<int, int>> path)
// {
//     int length = 0;
//     for(int i = 0; i < path.size(); i++)
//     {
//         int curX = std::get<0>(path[i]);
//         int curY = std::get<1>(path[i]);

//         switch(map[curX][curY])
//         {   
//             case 0:
//                 length -= 10;
//                 break;
//             case 1:
//                 length += 0;
//                 break;
//             case 2:
//                 length -= 1000;
//                 break;
//         }
//     }
//     return length;
// }

// vector<pair<int, int>> Player::getPath(pair<int,int> move, vector< vector< int > > &map)
// {
//     int X = std::get<0>(move);
//     int Y = std::get<1>(move);
//     int goal = map.size() -1;

//     vector<pair<int,int>> path;

//     for(pair<int,int> connection: getConnections(X,Y))
//     {
//         int curX = std::get<0>(connection);
//         int curY = std::get<1>(connection);
//         if((goal - curY < goal - Y && number == 1) || (goal - curX < goal - X && number == 2))
//         {
//             path.push_back(make_pair(curX,curY));
//         }
//     }
//     return path;
// }

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

int Player::monteCarlo(vector< vector< int > > &map)
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
    //cout << movesCount << "??????" << endl;
    //start montecarlo simulation and keep track of the number of times you win
    int timesWon = 0;
    int timesLost = 0;
    int playerNumber = number;
    for(int k = 0; k < 50; k++)
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

            int x = rand() % map.size();
            int y = rand() % map.size();
            if(mapCopy[x][y] == 0)
            {
                mapCopy[x][y] = playerNumber;
                count++;
            }
        }
        
        // mapCopy[2][3] = 89;
        // cout << endl;
        // for(int i = 0; i < map.size(); i++)
        // {
        //     for(int j = 0; j < map.size(); j++)
        //     {
        //         cout << map[i][j];
        //     }
        // }
        // cout << endl;
        // for(int i = 0; i < map.size(); i++)
        // {
        //     for(int j = 0; j < map.size(); j++)
        //     {
        //         cout << mapCopy[i][j];
        //     }
        // }
        // bool isWinner = checkWinner(mapCopy);
        if(checkWinner(mapCopy))
        {
            //cout << "koek" <<endl;
            timesWon++;
        }
        else
        {
            //cout << "meh" <<endl;
            timesLost++;
        }
        // else if(winner == 0)
        // {
        //     cout << "joji" << endl;
        // }
    }
    // cout<< timesWon << "-------------------------" << timesLost << endl;
    return timesWon - timesLost;
}




//-------------------------------------------------------------------------------------
// I HATE MYSELF FOR DOING THE FOLLOWING
//-------------------------------------------------------------------------------------

bool Player::checkWinner(vector< vector< int > > &map)
{
    for(int i = 0; i < map.size(); i++)
    {
        if(map[i][map[i].size() -1 ] == 1)
        {
            return startPath(i, 1, map);
        }
        if(map[map[i].size() -1 ][i] == 2)
        {
            return startPath(i, 2, map);
        }
        //cout << winner2 << ":::::" << winner1 << endl;
    }
    return false;
}

pair<int,int> Player::getEndPosition(int playerNumber, int position, vector< vector< int > > &map)
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

//start calculating a [ath when a point is located on the end position and on the start position
bool Player::startPath(int position, int playerNumber, vector< vector< int > > &map)
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
//@todo multithreading when more than one connection is located.
bool Player::buildPath(int currentX, int currentY, int playerNumber, vector<pair<int, int>> &path, vector<thread> &workers, vector< vector< int > > &map)
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
bool Player::determineWinner(int x, int y, int playerNumber, vector<pair<int, int>> &path, int &numberConnections, vector<thread> &workers ,vector< vector< int > > &map)
{
    if(map[x][y] == playerNumber && !stepTaken(x,y,path,map))
    {
        path.push_back(make_pair(x,y));

        if(playerNumber == 1 && y == 0)
        {
            return true;
        }
        else if(playerNumber == 2 && x == 0)
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

bool Player::stepTaken(int x, int y, vector<pair<int,int>> &path, vector< vector< int > > &map)
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






















