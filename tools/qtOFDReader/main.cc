#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    QDesktopWidget *desktop = QApplication::desktop();
    mainWindow.move((desktop->width() - mainWindow.width())/2,
                    (desktop->height() - mainWindow.height())/2);
    mainWindow.show();

    return a.exec();
}
