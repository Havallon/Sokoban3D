#include "Game0.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <iostream>
#include <string>
#include "boxposition.h"
#include <cmath>
#include <QStyle>
#include <QDesktopWidget>
#include <QApplication>
#include "menu.h"

#include "gamelevel.h"

#define PI_OVER_180 0.0174532925f

vector<vector<int>> wallMap;
vector<vector<int>> floorMap;
vector<BoxPosition*> fitBox;
vector<BoxPosition*> boxes;
BoxPosition *player;

//GLfloat x2 = -3.0f;
//GLfloat z2 = -25.0f;

int boxAmount;
int box = 0;

int lv;

float offsetX, offsetY, offsetZ, translateX, translateZ;

int maxStage;

int sound,music;

void initMap(int lv){
    GameLevel *g = new GameLevel(lv);

    wallMap = g->getMap();
    floorMap = g->getFloor();

    fitBox = g->getFits();
    boxes = g->getBoxes();

    player = g->getPlayer();

    boxAmount = boxes.size();

    offsetX = g->getCameraX();
    offsetY = g->getCameraY();
    offsetZ = g->getCameraZ();

    translateX = g->getTranslateX();
    translateZ = g->getTranslateZ();

    maxStage = 4;
}

Game0::Game0(int soundVolume, int musicVolume, int level){
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
    lv = level;
    sound = soundVolume;
    music = musicVolume;
}

Game0::~Game0(){
    glDeleteTextures(4, texture);
}

void Game0::initializeGL(){
    glShadeModel(GL_SMOOTH);
    qglClearColor(Qt::black);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    QImage box = convertToGLFormat(QImage("textures/box.bmp"));
    QImage wall = convertToGLFormat(QImage("textures/wall.bmp"));
    QImage fit = convertToGLFormat(QImage("textures/box2.png"));
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, fit.width(), fit.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, fit.bits());

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

    initMap(lv);
}

void Game0::resizeGL(int width, int height){
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

void Game0::drawMap(){
    for (unsigned int i = 0; i < wallMap.size(); i++){
        for (unsigned int j = 0; j < wallMap[0].size(); j++){
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
    for (int i = 0; i < boxAmount; i++){
        glPushMatrix();
        glTranslatef(fitBox[i]->x,1,fitBox[i]->y);
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

void Game0::showBoxes() {
    QFont font;
    font.setPixelSize(25);
    font.setBold(false);
    glColor3f(1.0, 1.0, 1.0);
    QString amount;
    QString current;
    amount.setNum(boxAmount);
    current.setNum(box);
    QString text = "Boxes: " + current + "/" + amount;
    this->renderText(340, 30, text, font);
}

void Game0::showInfo() {
    QFont font;
    font.setPixelSize(15);
    font.setBold(false);
    glColor3f(1.0, 1.0, 1.0);
    QString move = "PRESS W,A,S,D TO MOVE";
    QString rotate = "PRESS Q AND E TO ROTATE THE CAMERA";
    QString reset = "PRESS R TO RESET THE GAME";
    this->renderText(10, 590, move, font);
    this->renderText(255, 590, reset, font);
    this->renderText(505, 590, rotate, font);
}

void Game0::checkBoxes(){
    box = 0;
    for (int i=0; i < boxAmount; i++){
        int j = 0;
        while (j<boxAmount){
            if (fitBox[j]->x == boxes[i]->x && fitBox[j]->y == boxes[i]->y){
                box++;
                break;
            }
            j++;
        }
    }

    if(box == boxAmount){
        Menu *w = new Menu();
        w->setMinimumSize(800, 600);
        w->setMaximumSize(800, 600);
        w->setGeometry( //Iniciar a tela no centro
                        QStyle::alignedRect(
                            Qt::LeftToRight,
                            Qt::AlignCenter,
                            w->size(),
                            qApp->desktop()->availableGeometry()
                            )
                        );
        this->close();
        if (lv == maxStage){
            w->setVolume(sound,music);
            w->gameCompleted();
        }else{
            w->setVolume(sound,music);
            w->stageCompleted(++lv);
        }
        w->show();
    }
}

void Game0::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
    glLoadIdentity(); // Reset current modelview matrix

    glTranslatef(translateX, 0, translateZ); // Move into the screen
    glTranslatef(offsetX,offsetZ,offsetY);
    glRotatef(cameraAngleX  , 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    glRotatef(cameraAngleZ, 0, 0, 1);
    glTranslatef(-offsetX,-offsetZ,-offsetY);
    drawMap();

    showInfo();
    showFPS();
    showBoxes();
    checkBoxes();

    // Framerate control
    int delay = time.msecsTo(QTime::currentTime());
    if (delay == 0)
        delay = 1;
    time = QTime::currentTime();
    timer->start(qMax(0, 20 - delay));

}

void Game0::movePlayer(int dir){
    //DIR
    // 0 - frente
    // 1 - direita
    // 2 - tras
    // 3 - esquerda

    if (dir == 0){
        if (player->y > 0){
            if (wallMap[player->y-1][player->x] == 0){
                boolean canWalk = true;
                int idBox = -1;
                for (int i = 0; i < boxAmount; i++){
                    if (player->y-1 == boxes[i]->y && player->x == boxes[i]->x){
                        if (wallMap[player->y-2][player->x] == 1){
                            canWalk = false;
                        } else {
                            for (int j = 0; j < boxAmount; j++){
                                if (player->y-2 == boxes[j]->y && player->x == boxes[j]->x){
                                    canWalk = false;
                                }
                            }
                        }
                        if (canWalk) idBox = i;
                    }
                }
                if (canWalk){
                    if (idBox == -1)
                        player->y -= 1;
                    else{
                        player->y -= 1;
                        boxes[idBox]->y -= 1;
                    }
                }
            }
        }
    }
    else if (dir == 1){
        if (player->x < wallMap[0].size()-1){
            if (wallMap[player->y][player->x+1] == 0){
                boolean canWalk = true;
                int idBox = -1;
                for (int i = 0; i < boxAmount; i++){
                    if (player->y == boxes[i]->y && player->x+1 == boxes[i]->x){
                        if (wallMap[player->y][player->x+2] == 1){
                            canWalk = false;
                        } else {
                            for (int j = 0; j < boxAmount; j++){
                                if (player->y == boxes[j]->y && player->x+2 == boxes[j]->x){
                                    canWalk = false;
                                }
                            }
                        }
                        if (canWalk) idBox = i;
                    }
                }
                if (canWalk){
                    if (idBox == -1)
                        player->x += 1;
                    else{
                        player->x += 1;
                        boxes[idBox]->x += 1;
                    }
                }
            }
        }
    }
    else if (dir == 2){
        if (player->y < wallMap.size()-1){
            if (wallMap[player->y+1][player->x] == 0){
                boolean canWalk = true;
                int idBox = -1;
                for (int i = 0; i < boxAmount; i++){
                    if (player->y+1 == boxes[i]->y && player->x == boxes[i]->x){
                        if (wallMap[player->y+2][player->x] == 1){
                            canWalk = false;
                        } else {
                            for (int j = 0; j < boxAmount; j++){
                                if (player->y+2 == boxes[j]->y && player->x == boxes[j]->x){
                                    canWalk = false;
                                }
                            }
                        }
                        if (canWalk) idBox = i;
                    }
                }
                if (canWalk){
                    if (idBox == -1)
                        player->y += 1;
                    else{
                        player->y += 1;
                        boxes[idBox]->y += 1;
                    }
                }
            }
        }
    }
    else if (dir == 3){
        if (player->x > 0){
            if (wallMap[player->y][player->x-1] == 0){
                boolean canWalk = true;
                int idBox = -1;
                for (int i = 0; i < boxAmount; i++){
                    if (player->y == boxes[i]->y && player->x-1 == boxes[i]->x){
                        if (wallMap[player->y][player->x-2] == 1){
                            canWalk = false;
                        } else {
                            for (int j = 0; j < boxAmount; j++){
                                if (player->y == boxes[j]->y && player->x-2 == boxes[j]->x){
                                    canWalk = false;
                                }
                            }
                        }
                        if (canWalk) idBox = i;
                    }
                }
                if (canWalk){
                    if (idBox == -1)
                        player->x -= 1;
                    else{
                        player->x -= 1;
                        boxes[idBox]->x -= 1;
                    }
                }
            }
        }
    }
}

void Game0::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Q:
        cameraAngleY -= 1;
        if (cameraAngleY == -360) cameraAngleY = 0;
        break;
    case Qt::Key_E:
        cameraAngleY += 1;
        if (cameraAngleY == 360) cameraAngleY = 0;
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
    case Qt::Key_R:
        initMap(lv);
        break;

//    case Qt::Key_Left:
//        x2 -= 1;
//        break;
//    case Qt::Key_Right:
//        x2 += 1;
//        break;
//    case Qt::Key_Up:
//        z2 += 1;
//        break;
//    case Qt::Key_Down:
//        z2 -= 1;
//        break;

    }

    //std::cout<< "x: " << x2 << std::endl;
    //std::cout<< "y: " << y2 << std::endl;
    //std::cout<< "z: " << z2 << std::endl;
}
