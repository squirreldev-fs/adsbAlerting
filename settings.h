#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFile>
#include <QApplication>
#include <QString>

namespace Ui {
class Settings;
}

/*!
 * \brief The Settings class is both a container for settings and a window to handle them.
 */
class Settings : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Settings constructor.
     * \param application is used to be able to close the application if needed.
     * \param parent is passed to the QDialog constructor and is kept for future use.
     */
    explicit Settings(QApplication *application, QWidget *parent = nullptr);
    ~Settings();

    /*!
     * \brief refreshDisplay updates the UI according to the stored data.
     */
    void refreshDisplay();

    // accessors
    int getMinutesBeforeRemoved();

    QFile* getDatabase();
    QString getResourcesPath();
    QString getDump1090Path();

    int getPPM();
    int getNetRoSize();
    int getNetRoRate();
    int getNetBuffer();

public slots:
    /*!
     * \brief accept hides the settings window if all paths are defined (database, resources and dump1090 application). Displays an error message otherwise.
     */
    void accept() override;

    /*!
     * \brief reject same behavior as accept(), but closes the application if the paths are not all defined.
     */
    void reject() override;

    /*!
     * \brief conditionalShow shows the window only if required parameters are not defined (database, resources folder and dump1090 are defined).
     */
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
