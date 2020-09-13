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

    void refreshPaths();

    QFile* getDatabase();
    QString getResourcesPath();
    QString getDump1090Path();

public slots:
    void accept() override;
    void reject() override;

    void conditionalShow();

    void setDatabasePath();
    void setResourcesPath();
    void setDump1090Path();

private:
    Ui::Settings *ui;

    QApplication *app;

    QFile databasePath;
    QFile resourcesPath;
    QFile dump1090Path;

    void readSettings();
    void writeSettings();
    bool allPathsValid();

    QString getPathText(QFile *file);
};

#endif // SETTINGS_H
