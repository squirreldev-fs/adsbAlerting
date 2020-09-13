#include "mainwindow.h"
#include "settings.h"
#include "aircraftlist.h"
#include "adsbframereader.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AircraftList acfDatabase;
    AircraftList acfLive;

    Settings settings(&a);
    MainWindow window(&acfLive, &acfDatabase, &settings);

    ADSBFrameReader adsbReader(&acfLive, &acfDatabase, &window);

    window.show();
    settings.conditionalShow();

    return a.exec();
}
