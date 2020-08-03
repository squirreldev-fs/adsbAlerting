#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>
#include <QSound>
#include <QProcess>
#include <QMessageBox>

#ifdef WIN32
    #include <Windows.h>
#endif

MainWindow::MainWindow(QWidget *parent, QString database) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _socket(this)
{
    this->database.setFileName(database);

    // windows
    ui->setupUi(this);
    this->setWindowTitle("ADS-B Receiver");
    this->resize(452, this->height());

    ui->infoLayout->insertWidget(0, &picture);

    soundIcon = QIcon("D:/Cam_Laptop/Documents/Aeronautique/ADSB/resources/sound.png");
    mutedIcon = QIcon("D:/Cam_Laptop/Documents/Aeronautique/ADSB/resources/muted.png");
    ui->bMute->setIcon(soundIcon);

    // connection to dump 1090
    connectionTimer.setSingleShot(true);
    establishConnection();
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionErrorsH(QAbstractSocket::SocketError)));
    connect(&connectionTimer, SIGNAL(timeout()), this, SLOT(establishConnection()));
    connect(&_socket, SIGNAL(connected()), this, SLOT(connectionEstablished()));

    // acknowlege
    connect(ui->bAcknowlege, SIGNAL(clicked()), this, SLOT(aknowlegeAlerts()));

    // reset list
    connect(ui->bResetList, SIGNAL(clicked()), this, SLOT(clearAcfList()));

    // reload database
    connect(ui->bReloadDB, SIGNAL(clicked()), this, SLOT(reloadDatabase()));

    // alarms
    connect(&alarmTimer, SIGNAL(timeout()), this, SLOT(triggerAlerts()));

    // splitter
    connect(ui->splitter, SIGNAL(clicked()), this, SLOT(toggleInfoVisi()));

    // selected acf
    connect(ui->acfTable, SIGNAL(itemSelectionChanged()), this, SLOT(newAcfSelected()));

    // add test aircraft
    connect(ui->bAddTest, SIGNAL(clicked()), this, SLOT(addTestAircraft()));

    // mute / restor sound for alerts
    connect(ui->bMute, SIGNAL(clicked()), this, SLOT(toggleMute()));

    // live indicator
    liveTimer.setSingleShot(true);
    connect(&liveTimer, SIGNAL(timeout()), this, SLOT(updateLiveInfo()));

    // Dump1090 launcher
    connect(ui->bDump1090, SIGNAL(clicked()), this, SLOT(lauchDump1090()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(readDumpOutput()));

    // init
    ui->bAcknowlege->setEnabled(false);
    this->reloadDatabase();
    this->setInfoVisi(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::establishConnection()
{
    _socket.connectToHost(QHostAddress("127.0.0.1"), 30003);
}

void MainWindow::connectionErrorsH(QAbstractSocket::SocketError)
{
    _socket.abort();
    _socket.close();
    connectionTimer.start(2000);
    ui->bDump1090->setEnabled(true);
}

void MainWindow::connectionEstablished()
{
    ui->bDump1090->setEnabled(false);
}


void MainWindow::onReadyRead()
{
    QString line = _socket.readAll();
    QStringList data = line.split(",");
    if(data.size() == 22)
    {
        QString icao = data.at(4);
        if(icao != "" && interestingAcf.contains(icao))
        {
            int i=0;
            while(i<seenAcf.size()) // remove acf not seen for a while
            {
                if(seenAcf[i].getIcao() == icao)
                {
                    seenAcf[i].seen(); // check TODO
                }
                if(seenAcf[i].notSeenForSec() > awayTimeMin*60)
                {
                    seenAcf.removeAt(i);
                }
                i++;
            }
            if(!seenAcf.contains(icao)) // new aircraft
            {
                Aircraft acf = interestingAcf[interestingAcf.indexOfIcao(icao)];
                ui->acfTable->insertRow(0);
                ui->acfTable->setItem(0,0,new QTableWidgetItem(acf.getIcao()));
                ui->acfTable->setItem(0,1,new QTableWidgetItem(acf.getRegistration()));

                seenAcf.append(acf);

                // alarm
                this->triggerAlerts();
                alarmTimer.start(alarmIntervalMS);
                ui->bAcknowlege->setEnabled(true);
            }
            if(data.at(10) != "")
            {
                QString callsign = data.at(10);
                seenAcf[seenAcf.indexOfIcao(icao)].setCallsignLive(callsign);
            }
            if(data.at(11) != "")
            {
                int alti = data.at(11).toInt();
                seenAcf[seenAcf.indexOfIcao(icao)].setAltitude(alti);
            }
            if(data.at(12) != "")
            {
                int spd = data.at(12).toInt();
                seenAcf[seenAcf.indexOfIcao(icao)].setSpeed(spd);
            }
            if(data.at(13) != "")
            {
                int hdg = data.at(13).toInt();
                seenAcf[seenAcf.indexOfIcao(icao)].setHeading(hdg);
            }
            if(data.at(14) != "")
            {
                float lat = data.at(14).toFloat();
                seenAcf[seenAcf.indexOfIcao(icao)].setLatitude(lat);
            }
            if(data.at(15) != "")
            {
                float lon = data.at(15).toFloat();
                seenAcf[seenAcf.indexOfIcao(icao)].setLongitude(lon);
            }
            if(data.at(16) != "")
            {
                int vSpd = data.at(16).toInt();
                seenAcf[seenAcf.indexOfIcao(icao)].setVerticalSpeed(vSpd);
            }
            if(data.at(17) != "")
            {
                int sqk = data.at(17).toInt();
                seenAcf[seenAcf.indexOfIcao(icao)].setSqawk(sqk);
            }
            updateLiveInfo();
        }
        else if(addingTestAcf)
        {
            interestingAcf[interestingAcf.indexOfReg("TEST1")].setIcao(icao);
            addingTestAcf = false;
        }
    }
    // connect again if no message for 1 min
    connectionTimer.start(60000);
}

void MainWindow::aknowlegeAlerts()
{
    for(int i=0;i<seenAcf.size();i++)
    {
        seenAcf[i].setAcknowleged(true);
    }
    alarmTimer.stop();
    ui->bAcknowlege->setEnabled(false);
}

void MainWindow::clearAcfList()
{
    seenAcf.clear();
    while(ui->acfTable->rowCount() > 0)
    {
        ui->acfTable->removeRow(0);
    }
    this->resetInfo();
}

void MainWindow::reloadDatabase()
{
    if (database.open(QIODevice::ReadOnly | QIODevice::Text)) {
        interestingAcf.clear();
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
            Aircraft acf = Aircraft(icao, registration, callsign, location, type, version);
            interestingAcf.append(acf);
        }
        database.close();
    }
}

void MainWindow::triggerAlerts()
{
    bool alarm = false;
    for(int i=0;i<seenAcf.size();i++)
    {
        if(!seenAcf[i].isKnown())
        {
            alarm = true;
        }
    }
    if(alarm)
    {
        if(!muted)
        {
            QSound::play("D:/Cam_Laptop/Documents/Aeronautique/ADSB/resources/bell.wav");
        }
    }
    else
    {
        alarmTimer.stop();
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

void MainWindow::newAcfSelected()
{
    if(!seenAcf.isEmpty())
    {
        int row = ui->acfTable->currentRow();
        QString reg = ui->acfTable->item(row, 1)->text();
        int index = interestingAcf.indexOfReg(reg);
        if(index > -1)
        {
            Aircraft acf = interestingAcf[index]; // DATABASE
            // based on registration because of test aircraft

            ui->immatriculationField->setText(acf.getRegistration());
            ui->typeField->setText(acf.getType());
            ui->versionField->setText(acf.getVersion());
            ui->callsignField->setText(acf.getCallsign());
            ui->baseField->setText(acf.getLocation());
            picture.setPicturePath("D:/Cam_Laptop/Documents/Aeronautique/ADSB/resources/"+acf.getRegistration()+".jpg");
            this->updateLiveInfo();
        }
    }
}

void MainWindow::updateLiveInfo()
{
    if(!seenAcf.isEmpty() && ui->acfTable->currentRow() != -1)
    {
        int row = ui->acfTable->currentRow();
        QString icao = ui->acfTable->item(row, 0)->text();
        int index = seenAcf.indexOfIcao(icao);
        if(index > -1) // found
        {
            Aircraft acf = seenAcf[index]; // SEEN DATABASE

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
                liveTimer.start(max(32000-acf.notSeenForSec()*1000,0));
            #else
                liveTimer.start(std::max(32000-acf.notSeenForSec()*1000,0));
            #endif
            }
        }
    }
    else {
        ui->squawkField->setText("");
        ui->callsignLiveField->setText("");
        ui->altitudeField->setText("");
        ui->vSpeedField->setText("");
        ui->speedField->setText("");
        ui->headingField->setText("");
        ui->latitudeField->setText("");
        ui->longitudeField->setText("");
        ui->radioInLive->setChecked(false);
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

void MainWindow::addTestAircraft()
{
    addingTestAcf = true;
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

void MainWindow::lauchDump1090()
{
    QString dir = "D:/Applications/dump1090-win/";
    QString app = dir+"dump1090.exe";
#ifdef WIN32
    process.setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags &= ~ulong(CREATE_NO_WINDOW);
    });
#endif
    process.setProgram(app);
    process.setArguments({"--net",
                         "--net-ro-size", "500",
                         "--net-ro-rate", "5",
                         "--net-buffer", "5",
                         "--ppm", "41",
                         "--lat", "48.056038",
                         "--lon", "7.075759"});
    process.setWorkingDirectory(dir);
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
