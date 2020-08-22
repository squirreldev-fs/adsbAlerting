#include "mainwindow.h"
#include "aircraftlist.h"
#include "adsbframereader.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AircraftList acfDatabase;
    AircraftList acfLive;

    MainWindow window(&acfLive, &acfDatabase, "D:/Cam_Laptop/Documents/Aeronautique/ADSB/database.csv");
    ADSBFrameReader adsbReader(&acfLive, &acfDatabase, &window);

    window.show();

    return a.exec();
}
