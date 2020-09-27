#include "location.h"

Location::Location(QString name, double latitude, double longitude)
{
    this->name = name;
    this->latitude = latitude;
    this->longitude = longitude;
}

QString Location::getName() {return name;}
double Location::getLatitude() {return latitude;}
double Location::getLongitude() {return longitude;}
