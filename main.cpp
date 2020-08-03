#include "mainwindow.h"
#include "aircraft.h"
#include "aircraftlist.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(nullptr, "D:/Cam_Laptop/Documents/Aeronautique/ADSB/database.csv");
    w.show();

    return a.exec();
}
