#include "menu.h"
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Menu w;
    w.setMinimumSize(800, 600);
    w.setMaximumSize(800, 600);
    w.setGeometry( //Iniciar a tela no centro
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    w.size(),
                    qApp->desktop()->availableGeometry()
                    )
                );

    w.show();
    return a.exec();
}
