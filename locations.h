#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <QDialog>
#include "location.h"
#include <QApplication>
#include <vector>

namespace Ui {
class Locations;
}

class Locations : public QDialog
{
    Q_OBJECT

public:
    explicit Locations(QApplication *application, QWidget *parent = nullptr);
    ~Locations();

    void conditionalShow();

    void readFile();
    void writeFile();
    void clearEntries();
    Location getCurrentLocation();

public slots:
    void accept() override;
    void reject() override;

    void selectionChanged();
    void addNewLocation();
    void deleteSelectedLocation();

signals:
    void currentLocationChanged();

private:
    Ui::Locations *ui;

    QApplication *app;

    std::vector<Location> locations;
    Location currentLocation;
};

#endif // LOCATIONS_H
