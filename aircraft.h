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

private:
    QString icao = "";
    QString registration = "";
    QString callsign = "";
    QString location = "";
    QString version = "";
    QString type = "";
};

#endif // AIRCRAFT_H
