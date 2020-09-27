#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFile>
#include <QApplication>
#include <QString>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QApplication *application, QWidget *parent = nullptr);
    ~Settings();

    void refreshDisplay();

    int getMinutesBeforeRemoved();

    QFile* getDatabase();
    QString getResourcesPath();
    QString getDump1090Path();

    int getPPM();
    int getNetRoSize();
    int getNetRoRate();
    int getNetBuffer();

public slots:
    void accept() override;
    void reject() override;

    void conditionalShow();

    void setMinutesBeforeRemoved(int value);

    void setDatabasePath();
    void setResourcesPath();
    void setDump1090Path();

    void setPPMShift(int value);
    void setNetRoSize(int value);
    void setNetRoRate(int value);
    void setNetBuffer(int value);

private:
    Ui::Settings *ui;

    QApplication *app;

    int minutesBeforeRemoved = 5;

    QFile databasePath;
    QFile resourcesPath;
    QFile dump1090Path;

    void readSettings();
    void writeSettings();
    bool allPathsValid();

    QString getPathText(QFile *file);

    int ppmShift = 0;
    int netRoSize = 500;
    int netRoRate = 5;
    int netBuffer = 5;
};

#endif // SETTINGS_H
