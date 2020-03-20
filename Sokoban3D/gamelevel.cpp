#include "gamelevel.h"
#include <boxposition.h>

vector<BoxPosition*> fits;
vector<BoxPosition*> boxs;
BoxPosition *player1;

void GameLevel::map0(){
    map.clear();
    floor.clear();
    fits.clear();
    boxs.clear();

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


    cameraX = map[0].size()/2.0;
    cameraY = map.size()/2.0;

    fits.push_back(new BoxPosition(7,3));
    fits.push_back(new BoxPosition(7,4));
    fits.push_back(new BoxPosition(7,5));


    boxs.push_back(new BoxPosition(2,5));
    boxs.push_back(new BoxPosition(2,6));
    boxs.push_back(new BoxPosition(3,6));

    player1 = new BoxPosition(2,7);
}

void GameLevel::map1(){

    map.clear();
    floor.clear();
    fits.clear();
    boxs.clear();

    map = {
        {0,0,1,1,1,1,1,1,1,0},
        {0,0,1,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,0,1,0},
        {1,1,1,1,1,0,1,0,1,0},
        {1,0,0,0,0,0,0,0,1,0},
        {1,0,0,1,0,1,1,0,1,1},
        {1,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}};

    floor = {
        {0,0,1,1,1,1,1,1,1,0},
        {0,0,1,1,1,1,1,1,1,0},
        {0,0,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1}};

    cameraX = map[0].size()/2.0;
    cameraY = map.size()/2.0;

    fits.push_back(new BoxPosition(1,6));
    fits.push_back(new BoxPosition(1,7));
    fits.push_back(new BoxPosition(2,6));
    fits.push_back(new BoxPosition(2,7));

    boxs.push_back(new BoxPosition(4,2));
    boxs.push_back(new BoxPosition(2,4));
    boxs.push_back(new BoxPosition(4,5));
    boxs.push_back(new BoxPosition(3,6));

    player1 = new BoxPosition(6,2);

}


GameLevel::GameLevel(int level){
    if (level == 0){
        map0();
    } else if (level == 1){
        map1();
    }
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

float GameLevel::getCameraX(){
    return cameraX;
}

float GameLevel::getCameraY(){
    return cameraY;
}
