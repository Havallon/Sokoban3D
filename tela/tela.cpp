#include "tela.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include<string>

int menu = 0;
int display = 0;
GLfloat x1 = 0.0f;
GLint boxId;

Tela::Tela(){
    setWindowTitle("Main Menu");
    time = QTime::currentTime();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    xrot = 0;
}

Tela::~Tela()
{
    glDeleteTextures(1, texture);
}

void Tela::initializeGL(){
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
    boxId = createBox();
}

void Tela::resizeGL(int width, int height){
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

void Tela::showMenu(){
    QString name = "Sokoban3D";
    QString start = "New Game";
    QString credits = "Developed by: Havallon and Nadine";
    QString exit = "Exit";
    QString open = "Powered by OpenGL";
    QFont font;
    font.setPixelSize(75);
    font.setBold(true);

    glColor3f(1.0, 1.0, 1.0);
    this->renderText(190, 120, name, font);

    font.setPixelSize(50);
    font.setBold(false);
    if (menu == 0){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(270, 470, start, font);
    if (menu == 1){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(350, 540, exit, font);

    glColor3f(1.0, 1.0, 1.0);
    font.setPixelSize(20);
    this->renderText(10, 590, credits, font);
    this->renderText(610, 590, open, font);
}



void Tela::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer

    glLoadIdentity(); // Reset current modelview matrix

    glTranslatef(0.0f, 0.0f, -10); // Move into the screen

    showFPS();

    if (display == 0){
        glPushMatrix();
        glRotatef(xrot, 0.0f, 1.0f, 0.0f); // Rotate on Y-axis
        glCallList(boxId);
        glPopMatrix();

        showMenu();
        xrot += 1;
    }


    // Framerate control
    int delay = time.msecsTo(QTime::currentTime());
    if (delay == 0)
        delay = 1;
    time = QTime::currentTime();
    timer->start(qMax(0, 20 - delay));

}

void Tela::keyPressEvent(QKeyEvent *event){

    switch(event->key()){
    case Qt::Key_Up:
        if (menu > 0 && display == 0) menu--;
        break;
    case Qt::Key_Down:
        if (menu < 1 && display == 0) menu++;
        break;
    case Qt::Key_Return:
        if (menu == 1 && display == 0){
            close();
        }
        break;
    case Qt::Key_Left:
        x1 -= 1;
        break;
    case Qt::Key_Right:
        x1 += 1;
        break;
    }
    std::cout<< x1 << std::endl;
}
