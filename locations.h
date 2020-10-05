#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <QDialog>
#include "location.h"
#include <QApplication>
#include <vector>

namespace Ui {
    class Locations;
}

/*!
 * \brief The Locations class is both a container for saved Location and a window to handle them.
 */
class Locations : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Locations constructor.
     * \param application is used to exit the application in some specific cases.
     * \param parent is passed to the QDialog constructor. Kept for future use.
     */
    explicit Locations(QApplication *application, QWidget *parent = nullptr);
    ~Locations();

    /*!
     * \brief conditionalShow shows the window only if there is no Location defined yet.
     * It is used at the start of the application, to ensure that the user defines a location before using the application.
     */
    void conditionalShow();

    /*!
     * \brief clearEntries resets the user interfaces (new location's name and coordinates).
     */
    void clearEntries();

    /*!
     * \brief getCurrentLocation
     * \return Location selected by the user. The first one in Locations.csv by default.
     */
    Location getCurrentLocation();

public slots:
    /*!
     * \brief accept sets current Location, save locations list or asks to select a location before hiding the locations window.
     */
    void accept() override;

    /*!
     * \brief reject hides the locations window if a least a Location is defined. Otherwise it closes the application.
     */
    void reject() override;

    /*!
     * \brief selectionChanged is called when the selected Location is changed. It activates "ok" and "delete selected" buttons.
     */
    void selectionChanged();

    /*!
     * \brief addNewLocation is called to add a new location: parameters defined in the UI are copied to create a new Location object.
     */
    void addNewLocation();

    /*!
     * \brief deleteSelectedLocation is called to remove the selected Location from the list of those in memory.
     */
    void deleteSelectedLocation();

signals:
    /*!
     * \brief currentLocationChanged is emitted when a Location is selected and the button "ok" is pressed.
     */
    void currentLocationChanged();

private:
    Ui::Locations *ui;

    QApplication *app;

    std::vector<Location> locations;
    Location currentLocation;

    /*!
     * \brief readFile reads saved locations contained in Locations.csv.
     */
    void readFile();

    /*!
     * \brief writeFile writes locations to Locations.csv.
     */
    void writeFile();
};

#endif // LOCATIONS_H
