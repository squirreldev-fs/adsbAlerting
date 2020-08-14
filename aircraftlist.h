#ifndef AIRCRAFTLIST_H
#define AIRCRAFTLIST_H

#include <QList>

#include "aircraftadsb.h"

class AircraftList: public QList<AircraftADSB>
{
public:
    AircraftList();
    bool contains(const QString& icao) const;
    int indexOfIcao(const QString& icao) const;
    int indexOfReg(const QString& reg) const;
};

#endif // AIRCRAFTLIST_H
