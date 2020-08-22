#ifndef ADSBFRAMEREADER_H
#define ADSBFRAMEREADER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>

#include "aircraftlist.h"
#include "mainwindow.h"

struct ADSBFrameDescType
{
    int size;
    // indices in frame
    int icao;
    int call;
    int alti;
    int spd;
    int hdg;
    int lat;
    int lon;
    int vspd;
    int sqwk;
};

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

    ADSBFrameDescType adsbFrameDescr = {22, 4, 10, 11, 12, 13, 14, 15, 16, 17};

    const int awayTime = 5; // min, if no frame from a specific acf reveived
    const int reconnectAfter = 1; // min, if no frame received

    AircraftList *interestingAcf; // From database
    AircraftList *seenAcf; // From ADSB messages

    MainWindow *userInterface;
};

#endif // ADSBFRAMEREADER_H
