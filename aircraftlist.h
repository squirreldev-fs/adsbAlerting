#ifndef AIRCRAFTLIST_H
#define AIRCRAFTLIST_H

#include <QList>

#include "aircraft.h"

class AircraftList: public QList<Aircraft>
{
public:
    AircraftList();
    bool contains(const QString& icao) const;
    int indexOfIcao(const QString& icao) const;
    int indexOfReg(const QString& reg) const;
};

#endif // AIRCRAFTLIST_H
