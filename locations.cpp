#include "locations.h"
#include "ui_locations.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMessageBox>

// public

Locations::Locations(QApplication *application, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Locations),
    currentLocation(QString(""))
{
    ui->setupUi(this);

    app = application;

    connect(ui->buttonNewLocation, SIGNAL(clicked()), this, SLOT(addNewLocation()));
    connect(ui->buttonDeleteSelected, SIGNAL(clicked()), this, SLOT(deleteSelectedLocation()));
    connect(ui->listLocations, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

    readFile();
    clearEntries();
}

Locations::~Locations()
{
    delete ui;
}

void Locations::readFile()
{
    QFile file("Locations.csv");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(";");
            Location newLoc(fields.at(0), fields.at(1).toDouble(), fields.at(2).toDouble());
            locations.push_back(newLoc);
            ui->listLocations->addItem(locations.back().getName());
            ui->listLocations->setCurrentRow(0);
        }
    }

    file.close();
}

void Locations::writeFile()
{
    QFile file("Locations.csv");
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        // front = latest
        for(size_t i=0;i<locations.size();i++)
        {
            out << locations.at(i).getName() << ";"
                << locations.at(i).getLatitude() << ";"
                << locations.at(i).getLongitude() << endl;
        }
    }

    file.close();
}

void Locations::clearEntries()
{
    ui->lineLocName->setText("New location");
    ui->doubleLatitude->setValue(0.);
    ui->doubleLongitude->setValue(0.);
}

Location Locations::getCurrentLocation()
{
    return locations.back();
}

// public slots

void Locations::accept()
{
    if(ui->listLocations->selectionModel()->hasSelection())
    {
        size_t selectionIndex = ui->listLocations->selectionModel()->selectedRows().back().row();
        currentLocation = locations.at(selectionIndex);

        // move it to the last position
        locations.erase(locations.begin()+selectionIndex);
        locations.insert(locations.begin(), currentLocation);

        // sync view
        ui->listLocations->takeItem(int(selectionIndex));
        ui->listLocations->insertItem(0, currentLocation.getName());

        writeFile();
        this->hide();
    }
    else
    {
        QMessageBox::warning(this, "Location", "Please select a valid location.");
    }
}

void Locations::reject()
{
    if(currentLocation.getName() == "")
    {
        QMessageBox::critical(this, "Location", "No valid location. The application will close.");
        app->quit();
    }
    else
    {
        this->hide();
    }
}

void Locations::selectionChanged()
{
    if(ui->listLocations->selectionModel()->hasSelection())
    {
        ui->buttonDeleteSelected->setEnabled(true);
        ui->buttonBox->button(ui->buttonBox->Ok)->setEnabled(true);
    }
    else
    {
        ui->buttonDeleteSelected->setEnabled(false);
        ui->buttonBox->button(ui->buttonBox->Ok)->setEnabled(false);
    }
}

void Locations::addNewLocation()
{
    QString newName = ui->lineLocName->text();

    bool isADuplicate = false;
    for(size_t i=0; i<locations.size(); i++)
    {
        if(locations.at(i).getName() == newName)
        {
            isADuplicate = true;
        }
    }

    if(isADuplicate || "" == newName)
    {
        QMessageBox::warning(this, "Locations names", "Please enter a distinctive name.");
    }
    else
    {
        Location newLoc(newName,
                        ui->doubleLatitude->value(),
                        ui->doubleLongitude->value());
        locations.insert(locations.begin(),newLoc);
        ui->listLocations->insertItem(0, locations.front().getName());
        ui->listLocations->setCurrentRow(0);
    }
}

void Locations::deleteSelectedLocation()
{
    if(ui->listLocations->selectionModel()->hasSelection())
    {
        size_t selectionIndex = ui->listLocations->selectionModel()->selectedRows().back().row();
        // erase from vector
        locations.erase(locations.begin()+selectionIndex);
        // erase from list on the UI
        ui->listLocations->takeItem(int(selectionIndex));
    }
}
