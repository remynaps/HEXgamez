#include <chrono>
#include <iostream>
#include <math.h> 
#include <vector>
#include "include/view.h"

using namespace std;


//the beauty is real.
void View::print(vector<vector<int> > map)
{
    cout << "\x1b[2J\x1b[1;1H";

    cout << "  ";
    for(int s = 0; s < map.size(); s++)
    {
        string color = "\033[1;31m";
        color += (char(s + 65));
        color +=  "\033[0m";

        cout << color + "   ";
    }
    cout << endl;

    for(int i = 0; i < map.size(); i++)
    {

        for(int k = 0; k < (i*2) + 1; k++)
        {
            cout << " ";
        }

        cout << "\033[1;34m"+to_string(i + 1)+"\033[0m" + "  ";

        for(int j = 0; j <= map[i].size(); j++)
        {
            if(j == map[i].size())
            {
                std::cout << std::endl;
            }
            else if(j == 0)
            {
                if(map[i][j] == 2)
                {
                    cout << "\033[1;31m"+to_string(map[i][j])+"\033[0m";
                }
                else if(map[i][j] == 1)
                {
                    cout << "\033[1;34m"+to_string(map[i][j])+"\033[0m";
                }
                else
                {
                    cout << map[i][j];
                }
            }
            else
            {
                if(map[i][j] == 1)
                {
                    cout << " - " << "\033[1;34m"+to_string(map[i][j])+"\033[0m";
                }
                else if(map[i][j] == 2)
                {
                    cout << " - " << "\033[1;31m"+to_string(map[i][j])+"\033[0m";
                }
                else
                {
                   std::cout << " - " << map[i][j];
                }
            }
        }
        //meh
    }
    cout << endl;
}
//it jest werks





