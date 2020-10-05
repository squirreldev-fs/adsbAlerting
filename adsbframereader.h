#ifndef ADSBFRAMEREADER_H
#define ADSBFRAMEREADER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>

#include "aircraftlist.h"
#include "mainwindow.h"
#include "settings.h"

/*!
 * \brief The ADSBFrameDescType struct is used to register indices of information in the transmitted frames.
 */
struct ADSBFrameDescType
{
    /*!
     * \brief size of the frame (total number of elements).
     */
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

/*!
 * \brief The ADSBFrameReader class reads network frames sent by dump 1090 and make their data available to the rest of the application.
 */
class ADSBFrameReader : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief ADSBFrameReader constructor
     * \param live is the list of aircraft from which an emitted frame was recently received.
     * \param database is the list of "interesting" aircraft, for which an alert is triggered when detected.
     * \param gui is the main window of this application. Used to transmit some statuses.
     * \param settings contains settings of this application (here only the time to declare an aircraft "live" is used).
     * \param parent is required to inheritate from QObject. Kept available for future use.
     */
    ADSBFrameReader(AircraftList *live, AircraftList *database, MainWindow *gui, Settings *settings, QObject *parent = nullptr);

public slots:
    /*!
     * \brief establishConnection creates tcp connection with dump1090's server.
     */
    void establishConnection();
    /*!
     * \brief connectionErrorsH handles connection error and launches the timer to retry connection later (2s).
     * \param error is not used at the current time. Required for signal / slot compatibility.
     */
    void connectionErrorsH(QAbstractSocket::SocketError error);
    /*!
     * \brief connectionEstablished is used to inform the main window that the connection succeeded.
     */
    void connectionEstablished();
    /*!
     * \brief onReadyRead reads the incoming frames sent by dump1090.
     * This is a major function of this application.
     * It is reponsible for decoding the frames and adding their information into shared data structures.
     * It also checks if no frame from a specific aircraft has been received for a certain amount of time, to remove it from the list of "live" aircraft.
     * It also triggers an alert if the received frame was emitted by an "interesting" aircraft.
     */
    void onReadyRead();

private:
    QTcpSocket tcpSocket;
    QTimer connectionTimer;

    QHostAddress adsbServerIP = QHostAddress("127.0.0.1");
    int adsbServerPort = 30003;

    ADSBFrameDescType adsbFrameDescr = {22, 4, 10, 11, 12, 13, 14, 15, 16, 17};

    const int reconnectAfter = 1; // min, if no frame received

    AircraftList *interestingAcf; // From database
    AircraftList *seenAcf; // From ADSB messages

    MainWindow *userInterface;
    Settings *settings;
};

#endif // ADSBFRAMEREADER_H
