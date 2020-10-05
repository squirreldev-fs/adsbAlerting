#ifndef LOCATION_H
#define LOCATION_H

#include <QString>

/*!
 * \brief The Location class associates a name with GPS coordinates.
 */
class Location
{
private:
    QString name;
    double latitude, longitude;
public:
    /*!
     * \brief Location constructor.
     * \param name is the name of this location.
     * \param latitude in degrees (decimal notaion: no minutes, no seconds).
     * \param longitude in degrees (decimal notaion: no minutes, no seconds).
     */
    Location(QString name, double latitude = 0., double longitude = 0.);
    QString getName();
    double getLatitude();
    double getLongitude();
};

#endif // LOCATION_H
