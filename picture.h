#ifndef PICTURE_H
#define PICTURE_H

#include <QLabel>
#include <QSize>
#include <QString>

namespace Ui {
class Picture;
}

class Picture : public QLabel
{
    Q_OBJECT

public:
    Picture(QString resourcesFolderPath, QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent* event);
    void setPicturePath(QString path);
private:
    QString picturePath;
    QString defaultPicturePath;
};

#endif // PICTURE_H
