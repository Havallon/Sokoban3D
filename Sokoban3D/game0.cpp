#include "gameDesing.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <iostream>
#include<string>
#include "boxposition.h"

GLfloat x2 = 0.0f;
GLfloat y2 = 0.0f;
GLfloat z2 = 0.0f;



int mapSize = 9;

int wallMap[9][9];
int floorMap[9][9];
BoxPosition *fitBox[3];
BoxPosition *boxes[3];
BoxPosition *player;

void initMap(){
    int auxWall[9][9] = {
        {0,1,1,1,1,1,0,0,0},
        {0,1,0,0,0,1,1,1,1},
        {0,1,0,0,0,1,0,0,1},
        {0,1,1,0,0,0,0,0,1},
        {1,1,1,0,1,1,1,0,1},
        {1,0,0,0,1,0,1,0,1},
        {1,0,0,0,1,0,1,1,1},
        {1,0,0,0,1,0,0,0,0},
        {1,1,1,1,1,0,0,0,0}};

    int auxFloor[9][9] = {
        {0,1,1,1,1,1,0,0,0},
        {0,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,0,1},
        {1,1,1,1,1,1,1,0,1},
        {1,1,1,1,1,0,1,0,1},
        {1,1,1,1,1,0,1,1,1},
        {1,1,1,1,1,0,0,0,0},
        {1,1,1,1,1,0,0,0,0}};

    for (int i = 0; i < mapSize; i++){
        for (int j = 0; j < mapSize; j++){
            wallMap[i][j] = auxWall[i][j];
        }
    }

    for (int i = 0; i < mapSize; i++){
        for (int j = 0; j < mapSize; j++){
            floorMap[i][j] = auxFloor[i][j];
        }
    }

    fitBox[0] = new BoxPosition(7,3);
    fitBox[1] = new BoxPosition(7,4);
    fitBox[2] = new BoxPosition(7,5);

    boxes[0] = new BoxPosition(2,5);
    boxes[1] = new BoxPosition(2,6);
    boxes[2] = new BoxPosition(3,6);

    player = new BoxPosition(2,7);
}

GameDesing::GameDesing(int soundVolume, int musicVolume){
    setWindowTitle("Sokoban3D");
    time = QTime::currentTime();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

    selectSound = new QMediaPlayer();
    selectSound->setMedia(QUrl("sounds/select.wav"));
    selectSound->setVolume(soundVolume);

    backgroundMusic = new QMediaPlayer();
    backgroundMusic->setVolume(musicVolume);

    cameraAngleX = 50;
    cameraAngleY = 0;
    cameraAngleZ = 0;
}

GameDesing::~GameDesing(){
    glDeleteTextures(4, texture);
}

void GameDesing::initializeGL(){
    glShadeModel(GL_SMOOTH);
    qglClearColor(Qt::black);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_TEXTURE_2D);
    QImage box = convertToGLFormat(QImage("textures/box.bmp"));
    QImage wall = convertToGLFormat(QImage("textures/wall.bmp"));
    QImage floor = convertToGLFormat(QImage("textures/floor.bmp"));
    QImage player = convertToGLFormat(QImage("textures/player.bmp"));
    glGenTextures(4, texture);

    // Texture using nearest filter
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, box.width(), box.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, box.bits());

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, wall.width(), wall.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, wall.bits());

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, floor.width(), floor.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, floor.bits());

    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, player.width(), player.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, player.bits());

    // Set up lighting
    GLfloat ambLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPos[] = {0.0f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    _boxId = createBox(0);
    _wallId = createBox(1);
    _floorId = createBox(2);
    _player_Id = createBox(3);

    initMap();
}

void GameDesing::resizeGL(int width, int height){
    // Prevent divide by zero (in the gluPerspective call)
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height); // Reset current viewport

    glMatrixMode(GL_PROJECTION); // Select projection matrix
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(45.0f, static_cast<GLfloat>(width)/height, 0.1f, 100.0f); // Calculate aspect ratio

    glMatrixMode(GL_MODELVIEW); // Select modelview matrix
    glLoadIdentity(); // Reset modelview matrix
}

void GameDesing::drawMap(){
    for (int i = 0; i < mapSize; i++){
        for (int j = 0; j < mapSize; j++){
            if (wallMap[i][j] == 1){
                glPushMatrix();
                glTranslatef(j, 1.0, i);
                glCallList(_wallId);
                glPopMatrix();
            }
            if (floorMap[i][j] == 1){
                glPushMatrix();
                glTranslatef(j, 0, i);
                glCallList(_wallId);
                glPopMatrix();
            }
        }
    }
    for (int i = 0; i < 3; i++){
        glPushMatrix();
        glTranslatef(fitBox[i]->x,0,fitBox[i]->y);
        glCallList(_floorId);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(boxes[i]->x,1,boxes[i]->y);
        glCallList(_boxId);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(player->x,1,player->y);
    glCallList(_player_Id);
    glPopMatrix();
}

void GameDesing::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
    glLoadIdentity(); // Reset current modelview matrix

    glTranslatef(-3, -1, -25); // Move into the screen
    glTranslatef(+5,+1,+5);
    glRotatef(cameraAngleX  , 1, 0, 0);
    glRotatef(y2, 0, 1, 0);
    glRotatef(cameraAngleZ, 0, 0, 1);
    glTranslatef(-5,-1,-5);
    drawMap();

    showFPS();
    // Framerate control
    int delay = time.msecsTo(QTime::currentTime());
    if (delay == 0)
        delay = 1;
    time = QTime::currentTime();
    timer->start(qMax(0, 20 - delay));

}

void GameDesing::movePlayer(int dir){
    //DIR
    // 0 - frente
    // 1 - direta
    // 2 - tras
    // 3- esquerda

    if (dir == 0){
        if (player->y > 0)
            if (wallMap[player->y-1][player->x] == 0)
                player->y -= 1;
    }
    else if (dir == 1){
        if (player->x < 8)
            if (wallMap[player->y][player->x+1] == 0)
                player->x += 1;
    }
    else if (dir == 2){
        if (player->y < 8)
            if (wallMap[player->y+1][player->x] == 0)
                player->y += 1;
    }
    else if (dir == 3){
        if (player->x > 0)
            if (wallMap[player->y][player->x-1] == 0)
                player->x -= 1;
    }
}

void GameDesing::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Q:
        y2 -= 1;
        break;
    case Qt::Key_E:
        y2 += 1;
        break;
    case Qt::Key_Left:
        x2 -= 1;
        break;
    case Qt::Key_Right:
        x2 += 1;
        break;
    case Qt::Key_PageUp:
        z2 += 1;
        break;
    case Qt::Key_PageDown:
        z2 -= 1;
        break;
    case Qt::Key_W:
        movePlayer(0);
        break;
    case Qt::Key_S:
        movePlayer(2);
        break;
    case Qt::Key_A:
        movePlayer(3);
        break;
    case Qt::Key_D:
        movePlayer(1);
    break;
    }
    std::cout<< "x: " << x2 << std::endl;
    std::cout<< "y: " << y2 << std::endl;
    std::cout<< "z: " << z2 << std::endl;
}
