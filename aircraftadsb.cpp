#include "aircraftadsb.h"

AircraftADSB::AircraftADSB(QString icao, QString registration, QString callsign, QString location, QString type, QString version) :
    Aircraft(icao, registration, callsign, location, type, version)
{

}

// Flight data

void AircraftADSB::setAltitude(int alti)
{
    altitude = alti;
}

int AircraftADSB::getAltitude() const
{
    return altitude;
}

void AircraftADSB::setSpeed(int spd)
{
    speed = spd;
}

int AircraftADSB::getSpeed() const
{
    return speed;
}

void AircraftADSB::setHeading(int hdg)
{
    heading = hdg;
}

int AircraftADSB::getHeading() const
{
    return heading;
}

void AircraftADSB::setLatitude(float lat)
{
    latitude = lat;
}

float AircraftADSB::getLatitude() const
{
    return latitude;
}

void AircraftADSB::setLongitude(float lon)
{
    longitude = lon;
}

float AircraftADSB::getLongitude() const
{
    return longitude;
}

void AircraftADSB::setVerticalSpeed(int vSpd)
{
    verticalSpeed = vSpd;
}

int AircraftADSB::getVerticalSpeed() const
{
    return verticalSpeed;
}

void AircraftADSB::setSqawk(int sqk)
{
    squawk = sqk;
}

int AircraftADSB::getSquawk() const
{
    return squawk;
}

void AircraftADSB::setCallsignLive(QString callsign)
{
    callsignLive = callsign;
}

QString AircraftADSB::getCallsignLive() const
{
    return callsignLive;
}

// Seen timer

void AircraftADSB::seen()
{
    lastSeen = QTime::currentTime();
}

int AircraftADSB::notSeenForSec() const
{
    return lastSeen.secsTo(QTime::currentTime());
}

// Alerts

void AircraftADSB::setAcknowleged(bool ack)
{
    acknowleged = ack;
}

bool AircraftADSB::isKnown()
{
    return acknowleged;
}
