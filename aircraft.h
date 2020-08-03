#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QString>
#include <QTime>

class Aircraft
{
public:
    Aircraft(QString icao, QString registration, QString callsign, QString location, QString type, QString version);
    QString getIcao() const;
    void setIcao(QString icao);
    QString getRegistration() const;
    QString getCallsign() const;
    QString getLocation() const;
    QString getText() const;
    QString getType() const;
    QString getVersion() const;
    bool operator==(const QString& icao) const;

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

    void setAcknowleged(bool ack);
    bool isKnown();
private:
    QString icao;
    QString registration;
    QString callsign;
    QString location;
    QString version;
    QString type;

    int altitude=0;
    int speed=0;
    int heading=0;
    float latitude=0.f;
    float longitude=0.f;
    int verticalSpeed=0;
    int squawk=0;
    QString callsignLive="";

    QTime lastSeen;
    bool acknowleged;
};

#endif // AIRCRAFT_H
