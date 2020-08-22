#ifndef ADSBFRAMEREADER_H
#define ADSBFRAMEREADER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>

#include "aircraftlist.h"
#include "mainwindow.h"

class ADSBFrameReader : public QObject
{
    Q_OBJECT

public:
    ADSBFrameReader(AircraftList *live, AircraftList *database, MainWindow *gui, QObject *parent = nullptr);

public slots:
    void establishConnection();
    void connectionErrorsH(QAbstractSocket::SocketError error);
    void connectionEstablished();
    void onReadyRead();

private:
    QTcpSocket tcpSocket;
    QTimer connectionTimer;

    QHostAddress adsbServerIP = QHostAddress("127.0.0.1");
    int adsbServerPort = 30003;

    const int awayTimeMin = 5;

    AircraftList *interestingAcf; // From database
    AircraftList *seenAcf; // From ADSB messages

    MainWindow *userInterface;
};

#endif // ADSBFRAMEREADER_H
