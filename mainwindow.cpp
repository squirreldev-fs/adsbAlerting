#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QSound>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

#ifdef WIN32
    #include <Windows.h>
#endif

MainWindow::MainWindow(AircraftList *live, AircraftList *database, QString databasePath, QString ressourcesPath, QString dump1090App, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    seenAcf = live;
    interestingAcf = database;

    this->database.setFileName(databasePath);
    this->ressourcesPath = ressourcesPath;
    this->dump1090App = dump1090App;

    // windows
    ui->setupUi(this);
    this->setWindowTitle("ADS-B Receiver");
    this->resize(452, this->height());

    ui->infoLayout->insertWidget(0, &picture);

    soundIcon = QIcon(ressourcesPath+"sound.png");
    mutedIcon = QIcon(ressourcesPath+"muted.png");
    ui->bMute->setIcon(soundIcon);

    connect(ui->bAcfTableFilter, SIGNAL(clicked()), this, SLOT(filterAcfList()));
    connect(ui->bAcknowlege, SIGNAL(clicked()), this, SLOT(aknowlegeAlerts()));
    connect(ui->bReloadDB, SIGNAL(clicked()), this, SLOT(reloadDatabase()));
    connect(&alarmTimer, SIGNAL(timeout()), this, SLOT(triggerAlerts()));
    connect(ui->splitter, SIGNAL(clicked()), this, SLOT(toggleInfoVisi()));
    connect(ui->acfTable, SIGNAL(itemSelectionChanged()), this, SLOT(newAcfSelected()));
    connect(ui->bAddToDB, SIGNAL(clicked()), this, SLOT(addToDatabase()));

    // mute / restor sound for alerts
    connect(ui->bMute, SIGNAL(clicked()), this, SLOT(toggleMute()));
    liveTimer.setSingleShot(true);
    connect(&liveTimer, SIGNAL(timeout()), this, SLOT(updateLiveInfo()));

    // Dump1090 launcher
    connect(ui->bDump1090, SIGNAL(clicked()), this, SLOT(lauchDump1090()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(readDumpOutput()));

    connect(ui->bWebInfo, SIGNAL(clicked()), this, SLOT(LaunchWebForInfo()));

    // init
    ui->bAcknowlege->setEnabled(false);
    this->reloadDatabase();
    this->setInfoVisi(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectionFailed()
{
    ui->bDump1090->setEnabled(true);
}

void MainWindow::connectionSucceeded()
{
    ui->bDump1090->setEnabled(false);
}

void MainWindow::raiseAlert()
{
    triggerAlerts();
    alarmTimer.start(alarmIntervalMS);
    ui->bAcknowlege->setEnabled(true);
}

void MainWindow::aknowlegeAlerts()
{
    for(int i=0;i<seenAcf->size();i++)
    {
        (*seenAcf)[i].setAlertStatus(AlertAcknowledged);
    }
    alarmTimer.stop();
    ui->bAcknowlege->setEnabled(false);
}

void MainWindow::triggerAlerts()
{
    bool alarm = false;
    for(int i=0;i<seenAcf->size();i++)
    {
        if(seenAcf->at(i).getAlertStatus() == AlertRaised)
        {
            alarm = true;
        }
    }
    if(alarm)
    {
        if(!muted)
        {
            QSound::play(ressourcesPath+"bell.wav");
        }
    }
    else
    {
        alarmTimer.stop();
    }
}

void MainWindow::toggleMute()
{
    muted = !muted;
    if(muted)
    {
        ui->bMute->setIcon(mutedIcon);
    }
    else {
        ui->bMute->setIcon(soundIcon);
    }
}

void MainWindow::filterAcfList()
{
    showingOnlyRecognized = ui->bAcfTableFilter->isChecked();
    redrawAcfList();
}

void MainWindow::redrawAcfList()
{
    this->resetInfo();
    ui->acfTable->setRowCount(0);

    for(int i=0;i<seenAcf->size();i++)
    {
        if(!showingOnlyRecognized || interestingAcf->contains(seenAcf->at(i).getIcao()))
        {
            ui->acfTable->insertRow(0);
            ui->acfTable->setItem(0,0,new QTableWidgetItem(seenAcf->at(i).getIcao()));
            ui->acfTable->setItem(0,1,new QTableWidgetItem(seenAcf->at(i).getCallsignLive()));
            ui->acfTable->setItem(0,2,new QTableWidgetItem(QString::number(seenAcf->at(i).getAltitude())));
        }
    }
}

void MainWindow::newAcfSelected()
{
    resetInfo();
    if(!seenAcf->isEmpty() && ui->acfTable->rowCount() > 0)
    {
        int row = ui->acfTable->currentRow();
        if(row >= 0 && row < ui->acfTable->rowCount()) // meaningful selection
        {
            QString icao = ui->acfTable->item(row, 0)->text();
            int index = interestingAcf->indexOfIcao(icao);
            if(index > -1)
            {
                Aircraft acf = interestingAcf->at(index); // DATABASE
                // based on registration because of test aircraft

                ui->immatriculationField->setText(acf.getRegistration());
                ui->typeField->setText(acf.getType());
                ui->versionField->setText(acf.getVersion());
                ui->callsignField->setText(acf.getCallsign());
                ui->baseField->setText(acf.getLocation());
                picture.setPicturePath(ressourcesPath+acf.getRegistration()+".jpg");
                this->updateLiveInfo();
            }
        }
    }
}

void MainWindow::updateListInfo(QString icao, ColumnsType column, QString value)
{
    switch(column)
    {
    case IcaoClmn:
        if(!showingOnlyRecognized || interestingAcf->contains(icao))
        {
            ui->acfTable->insertRow(0);
            ui->acfTable->setItem(0,IcaoClmn,new QTableWidgetItem(value));
        }
        break;
    default:
        for(int i=0;i<ui->acfTable->rowCount();i++)
        {
            QTableWidgetItem *item = ui->acfTable->item(i, 0);
            if(item && item->text() == icao)
            {
                ui->acfTable->setItem(i,column,new QTableWidgetItem(value));
            }
        }
    }
}

void MainWindow::reloadDatabase()
{
    if (database.open(QIODevice::ReadOnly | QIODevice::Text)) {
        interestingAcf->clear();
        database.readLine(); // skip titles
        while (!database.atEnd()) {
            QList<QByteArray> line = database.readLine().split(',');
            QString icao = line.at(5);
            icao=icao.remove('"');
            QString registration = line.at(0);
            registration=registration.remove('"');
            QString callsign = line.at(7);
            callsign=callsign.remove('"');
            QString location = line.at(6);
            location=location.remove('"');
            QString type = line.at(1);
            type=type.remove('"');
            QString version = line.at(2);
            version=version.remove('"');
            AircraftADSB acf = AircraftADSB(icao, registration, callsign, location, type, version);
            interestingAcf->append(acf);
        }
        database.close();
    }
}

void MainWindow::addToDatabase()
{
    if(!seenAcf->isEmpty() && ui->acfTable->rowCount() > 0)
    {
        int row = ui->acfTable->currentRow();
        if(row >= 0 && row < ui->acfTable->rowCount()) // meaningful selection
        {
            QString icao = ui->acfTable->item(row, 0)->text();
            int index = interestingAcf->indexOfIcao(icao);
            if(index < 0) // not existing yet
            {
                interestingAcf->append(AircraftADSB(icao));
                if(database.open(QIODevice::Append))
                {
                    QString newLine = ",,,,," + icao + ",,,\r\n";
                    database.write(newLine.toStdString().c_str());
                }
                database.close();
            }
        }
    }
}

void MainWindow::setInfoVisi(bool visible)
{
    if(visible)
    {
        ui->infoFrame->show();
        ui->splitter->setText("<<");
    }
    else {
        smallWidth = this->width() - ui->infoFrame->width();
        ui->infoFrame->hide();
        ui->splitter->setText(">>");
        resizeTimer.singleShot(100, this, SLOT(resizeWindow()));
    }
}

void MainWindow::toggleInfoVisi()
{
    this->setInfoVisi(!ui->infoFrame->isVisible());
}

void MainWindow::resizeWindow()
{
    this->resize(smallWidth, this->height());
}

void MainWindow::updateLiveInfo()
{
    if(!seenAcf->isEmpty() && ui->acfTable->currentRow() != -1)
    {
        int row = ui->acfTable->currentRow();
        QString icao = ui->acfTable->item(row, 0)->text();
        int index = seenAcf->indexOfIcao(icao);
        if(index > -1) // found
        {
            AircraftADSB acf = seenAcf->at(index); // SEEN DATABASE

            ui->squawkField->setText(QString::number(acf.getSquawk()));
            ui->callsignLiveField->setText(acf.getCallsignLive());
            ui->altitudeField->setText(QString::number(acf.getAltitude()));
            ui->vSpeedField->setText(QString::number(acf.getVerticalSpeed()));
            ui->speedField->setText(QString::number(acf.getSpeed()));
            ui->headingField->setText(QString::number(acf.getHeading()));
            ui->latitudeField->setText(QString::number(double(acf.getLatitude())));
            ui->longitudeField->setText(QString::number(double(acf.getLongitude())));
            bool isInLive = acf.notSeenForSec() < 30;
            ui->radioInLive->setChecked(isInLive); // seen less than 30 sec ago
            if(isInLive)
            {
                // is it still there 30 sec after last msg?
            #ifdef WIN32
                liveTimer.start(max(32-acf.notSeenForSec(),0));
            #else
                liveTimer.start(std::max(32000-acf.notSeenForSec()*1000,0));
            #endif
            }
        }
        else {
            this->resetInfo();
        }
    }
    else {
        this->resetInfo();
    }
}

void MainWindow::resetInfo()
{
    ui->immatriculationField->setText("");
    ui->typeField->setText("");
    ui->versionField->setText("");
    ui->callsignField->setText("");
    ui->baseField->setText("");
    ui->squawkField->setText("");
    ui->callsignLiveField->setText("");
    ui->altitudeField->setText("");
    ui->vSpeedField->setText("");
    ui->speedField->setText("");
    ui->headingField->setText("");
    ui->latitudeField->setText("");
    ui->longitudeField->setText("");
    ui->radioInLive->setChecked(false);
    picture.setPicturePath("");
}

void MainWindow::lauchDump1090()
{
#ifdef WIN32
    process.setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags &= ~ulong(CREATE_NO_WINDOW);
    });
#endif
    process.setProgram(dump1090App);
    process.setArguments({"--net",
                         "--net-ro-size", "500",
                         "--net-ro-rate", "5",
                         "--net-buffer", "5",
                         "--ppm", "41",
                         "--lat", "48.72",
                         "--lon", "10.78"});
    QDir dir = QDir(dump1090App);
    dir.cdUp();
    process.setWorkingDirectory(dir.absolutePath());
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start();
}

void MainWindow::readDumpOutput()
{
    QString text = process.readAll();
    if(text.contains("No supported RTLSDR devices found."))
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText("No supported RTLSDR device found by dump1090.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
    else if(text.contains("Error opening the RTLSDR device: No error"))
    {
        msgBox.setWindowTitle("Error");
        msgBox.setText("Dump1090 may be already running.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void MainWindow::LaunchWebForInfo()
{
    if(!seenAcf->isEmpty() && ui->acfTable->rowCount() > 0)
    {
        int row = ui->acfTable->currentRow();
        if(row >= 0 && row < ui->acfTable->rowCount()) // meaningful selection
        {
            QString icao = ui->acfTable->item(row, 0)->text();
            QDesktopServices::openUrl(QUrl("https://opensky-network.org/aircraft-profile?icao24="+icao));
        }
    }
}
