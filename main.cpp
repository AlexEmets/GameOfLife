#include "mainwindow.h"
#include <QApplication>
#include<iostream>
#include<fstream>
#include<string>

#include <unistd.h>

std::string getCurrentWorkingDirectory() {
    char tmp[256];
    getcwd(tmp, 256);
    return std::string(tmp);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1900,1200);

    std::string nameOfPattern(argv[1]);
    w.getTableFromPattern(nameOfPattern);

    w.show();

    usleep(5000);
    w.execLifeGame();

    return a.exec();
}
