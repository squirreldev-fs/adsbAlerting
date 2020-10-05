#ifndef AIRCRAFTLIST_H
#define AIRCRAFTLIST_H

#include <QList>

#include "aircraftadsb.h"

/*!
 * \brief The AircraftList class extends QList for AircraftADSB objects.
 */
class AircraftList: public QList<AircraftADSB>
{
public:
    AircraftList();

    /*!
     * \brief contains
     * \param icao is the icao searched in the list.
     * \return a boolean: if there is an aircraft identified by the icao.
     */
    bool contains(const QString& icao) const;

    /*!
     * \brief indexOfIcao
     * \param icao is the unique identifier code sent by the transponder.
     * \return the index in the list of the aircraft having the passed icao. -1 if not found.
     */
    int indexOfIcao(const QString& icao) const;

    /*!
     * \brief indexOfReg
     * \param reg is an aircraft registration (as painted on the airframe).
     * \return the index in the list of the aircraft having the passed registration. -1 if not found.
     */
    int indexOfReg(const QString& reg) const;
};

#endif // AIRCRAFTLIST_H
