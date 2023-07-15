#include "adsbframereader.h"

ADSBFrameReader::ADSBFrameReader(AircraftList *live, AircraftList *database, MainWindow *gui, Settings *settings, QObject *parent) : QObject(parent), tcpSocket(this)
{
    interestingAcf = database;
    seenAcf = live;
    userInterface = gui;

    // connection to dump 1090
    connectionTimer.setSingleShot(true);

    establishConnection();

    connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionErrorsH(QAbstractSocket::SocketError)));
    connect(&connectionTimer, SIGNAL(timeout()), this, SLOT(establishConnection()));
    connect(&tcpSocket, SIGNAL(connected()), this, SLOT(connectionEstablished()));

    this->settings = settings;
}

void ADSBFrameReader::establishConnection()
{
    tcpSocket.connectToHost(adsbServerIP, adsbServerPort);
}

void ADSBFrameReader::connectionErrorsH(QAbstractSocket::SocketError error)
{
    (void) error;

    tcpSocket.abort();
    tcpSocket.close();
    connectionTimer.start(2000);

    userInterface->connectionFailed();
}

void ADSBFrameReader::connectionEstablished()
{
    userInterface->connectionSucceeded();
}

void ADSBFrameReader::onReadyRead()
{
    while(tcpSocket.canReadLine())
    {
        QString line = tcpSocket.readLine();
        QStringList data = line.split(",");
        if(data.size() == adsbFrameDescr.size)
        {
            QString icao = data.at(adsbFrameDescr.icao);
            if(icao != "" && icao != "000000")
            {
                int i=0;
                while(i<seenAcf->size()) // remove acf not seen for a while
                {
                    int awayTime = settings->getMinutesBeforeRemoved()*60;
                    if(seenAcf->at(i).getIcao() == icao)
                    {
                        (*seenAcf)[i].seen();
                        i++;
                    }
                    else if(seenAcf->at(i).notSeenForSec() > awayTime && interestingAcf->indexOfIcao(icao) < 0)
                    {
                        seenAcf->removeAt(i);
                        userInterface->redrawAcfList();
                    }
                    else
                    {
                        i++;
                    }
                }

                if(!seenAcf->contains(icao)) // new aircraft
                {
                    AircraftADSB acf = AircraftADSB(icao);
                    if(interestingAcf->contains(icao)) // get info from database
                    {
                        acf = interestingAcf->at(interestingAcf->indexOfIcao(icao));
                    }

                    seenAcf->append(acf);
                    userInterface->updateListInfo(icao, IcaoClmn, icao);

                    if(interestingAcf->contains(icao)) // alarm
                    {
                        seenAcf->back().setAlertStatus(AlertRaised);
                        userInterface->raiseAlert(icao);
                    }
                }

                AircraftADSB *currentAircraft = &(*seenAcf)[seenAcf->indexOfIcao(icao)];
                if(data.at(adsbFrameDescr.call) != "")
                {
                    QString callsign = data.at(adsbFrameDescr.call);
                    currentAircraft->setCallsignLive(callsign);
                    userInterface->updateListInfo(icao, CallsignClmn, callsign);
                }
                if(data.at(adsbFrameDescr.alti) != "")
                {
                    int alti = data.at(adsbFrameDescr.alti).toInt();
                    currentAircraft->setAltitude(alti);
                    userInterface->updateListInfo(icao, AltiClmn, QString::number(alti));
                }
                if(data.at(adsbFrameDescr.spd) != "")
                {
                    int spd = data.at(adsbFrameDescr.spd).toInt();
                    currentAircraft->setSpeed(spd);
                }
                if(data.at(adsbFrameDescr.hdg) != "")
                {
                    int hdg = data.at(adsbFrameDescr.hdg).toInt();
                    currentAircraft->setHeading(hdg);
                }
                if(data.at(adsbFrameDescr.lat) != "")
                {
                    float lat = data.at(adsbFrameDescr.lat).toFloat();
                    currentAircraft->setLatitude(lat);
                }
                if(data.at(adsbFrameDescr.lon) != "")
                {
                    float lon = data.at(adsbFrameDescr.lon).toFloat();
                    currentAircraft->setLongitude(lon);
                }
                if(data.at(adsbFrameDescr.vspd) != "")
                {
                    int vSpd = data.at(adsbFrameDescr.vspd).toInt();
                    currentAircraft->setVerticalSpeed(vSpd);
                }
                if(data.at(adsbFrameDescr.sqwk) != "")
                {
                    int sqwk = data.at(adsbFrameDescr.sqwk).toInt();
                    currentAircraft->setSqawk(sqwk);
                }
                userInterface->updateLiveInfo();
            }
        }
    }
    // connect again if no message for N min
    connectionTimer.start(reconnectAfter*60000);
}
