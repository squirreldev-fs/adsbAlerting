#include "settings.h"
#include "ui_settings.h"

#include <fstream>
#include <string>
#include <QMessageBox>
#include <QFileDialog>

Settings::Settings(QApplication *application, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    app = application;

    connect(ui->minutesBeforeRemovingSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setMinutesBeforeRemoved(int)));
    connect(ui->databaseButton, SIGNAL(clicked()), this, SLOT(setDatabasePath()));
    connect(ui->resourcesButton, SIGNAL(clicked()), this, SLOT(setResourcesPath()));
    connect(ui->dump1090Button, SIGNAL(clicked()), this, SLOT(setDump1090Path()));
    connect(ui->pPMShiftSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPPMShift(int)));
    connect(ui->netRoSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setNetRoSize(int)));
    connect(ui->netRoRateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setNetRoRate(int)));
    connect(ui->netBufferSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setNetBuffer(int)));

    readSettings();
    refreshDisplay();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::refreshDisplay()
{
    ui->minutesBeforeRemovingSpinBox->setValue(minutesBeforeRemoved);
    ui->databaseButton->setText(getPathText(&databasePath));
    ui->resourcesButton->setText(getPathText(&resourcesPath));
    ui->dump1090Button->setText(getPathText(&dump1090Path));
    ui->pPMShiftSpinBox->setValue(ppmShift);
    ui->netRoSizeSpinBox->setValue(netRoSize);
    ui->netRoRateSpinBox->setValue(netRoRate);
    ui->netBufferSpinBox->setValue(netBuffer);
}

int Settings::getMinutesBeforeRemoved()
{
    return minutesBeforeRemoved;
}

QFile* Settings::getDatabase()
{
    return &databasePath;
}

QString Settings::getResourcesPath()
{
    return resourcesPath.fileName()+"/";
}

QString Settings::getDump1090Path()
{
    return dump1090Path.fileName();
}

int Settings::getPPM()
{
    return ppmShift;
}

int Settings::getNetRoSize()
{
    return netRoSize;
}

int Settings::getNetRoRate()
{
    return netRoRate;
}

int Settings::getNetBuffer()
{
    return netBuffer;
}

void Settings::accept()
{
    if(allPathsValid())
    {
        writeSettings();
        this->hide();
    }
    else
    {
        QMessageBox::warning(this, "Paths", "Please enter valid paths.");
    }
}

void Settings::reject()
{
    readSettings();
    if(allPathsValid())
    {
        this->hide();
    }
    else
    {
        QMessageBox::critical(this, "Paths", "Invalid path. The application will close.");
        app->quit();
    }
}

void Settings::conditionalShow()
{
    if(!allPathsValid())
    {
        this->show();
    }
}

void Settings::setMinutesBeforeRemoved(int value)
{
    minutesBeforeRemoved = value;
}

void Settings::setDatabasePath()
{
    databasePath.setFileName(QFileDialog::getOpenFileName(this, "Database", QString(), "CSV (*.csv)"));
    refreshDisplay();
}

void Settings::setResourcesPath()
{
    resourcesPath.setFileName(QFileDialog::getExistingDirectory(this));
    refreshDisplay();
}

void Settings::setDump1090Path()
{
    dump1090Path.setFileName(QFileDialog::getOpenFileName(this, "Dump1090", QString(), "Application (*)"));
    refreshDisplay();
}

void Settings::setPPMShift(int value)
{
    ppmShift = value;
}

void Settings::setNetRoSize(int value)
{
    netRoSize = value;
}

void Settings::setNetRoRate(int value)
{
    netRoRate = value;
}

void Settings::setNetBuffer(int value)
{
    netBuffer = value;
}

void Settings::readSettings()
{
    std::ifstream settingsReader("settings.ini");
    if(settingsReader)
    {
        std::string keyword, value;
        while(settingsReader >> keyword)
        {
            if("minutesBeforeRemoved" == keyword)
            {
                settingsReader >> minutesBeforeRemoved;
            }
            else if("database" == keyword)
            {
                settingsReader >> value;
                databasePath.setFileName(value.c_str());
            }
            else if("resources" == keyword)
            {
                settingsReader >> value;
                resourcesPath.setFileName(value.c_str());
            }
            else if("dump1090" == keyword)
            {
                settingsReader >> value;
                dump1090Path.setFileName(value.c_str());
            }
            else if("ppm" == keyword)
            {
                settingsReader >> ppmShift;
            }
            else if("net-ro-size" == keyword)
            {
                settingsReader >> netRoSize;
            }
            else if("net-ro-rate" == keyword)
            {
                settingsReader >> netRoRate;
            }
            else if("net-buffer" == keyword)
            {
                settingsReader >> netBuffer;
            }

        }
    }
}

void Settings::writeSettings()
{
    std::ofstream settingsWriter("settings.ini");
    if(settingsWriter)
    {
        settingsWriter << "minutesBeforeRemoved " << minutesBeforeRemoved << std::endl
                       << "database " << databasePath.fileName().toStdString() << std::endl
                       << "resources " << resourcesPath.fileName().toStdString() << std::endl
                       << "dump1090 " << dump1090Path.fileName().toStdString() << std::endl
                       << "ppm " << ppmShift << std::endl
                       << "net-ro-size " << netRoSize << std::endl
                       << "net-ro-rate " << netRoRate << std::endl
                       << "net-buffer " << netBuffer << std::endl;
    }
}

bool Settings::allPathsValid()
{
    return databasePath.exists() && resourcesPath.exists() && dump1090Path.exists();
}

QString Settings::getPathText(QFile *file)
{
    if(file->exists())
    {
        return file->fileName();
    }
    else
    {
        return QString("Set path");
    }
}
