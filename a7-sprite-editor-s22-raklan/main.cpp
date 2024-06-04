#include "mainwindow.h"

#include <QDebug>
#include "frame.h"
#include "model.h"
#include "mainmenu.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu mainMenu;
    mainMenu.show();
    return a.exec();
}
