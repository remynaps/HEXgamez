#include <chrono>
#include <iostream>
#include <math.h>
#include <vector>
#include <thread>
#include "include/map.h"
#include "include/player.h"
#include "include/view.h"

using namespace std;

Map::Map(int width, int height, int gameMode, int maxStep, int startPlayer)
{
    view = new View();
    hasWinner = false;

    this -> createMap(height, width);
    this -> createPlayers(gameMode);

    this -> width = width;
    this -> height = height;
    this -> gameMode = gameMode;
    this -> maxStep = maxStep;
    this -> startPlayer = startPlayer;

    setStartPlayer(startPlayer);
    view->print(map);
}

Map::~Map()
{
    delete view;
    delete player1;
    delete player2; 
}


//create a 2d vector of integers
void Map::createMap(int height, int width)
{
    for(int i = 0; i < height; i++)
    {
        std::vector<int> temp_vec;
        for(int j = 0; j < width; j++)
        {
            temp_vec.push_back(0);
        }
        map.push_back(temp_vec);
    }
}

void Map::setStartPlayer(int playerNumber)
{
    switch(playerNumber)
    {
        case 1:
            currentPlayer = player1;
            break;
        case 2:
            currentPlayer = player2;
            break;
    }
}

//show a menu to create the players and generate an ai if nessecary.
void Map::createPlayers(int gameMode)
{
    switch(gameMode)
    {
        case 1:
        player1 = new Player("blue",false, 1);
        player2 = new Player("red",false, 2);
        break;
        case 2:
            bool valid = false;
            while(!valid)
            {
                string input;
                cout << "select player" << endl;
                cout << "1 : blue" << endl;
                cout << "2 : red" << endl;
                cin >> input;
                if(input == "2")
                {
                    player1 = new Player("blue",true, 1);
                    player2 = new Player("red",false, 2);
                    valid = true;
                }
                else if(input == "1")
                {
                    player1 = new Player("blue",false, 1);
                    player2 = new Player("red",true, 2);
                    valid = true;
                }
                else{
                    cout << "no player" << endl;
                }
            }
            break;
    }
}

//set the move depending on the type of player
void Map::setMove()
{
    pair<int, int> coords;
    bool moveValid = false;

    while(!moveValid)
    {

        if(!currentPlayer->isCPU)
        {
            string input = currentPlayer->returnInput();
            coords = currentPlayer->detMove(map,input);
        }
        else
        {
            cout <<endl << "Calculating best move...." << endl;
            coords = currentPlayer->detMove(map);
        }

        int x = std::get<0>(coords);
        int y = std::get<1>(coords);
        moveValid = giveError(x,y, coords);
    }
}

//check for possible errors in the coordinate and show an error message
bool Map::giveError(int x, int y, pair<int, int> &coords)
{
    if(x >= map.size() || y >= map.size() || y < 0 || x < 0)
    {
        cout << "point does not exist" << endl;
        return false;
    }
    else if(map[y][x] == 1 || map[y][x] == 2)
    {
        cout << "point is already taken" << endl;
        return false;
    }
    else
    {
        currentPlayer->saveMove(coords);
        inputMove(x,y);
        moves.push(coords);
        return true;
    }
}

//long method
//this method only shows nessecary menu items depending on the current situations.
// example: it will only show pierule after the first move is set.
void Map::moveMenu()
{
    if(!moves.empty())
    {
        pair<int,int> lastMove = moves.top();
        int x = std::get<0>(lastMove);
        int y = std::get<1>(lastMove);

        char letter = (char(x + 65));
        cout << "Last Move: " << letter << y + 1 << endl << endl;
    }

    cout << currentPlayer->color << endl;
    bool moveValid = false;
    string input;

    if(moves.size() == 1)
    {
        cout << "1 : pierule" << endl;
    }
    if(!moves.empty())
    {
        cout << "2 : delete move" << endl;
    }

    cout << "3 : set move" << endl;

    while(!moveValid)
    {
        cout << "input option: ";
        cin >> input;
        if(input == "1" && moves.size() == 1)
        {
            pieRule();
            view->print(map);
            moveValid = true;
        }
        else if(input == "2" && !moves.empty())
        {
            deleteMove();
            view->print(map);
            moveValid = true;
        }
        else if(input == "3")
        {
            setMove();
            moveValid = true;
        }
        else
        {
            cout << "dusnt werk" << endl;
        }
    }
}

void Map::inputMove(int x, int y)
{
    map[y][x] = currentPlayer->number;
}

void Map::switchPlayer()
{
    if(currentPlayer == player1)
    {
        currentPlayer = player2;
    }
    else
    {
        currentPlayer = player1;
    }
}

//switch the players.
void Map::pieRule()
{
    for(int i = 0; i < moves.size(); i++)
    {
        pair<int,int> firstMove = moves.top();
        int x = std::get<0>(firstMove);
        int y = std::get<1>(firstMove);
        switch(map[y][x])
        {
            case 1:
                map[y][x] = 2;
                break;
            case 2:
                map[y][x] = 1;
                break;
        }
        cout << map[y][x] << endl;
    }
}

void Map::deleteMove()
{
    if(!moves.empty())
    {
        pair<int,int> move = moves.top();

        int takenX = std::get<0>(move);
        int takenY = std::get<1>(move);

        map[takenY][takenX] = 0;
        moves.pop();
    }
}

//return all the connections for a given point on the map.
vector<pair<int, int>> Map::getConnections(int x, int y)
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

//start building a path if a move is detected on the player generated end position.
void Map::checkWinner()
{
    for(int i = 0; i < map.size(); i++)
    {
        if(map[i][map[i].size() -1 ] == 1)
        {
            startPath(i, 1);
        }
        if(map[map[i].size() -1 ][i] == 2)
        {
            cout << "---------" << endl;
            startPath(i, 2);
        }
    }
}

//return the position required to finish the map.
//the answer depends on the supplied playerNumber;
pair<int,int> Map::getEndPosition(int playerNumber, int position)
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
void Map::startPath(int position, int playerNumber)
{
    vector<pair<int, int>> path;
    vector<thread> workers;

    pair<int, int> endPosition = getEndPosition(playerNumber, position);
    int endX = std::get<0>(endPosition);
    int endY = std::get<1>(endPosition);

    path.push_back(endPosition);

    buildPath(endY,endX, playerNumber, path, workers);

    for(auto &th: workers)
    {
        th.join();
    }

}

//actually build the path recursivly
//@todo multithreading when more than one connection is located.
void Map::buildPath(int currentX, int currentY, int playerNumber, vector<pair<int, int>> &path, vector<thread> &workers)
{
    vector<pair<int, int>> connections = getConnections(currentX,currentY);
    int numberConnections = 0;
    for(int i = 0; i < connections.size(); i++)
    {
        int x = std::get<0>(connections[i]);
        int y = std::get<1>(connections[i]);

        if(x < map.size() && x >= 0 && y < map.size() && y >= 0)
        {
            determineWinner(x,y,playerNumber,path, numberConnections, workers);
        }
    }
}

//determine the winner of the game and quit the game.
void Map::determineWinner(int x, int y, int playerNumber, vector<pair<int, int>> &path, int &numberConnections, vector<thread> &workers)
{
    if(map[x][y] == playerNumber && !stepTaken(x,y,path))
    {
        cout << x << " : " << y << endl;
        path.push_back(make_pair(x,y));

        if(playerNumber == 1 && y == 0)
        {
            hasWinner = true;
        }
        else if(playerNumber == 2 && x == 0)
        {
            hasWinner = true;
        }
        else
        {
            numberConnections++;
            if(numberConnections > 1)
            {
                pathThread(x, y, playerNumber, path, workers);
            }
            else
            {
                buildPath(x, y, playerNumber, path, workers);
            }
        }
    }
}

void Map::pathThread(int x, int y, int playerNumber, vector<pair<int, int>> &path, vector<thread> &workers)
{
    workers.push_back(std::thread([x, y, playerNumber, &path, &workers, this]()
    {
        this->buildPath(x, y, playerNumber, path ,workers);
    }));
}

bool Map::stepTaken(int x, int y, vector<pair<int,int>> &path)
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


void Map::update()
{
    moveMenu();
    checkWinner();
    view->print(map);
    switchPlayer();
}









