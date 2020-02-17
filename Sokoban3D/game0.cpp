#include "game0.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <iostream>
#include<string>

GLfloat x2 = 0.0f;
GLint _boxId;

Game0::Game0(int soundVolume, int musicVolume){
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

    cameraAngleY = 0;
}

Game0::~Game0(){
    glDeleteTextures(1, texture);
}

void Game0::initializeGL(){
    glShadeModel(GL_SMOOTH);
    qglClearColor(Qt::black);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_TEXTURE_2D);
    QImage img = convertToGLFormat(QImage("textures/box.bmp"));
    glGenTextures(1, texture);

    // Texture using nearest filter
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

    // Set up lighting
    GLfloat ambLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPos[] = {0.0f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    _boxId = createBox();
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

void Game0::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer

    glLoadIdentity(); // Reset current modelview matrix

    glTranslatef(0.0, -1, -10); // Move into the screen
    glRotatef(cameraAngleY, 0, 1, 0);
    glRotatef(x2, 1, 0, 0);

    glPushMatrix();
    glCallList(_boxId);
    glPopMatrix();


    showFPS();
    // Framerate control
    int delay = time.msecsTo(QTime::currentTime());
    if (delay == 0)
        delay = 1;
    time = QTime::currentTime();
    timer->start(qMax(0, 20 - delay));

}

void Game0::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Q:
        cameraAngleY -= 5;
        break;
    case Qt::Key_E:
        cameraAngleY += 5;
        break;
    case Qt::Key_Left:
        x2 -= 1;
        break;
    case Qt::Key_Right:
        x2 += 1;
        break;
    }
    std::cout<< x2 << std::endl;
}
