#ifndef LOCATION_H
#define LOCATION_H

#include <QString>

class Location
{
private:
    QString name;
    double latitude, longitude;
public:
    Location(QString name, double latitude = 0., double longitude = 0.);
    QString getName();
    double getLatitude();
    double getLongitude();
};

#endif // LOCATION_H
