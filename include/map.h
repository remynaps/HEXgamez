#ifndef __MAP_H_INCLUDED__
#define __MAP_H_INCLUDED__


#include <chrono>
#include <iostream>
#include <math.h>
#include <vector> 
#include <thread>
#include "view.h"
#include "player.h"

using namespace std;

class Map {
private:
    View *view;

    Player *player1;
    Player *player2;

    Player *currentPlayer;

    std::vector< std::vector< int > > map;
    stack<pair<int, int> > moves;
    int height;
    int width;
    int gameMode;
    int startPlayer;
    int maxStep;

public:
    Map(int width, int height, int gameMode, int maxStep, int startPlayer);
    ~Map();

    bool hasWinner;

    void createMap(int height, int width);
    void createPlayers(int gameMode);
    void setStartPlayer(int playerNumber);
    int getWidth();
    int getHeight();
    void switchPlayer();
    vector<vector<int> > getMap();
    void setMove();
    std::vector<pair<int, int>> getConnections(int x, int y);
    void inputMove(int x, int y);
    bool giveError(int x, int y, pair<int, int> &coords);
    void setColor(int x, int y);
    void checkWinner();
    void determineWinner(int x, int y, int playerNumber, vector<pair<int, int>> &path, int &numberConnections,  vector<thread> &workers);
    void pathThread(int x, int y, int playerNumber, vector<pair<int, int>> &path,  vector<thread> &workers);
    pair<int,int>getEndPosition(int playerNumber, int position);
    void startPath(int position, int playerNumber);
    void buildPath(int x,int y, int playerNumber, vector<pair<int, int>> &path,  vector<thread> &workers);
    bool stepTaken(int x, int y, vector<pair<int,int>> &path);
    void pieRule();
    string returnInput();
    void update();
    void print();
    void callMove();
    void deleteMove();
    void getInput();
    pair<int,int> detAction(string input);
};

#endif // __MAP_H_INCLUDED__ 