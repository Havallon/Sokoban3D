#include "menu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Menu w;
    w.setMinimumSize(800, 600);
    w.setMaximumSize(800, 600);
    w.show();
    return a.exec();
}