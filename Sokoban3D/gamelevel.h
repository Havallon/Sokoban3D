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
    float cameraZ;
    float translateX;
    float translateZ;

    vector<vector<int>> map;
    vector<vector<int>> floor;


public:

    GameLevel(int level);


    float getCameraX();
    float getCameraY();
    float getCameraZ();
    float getTranslateX();
    float getTranslateZ();


    vector<vector<int>> getMap();
    vector<vector<int>> getFloor();
    vector<BoxPosition*> getFits();
    vector<BoxPosition*> getBoxes();
    BoxPosition* getPlayer();

    void map0();
    void map1();
    void map2();
    void map3();
    void map4();
};

#endif // GAMELEVEL_H

