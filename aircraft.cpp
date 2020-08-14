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
