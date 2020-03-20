#include "gamelevel.h"
#include <boxposition.h>

vector<BoxPosition*> fits;
vector<BoxPosition*> boxs;
BoxPosition *player1;

void GameLevel::map0(){
    mapSizeX = mapSizeY = 9;
    map = {
        {0,1,1,1,1,1,0,0,0},
        {0,1,0,0,0,1,1,1,1},
        {0,1,0,0,0,1,0,0,1},
        {0,1,1,0,0,0,0,0,1},
        {1,1,1,0,1,1,1,0,1},
        {1,0,0,0,1,0,1,0,1},
        {1,0,0,0,1,0,1,1,1},
        {1,0,0,0,1,0,0,0,0},
        {1,1,1,1,1,0,0,0,0}};

    floor = {
        {0,1,1,1,1,1,0,0,0},
        {0,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,0,1},
        {1,1,1,1,1,1,1,0,1},
        {1,1,1,1,1,0,1,0,1},
        {1,1,1,1,1,0,1,1,1},
        {1,1,1,1,1,0,0,0,0},
        {1,1,1,1,1,0,0,0,0}};

    fits.clear();
    fits.push_back(new BoxPosition(7,3));
    fits.push_back(new BoxPosition(7,4));
    fits.push_back(new BoxPosition(7,5));

    boxs.clear();
    boxs.push_back(new BoxPosition(2,5));
    boxs.push_back(new BoxPosition(2,6));
    boxs.push_back(new BoxPosition(3,6));

    player1 = new BoxPosition(2,7);
}

GameLevel::GameLevel(int level){
    if (level == 0){
        map0();
    }
}

int GameLevel::getMapSizeX(){
    return mapSizeX;
}

int GameLevel::getMapSizeY(){
    return mapSizeY;
}

vector<vector<int>> GameLevel::getMap(){
    return map;
}

vector<vector<int>> GameLevel::getFloor(){
    return floor;
}

vector<BoxPosition*> GameLevel::getFits(){
    return fits;
}

vector<BoxPosition*> GameLevel::getBoxes(){
    return boxs;
}

BoxPosition* GameLevel::getPlayer(){
    return player1;
}
