#ifndef GAMELEVEL_H
#define GAMELEVEL_H


#include <vector>
#include "boxposition.h"
using namespace std;

class GameLevel
{
private:

    float cameraX;
    float cameraY;

    vector<vector<int>> map;
    vector<vector<int>> floor;


public:

    GameLevel(int level);


    float getCameraX();
    float getCameraY();


    vector<vector<int>> getMap();
    vector<vector<int>> getFloor();
    vector<BoxPosition*> getFits();
    vector<BoxPosition*> getBoxes();
    BoxPosition* getPlayer();

    void map0();
    void map1();
    void map2();
};

#endif // GAMELEVEL_H

