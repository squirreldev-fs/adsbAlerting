#include "mainwindow.h"
#include "settings.h"
#include "locations.h"

#include "aircraftlist.h"
#include "adsbframereader.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AircraftList acfDatabase;
    AircraftList acfLive;

    Locations locations(&a);
    Settings settings(&a);
    MainWindow window(&acfLive, &acfDatabase, &settings, &locations);

    ADSBFrameReader adsbReader(&acfLive, &acfDatabase, &window);

    window.show();
    settings.conditionalShow();
    locations.show();

    return a.exec();
}
