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

    connect(ui->databaseButton, SIGNAL(clicked()), this, SLOT(setDatabasePath()));
    connect(ui->resourcesButton, SIGNAL(clicked()), this, SLOT(setResourcesPath()));
    connect(ui->dump1090Button, SIGNAL(clicked()), this, SLOT(setDump1090Path()));

    readSettings();
    refreshPaths();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::refreshPaths()
{
    ui->databaseButton->setText(getPathText(&databasePath));
    ui->resourcesButton->setText(getPathText(&resourcesPath));
    ui->dump1090Button->setText(getPathText(&dump1090Path));
}

QFile* Settings::getDatabase()
{
    return &databasePath;
}

QString Settings::getResourcesPath()
{
    return resourcesPath.fileName();
}

QString Settings::getDump1090Path()
{
    return dump1090Path.fileName();
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

void Settings::setDatabasePath()
{
    databasePath.setFileName(QFileDialog::getOpenFileName(this, "Database", QString(), "CSV (*.csv)"));
    refreshPaths();
}

void Settings::setResourcesPath()
{
    resourcesPath.setFileName(QFileDialog::getExistingDirectory(this));
    refreshPaths();
}

void Settings::setDump1090Path()
{
    dump1090Path.setFileName(QFileDialog::getOpenFileName(this, "Dump1090", QString(), "Application (*)"));
    refreshPaths();
}

void Settings::readSettings()
{
    std::ifstream settingsReader("settings.ini");
    if(settingsReader)
    {
        std::string keyword, value;
        while(settingsReader >> keyword >> value)
        {
            if("database" == keyword)
            {
                databasePath.setFileName(value.c_str());
            }
            else if("resources" == keyword)
            {
                resourcesPath.setFileName(value.c_str());
            }
            else if("dump1090" == keyword)
            {
                dump1090Path.setFileName(value.c_str());
            }
        }
    }
}

void Settings::writeSettings()
{
    std::ofstream settingsWriter("settings.ini");
    if(settingsWriter)
    {
        settingsWriter << "database " << databasePath.fileName().toStdString() << std::endl
                       << "resources " << resourcesPath.fileName().toStdString() << std::endl
                       << "dump1090 " << dump1090Path.fileName().toStdString() << std::endl;
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
