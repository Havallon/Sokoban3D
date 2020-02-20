#ifndef GAME0_H
#define GAME0_H

#include <QGLWidget>
#include <QTime>
#include <QMediaPlayer>

class QTimer;

class Game0 : public QGLWidget
{
    Q_OBJECT

public:
    Game0(int soundVolume, int musicVolume);
    ~Game0();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);

private:
    QTimer *timer;
    QTime time;
    GLuint texture[4];

    int maxIdTexture = 3;

    GLint _boxId, _wallId, _floorId, _player_Id;

    float cameraAngleX = 0;
    float cameraAngleY = 0;
    float cameraAngleZ = 0;

    QMediaPlayer *selectSound;
    QMediaPlayer *backgroundMusic;

    float frame, frameAnt=0;
    qint64 currentTime, timebase=0;

    GLint createBox(int kind) {
        GLint id = glGenLists(1);
        glNewList(id,GL_COMPILE);

        GLfloat boxSize = 0.5f;

        // KIND:
        // 0 - BOX
        // 1 - WALL
        // 2 - FLOOR

        qglColor(Qt::white);
        if (kind > maxIdTexture) kind = 0;
        glBindTexture(GL_TEXTURE_2D, texture[kind]);
        glBegin(GL_QUADS);
        // Front Face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-boxSize, -boxSize, boxSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( boxSize, -boxSize, boxSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( boxSize, boxSize, boxSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-boxSize, boxSize, boxSize); // Top Left Of The Texture and Quad
        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-boxSize, -boxSize, -boxSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-boxSize, boxSize, -boxSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( boxSize, boxSize, -boxSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( boxSize, -boxSize, -boxSize); // Bottom Left Of The Texture and Quad
        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-boxSize, boxSize, -boxSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-boxSize, boxSize, boxSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( boxSize, boxSize, boxSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( boxSize, boxSize, -boxSize); // Top Right Of The Texture and Quad
        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-boxSize, -boxSize, -boxSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( boxSize, -boxSize, -boxSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( boxSize, -boxSize, boxSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-boxSize, -boxSize, boxSize); // Bottom Right Of The Texture and Quad
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( boxSize, -boxSize, -boxSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( boxSize, boxSize, -boxSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( boxSize, boxSize, boxSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( boxSize, -boxSize, boxSize); // Bottom Left Of The Texture and Quad
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-boxSize, -boxSize, -boxSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-boxSize, -boxSize, boxSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-boxSize, boxSize, boxSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-boxSize, boxSize, -boxSize); // Top Left Of The Texture and Quad
        glEnd();
        glEndList();

        return id;
    }

    void showFPS() {
        frame++;
        currentTime=QDateTime::currentMSecsSinceEpoch();
        if(currentTime - timebase > 1000) {
            timebase = currentTime;
            frameAnt = frame;
            frame = 0;
        }
        QFont font;
        font.setPixelSize(25);
        font.setBold(false);
        glColor3f(1.0, 1.0, 1.0);
        QString fps;
        fps.setNum(frameAnt);
        QString text = "FPS: " + fps;
        this->renderText(10, 30, text, font);
    }

    void playSound(QMediaPlayer *sound){
        if(sound->state() == QMediaPlayer::PlayingState){
            sound->setPosition(0);
        }else if (sound->state() == QMediaPlayer::StoppedState){
            sound->play();
        }
    }

    void drawMap();
    void movePlayer(int dir);
    void showBoxes();
    void showInfo();
};

#endif // GAME0_H

