#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include <QList>
#include <QFile>
#include <QProcess>
#include <QMessageBox>

#include "aircraftlist.h"
#include "picture.h"
#include "settings.h"

enum ColumnsType{IcaoClmn, CallsignClmn, AltiClmn};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(AircraftList *live, AircraftList *database, Settings *settings, QWidget *parent = nullptr);
    ~MainWindow();

    void setInfoVisi(bool visible);
    void resetInfo();

    void connectionFailed();
    void connectionSucceeded();

    void raiseAlert();

public slots:
    void aknowlegeAlerts();
    void triggerAlerts();
    void toggleMute();

    void filterAcfList();
    void redrawAcfList();
    void newAcfSelected();
    void updateListInfo(QString icao, ColumnsType column, QString value);

    void reloadDatabase();
    void addToDatabase();

    void toggleInfoVisi();
    void resizeWindow();
    void updateLiveInfo();

    void lauchDump1090();
    void readDumpOutput();

    void LaunchWebForInfo();

private:
    Ui::MainWindow *ui;

    Settings *settings;

    QTimer resizeTimer;
    int smallWidth;

    Picture picture;

    AircraftList *interestingAcf; // From database
    AircraftList *seenAcf; // From ADSB messages

    bool showingOnlyRecognized = false;

    QTimer alarmTimer;
    const int alarmIntervalMS = 5000;
    QIcon soundIcon;
    QIcon mutedIcon;
    bool muted = false;

    QTimer liveTimer;

    QProcess process;

    QMessageBox msgBox;
};

#endif // MAINWINDOW_H
