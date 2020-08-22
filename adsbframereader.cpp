#include "adsbframereader.h"

ADSBFrameReader::ADSBFrameReader(AircraftList *live, AircraftList *database, MainWindow *gui, QObject *parent) : QObject(parent), tcpSocket(this)
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

    //emit tcpSocket.connected();
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
    QString line = tcpSocket.readLine();
    QStringList data = line.split(",");
    if(data.size() == 22)
    {
        QString icao = data.at(4);
        if(icao != "")
        {
            int i=0;
            while(i<seenAcf->size()) // remove acf not seen for a while
            {
                if(seenAcf->at(i).getIcao() == icao)
                {
                    (*seenAcf)[i].seen();
                    i++;
                }
                else if(seenAcf->at(i).notSeenForSec() > awayTimeMin*60 && interestingAcf->indexOfIcao(icao) < 0)
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
                    userInterface->raiseAlert();
                }
            }

            if(data.at(10) != "")
            {
                QString callsign = data.at(10);
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setCallsignLive(callsign);
                userInterface->updateListInfo(icao, CallsignClmn, callsign);
            }
            if(data.at(11) != "")
            {
                int alti = data.at(11).toInt();
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setAltitude(alti);
                userInterface->updateListInfo(icao, AltiClmn, QString::number(alti));
            }
            if(data.at(12) != "")
            {
                int spd = data.at(12).toInt();
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setSpeed(spd);
            }
            if(data.at(13) != "")
            {
                int hdg = data.at(13).toInt();
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setHeading(hdg);
            }
            if(data.at(14) != "")
            {
                float lat = data.at(14).toFloat();
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setLatitude(lat);
            }
            if(data.at(15) != "")
            {
                float lon = data.at(15).toFloat();
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setLongitude(lon);
            }
            if(data.at(16) != "")
            {
                int vSpd = data.at(16).toInt();
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setVerticalSpeed(vSpd);
            }
            if(data.at(17) != "")
            {
                int sqk = data.at(17).toInt();
                (*seenAcf)[seenAcf->indexOfIcao(icao)].setSqawk(sqk);
            }
            userInterface->updateLiveInfo();
        }
    }
    // connect again if no message for 1 min
    connectionTimer.start(60000);
}
