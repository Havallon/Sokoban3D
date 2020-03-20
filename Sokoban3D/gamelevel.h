#ifndef GAMELEVEL_H
#define GAMELEVEL_H


#include <vector>
#include "boxposition.h"
using namespace std;

class GameLevel
{
private:
    int mapSizeX;
    int mapSizeY;

    vector<vector<int>> map;
    vector<vector<int>> floor;


public:

    GameLevel(int level);

    int getMapSizeX();
    int getMapSizeY();

    vector<vector<int>> getMap();
    vector<vector<int>> getFloor();
    vector<BoxPosition*> getFits();
    vector<BoxPosition*> getBoxes();
    BoxPosition* getPlayer();

    void map0();
};

#endif // GAMELEVEL_H

