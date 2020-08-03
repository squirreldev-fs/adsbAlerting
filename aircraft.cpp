#include "aircraft.h"

Aircraft::Aircraft(QString icao, QString registration, QString callsign, QString location, QString type, QString version)
{
    if(registration == "")
    {
        registration = "Unknown";
    }
    if(callsign == "")
    {
        callsign = "Unknown";
    }
    if(location == "")
    {
        location = "Unknown";
    }
    if(type == "")
    {
        type = "Uknw";
    }
    if(version == "")
    {
        version = "Uknw";
    }
    this->icao = icao;
    this->registration = registration;
    this->callsign = callsign;
    this->location = location;
    this->type = type;
    this->version = version;
    acknowleged = false;
}

QString Aircraft::getIcao() const
{
    return icao;
}

void Aircraft::setIcao(QString icao)
{
    this->icao = icao;
}

QString Aircraft::getRegistration() const
{
    return registration;
}

QString Aircraft::getCallsign() const
{
    return callsign;
}

QString Aircraft::getLocation() const
{
    return location;
}

QString Aircraft::getText() const
{
    return icao+": "+registration+", "+callsign+" from "+location;
}

QString Aircraft::getType() const
{
    return type;
}

QString Aircraft::getVersion() const
{
    return version;
}

bool Aircraft::operator==(const QString& icao) const
{
    return icao == this->icao;
}



// Flight data

void Aircraft::setAltitude(int alti)
{
    altitude = alti;
}

int Aircraft::getAltitude() const
{
    return altitude;
}

void Aircraft::setSpeed(int spd)
{
    speed = spd;
}

int Aircraft::getSpeed() const
{
    return speed;
}

void Aircraft::setHeading(int hdg)
{
    heading = hdg;
}

int Aircraft::getHeading() const
{
    return heading;
}

void Aircraft::setLatitude(float lat)
{
    latitude = lat;
}

float Aircraft::getLatitude() const
{
    return latitude;
}

void Aircraft::setLongitude(float lon)
{
    longitude = lon;
}

float Aircraft::getLongitude() const
{
    return longitude;
}

void Aircraft::setVerticalSpeed(int vSpd)
{
    verticalSpeed = vSpd;
}

int Aircraft::getVerticalSpeed() const
{
    return verticalSpeed;
}

void Aircraft::setSqawk(int sqk)
{
    squawk = sqk;
}

int Aircraft::getSquawk() const
{
    return squawk;
}

void Aircraft::setCallsignLive(QString callsign)
{
    callsignLive = callsign;
}

QString Aircraft::getCallsignLive() const
{
    return callsignLive;
}


// Seen timer

void Aircraft::seen()
{
    lastSeen = QTime::currentTime();
}

int Aircraft::notSeenForSec() const
{
    return lastSeen.secsTo(QTime::currentTime());
}



// Alerts

void Aircraft::setAcknowleged(bool ack)
{
    acknowleged = ack;
}

bool Aircraft::isKnown()
{
    return acknowleged;
}
