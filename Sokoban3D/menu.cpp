#include "menu.h"
#include "game0.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include <string>
#include <QStyle>
#include <QDesktopWidget>
#include <QApplication>

int menuItem = 0;
int display = 0;
int itemAmount = 2;
GLfloat x1 = 0.0f;
GLint boxId;
int soundVolume = 30;
int musicVolume = 60;

Menu::Menu(){
    setWindowTitle("Sokoban3D");
    time = QTime::currentTime();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    xrot = 0;

    selectSound = new QMediaPlayer();
    selectSound->setMedia(QUrl("sounds/select.wav"));
    selectSound->setVolume(soundVolume);

    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("sounds/backgroundSokoban.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    backgroundMusic = new QMediaPlayer();
    backgroundMusic->setVolume(musicVolume);
    backgroundMusic->setPlaylist(playlist);
    backgroundMusic->play();
}

Menu::~Menu()
{
    glDeleteTextures(1, texture);
}

void Menu::initializeGL(){
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

void Menu::resizeGL(int width, int height){
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

void Menu::showMenu(){
    QString name = "Sokoban3D";
    QString start = "Start Game";
    QString settings = "Settings";

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
    if (menuItem == 0){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(270, 470, start, font);

    if (menuItem == 1){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(310, 520, settings, font);

    if (menuItem == 2){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(350, 570, exit, font);

    glColor3f(1.0, 1.0, 1.0);
    font.setPixelSize(15);
    this->renderText(10, 590, credits, font);
    this->renderText(660, 590, open, font);
}

void Menu::showSettings(){
    QString soundVolumeStr;
    QString musicVolumeStr;
    soundVolumeStr.setNum(soundVolume);
    musicVolumeStr.setNum(musicVolume);
    QString soundVolumeText = "Sound Volume: " + soundVolumeStr;
    QString musicVolumeText = "Music Volume: " + musicVolumeStr;

    QString back = "Back";
    QString sets = "Settings";

    QFont font;
    font.setPixelSize(50);
    font.setBold(false);

    if (menuItem == 0){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(185, 150, soundVolumeText, font);

    if (menuItem == 1){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(185, 225, musicVolumeText, font);

    if (menuItem == 2){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(315, 295, back, font);

    glColor3f(1.0, 1.0, 1.0);
    font.setBold(true);
    this->renderText(290, 65, sets, font);

}

void Menu::stageCompleted() {
    QString stageCompleted = "Stage Completed";
    QString home = "Go Back to Home Screen";
    QFont font;

    backgroundMusic->stop();

    display = 3;

    font.setPixelSize(50);
    font.setBold(true);

    glColor3f(1.0, 1.0, 1.0);
    this->renderText(190, 120, stageCompleted, font);

    font.setPixelSize(50);
    font.setBold(false);

    if (menuItem == 0){
        glColor3f(1.0, 0,0);
    } else{
        glColor3f(1.0, 1.0, 1.0);
    }
    this->renderText(125, 295, home, font);
}

void Menu::paintGL(){

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
    } else if (display == 1){
        showSettings();
    } else if (display == 3){
        stageCompleted();
    }


    // Framerate control
    int delay = time.msecsTo(QTime::currentTime());
    if (delay == 0)
        delay = 1;
    time = QTime::currentTime();
    timer->start(qMax(0, 20 - delay));

}

void Menu::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Up:
        if (menuItem > 0 && display < 2){
            menuItem--;
            playSound(selectSound);
        }
        break;
    case Qt::Key_Down:
        if (menuItem < itemAmount && display < 2){
            menuItem++;
            playSound(selectSound);
        }
        break;
    case Qt::Key_Return:
        if (menuItem == itemAmount && display == 0){
            close();
        }
        else if (menuItem == 1 && display == 0){
            display = 1;
            menuItem = 0;
        }else if (menuItem == 2 && display == 1){
            menuItem = 0;
            display = 0;
        } else if (menuItem == 0 && display == 0){
            Game0 *game0 = new Game0(soundVolume,musicVolume,0);
            game0->setMinimumSize(800,600);
            game0->setMaximumSize(800,600);
            game0->setGeometry(
                        QStyle::alignedRect(
                            Qt::LeftToRight,
                            Qt::AlignCenter,
                            game0->size(),
                            qApp->desktop()->availableGeometry()
                            )
                        );

            this->close();
            game0->show();

        } else if (menuItem == 0 && display == 3){
            menuItem = 0;
            display = 0;
        }
        break;
    case Qt::Key_Left:
        if (display == 1 && menuItem == 0){
            if (soundVolume > 0) soundVolume--;
            selectSound->setVolume(soundVolume);
        }
        else if (display == 1 && menuItem == 1){
            if (musicVolume > 0) musicVolume--;
            backgroundMusic->setVolume(musicVolume);
        }
        break;
    case Qt::Key_Right:
        if (display == 1 && menuItem == 0){
            if (soundVolume < 100) soundVolume++;
            selectSound->setVolume(soundVolume);
        }
        else if (display == 1 && menuItem == 1){
            if (musicVolume < 100) musicVolume++;
            backgroundMusic->setVolume(musicVolume);
        }
        break;
    case Qt::Key_A:
        x1 -= 1;
        break;
    case Qt::Key_D:
        x1 += 1;
        break;
    }
}
