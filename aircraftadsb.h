#ifndef AIRCRAFTADSB_H
#define AIRCRAFTADSB_H

#include "aircraft.h"

enum AlertStatus{NoAlert, AlertRaised, AlertAcknowledged};

class AircraftADSB : public Aircraft
{
public:
    AircraftADSB(QString icao);
    AircraftADSB(QString icao, QString registration, QString callsign, QString location, QString type, QString version);

    void setAltitude(int alti);
    int getAltitude() const;
    void setSpeed(int spd);
    int getSpeed() const;
    void setHeading(int hdg);
    int getHeading() const;
    void setLatitude(float lat);
    float getLatitude() const;
    void setLongitude(float lon);
    float getLongitude() const;
    void setVerticalSpeed(int vSpd);
    int getVerticalSpeed() const;
    void setSqawk(int sqk);
    int getSquawk() const;

    void setCallsignLive(QString callsign);
    QString getCallsignLive() const;

    void seen();
    int notSeenForSec() const;

    void setAlertStatus(AlertStatus status);
    AlertStatus getAlertStatus() const;

private:
    int altitude = 0;
    int speed = 0;
    int heading = 0;
    float latitude = 0.f;
    float longitude = 0.f;
    int verticalSpeed = 0;
    int squawk = 0;

    QString callsignLive = "";
    QTime lastSeen;
    AlertStatus alertStatus = NoAlert;
};

#endif // AIRCRAFTADSB_H
