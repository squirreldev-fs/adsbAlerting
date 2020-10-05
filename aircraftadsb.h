#ifndef AIRCRAFTADSB_H
#define AIRCRAFTADSB_H

/*!
 * \file aircraftadsb.h
 */

#include "aircraft.h"

/*!
 * \brief The AlertStatus enum listing possible alerting states relative to an aircraft.
 * Either no alert has been raised, or one is currenlty raised or one was raised and has been acknowledged by the user.
 */
enum AlertStatus{
    NoAlert, ///< No alert triggered. Will not be, according to the current implementation.
    AlertRaised, ///< Alert triggered but not yet acknowledged.
    AlertAcknowledged ///< Alert was triggered but the user acknowledged it.
};

/*!
 * \brief The AircraftADSB class adds all "live" information to Aircraft.
 */
class AircraftADSB : public Aircraft
{
public:
    /*!
     * \brief AircraftADSB will call the Aircraft constructor with "unknown" for every parameter, except for the icao.
     * \param icao is the minimum parameter required, since it is used as an identifier.
     */
    AircraftADSB(QString icao);

    /*!
     * \brief AircraftADSB will call the Aircraft constructor, passing all parameters. See Aircraft for details.
     */
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

    /*!
     * \brief getCallsignLive
     * \return callsign as written in ADS-B frames.
     */
    QString getCallsignLive() const;

    /*!
     * \brief seen is called every time the aircraft has been detected.
     * Updates the date containing the latest detection.
     */
    void seen();

    /*!
     * \brief notSeenForSec
     * \return number of seconds since the latest message received from this aircraft.
     */
    int notSeenForSec() const;

    /*!
     * \brief setAlertStatus sets the ::AlertStatus for this aircraft.
     * \param see ::AlertStatus.
     */
    void setAlertStatus(AlertStatus status);

    /*!
     * \brief getAlertStatus
     * \return see ::AlertStatus.
     */
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
