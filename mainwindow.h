#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include <QTimer>
#include <QList>
#include <QFile>
#include <QProcess>
#include <QMessageBox>

#include "aircraftlist.h"
#include "picture.h"

enum ColumnsType{IcaoClmn, CallsignClmn, AltiClmn};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, QString database = "");
    ~MainWindow();

public slots:
    void establishConnection();
    void connectionErrorsH(QAbstractSocket::SocketError error);
    void connectionEstablished();
    void onReadyRead();
    void aknowlegeAlerts();

    void setInfoVisi(bool visible);
    void resetInfo();

    void redrawAcfList();

    void reloadDatabase();

    void triggerAlerts();

    void toggleInfoVisi();
    void resizeWindow();

    void newAcfSelected();

    void addTestAircraft();

    void updateLiveInfo();
    void updateListInfo(QString icao, ColumnsType column, QString value);
    void removeFromList(QString icao);

    void toggleMute();

    void lauchDump1090();
    void readDumpOutput();

private:
    Ui::MainWindow *ui;
    QTcpSocket  _socket;
    QTimer connectionTimer;

    QFile database;
    QTimer resizeTimer;
    int smallWidth;

    Picture picture;

    AircraftList interestingAcf; // From Database
    AircraftList seenAcf; // From ADSB messages

    QTimer alarmTimer;
    const int alarmIntervalMS = 5000;
    const int awayTimeMin = 5;
    QIcon soundIcon;
    QIcon mutedIcon;
    bool muted = false;

    QTimer liveTimer;

    bool addingTestAcf = false;

    QProcess process;

    QMessageBox msgBox;
};

#endif // MAINWINDOW_H
