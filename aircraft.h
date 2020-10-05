#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QString>
#include <QTime>

/*!
 * \brief The Aircraft class represents an aircraft, whatever its status is.
 * Only "static" data is contained here (data changing rarely during aircraft's life).
 */
class Aircraft
{
public:
    /*!
     * \brief Aircraft constructor.
     * \param icao is the transponder icao unique identifier (six hexadecimal characters).
     * \param registration is the registration as painted on the airframe.
     * \param callsign is the callsign used by ATC. Often same as the registration for general aviation.
     * \param location is the location (base) of the aircraft.
     * \param type is the type of the aircraft, <a href="https://www.icao.int/publications/DOC8643/Pages/Search.aspx">"type designator" as defined by the ICAO</a>.
     * \param version is the version of the aircraft, to give more details than type.
     */
    Aircraft(QString icao, QString registration, QString callsign, QString location, QString type, QString version);

    QString getIcao() const;

    QString getRegistration() const;

    QString getCallsign() const;

    QString getLocation() const;

    /*!
     * \brief getText returns a summary about this aircraft. Useful for debug.
     * \return icao: registration, callsign from location
     */
    QString getText() const;

    QString getType() const;

    QString getVersion() const;

    /*!
     * \brief operator == overload of == operator for Aircraft class. Uses only the icao.
     * \param icao to be compared.
     * \return icao == this->icao.
     */
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
