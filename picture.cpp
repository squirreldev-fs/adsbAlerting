#include <QString>
#include <QPixmap>

#include "picture.h"

Picture::Picture(QWidget *parent) : QLabel(parent)
{
    this->setMinimumSize(300,200);
    picturePath = "D:/Cam_Laptop/Documents/Aeronautique/ADSB/resources/noPicture.jpg";
    QPixmap pic = QPixmap(picturePath);
    pic=pic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pic);
}

void Picture::resizeEvent(QResizeEvent* event)
{
    QLabel::resizeEvent(event);
    if(this->pixmap())
    {
        QPixmap pic = QPixmap(picturePath);
        pic=pic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->setPixmap(pic);
    }
}

void Picture::setPicturePath(QString path)
{
    picturePath=path;
    QPixmap pic = QPixmap(picturePath);
    if(pic.isNull())
    {
        picturePath = "D:/Cam_Laptop/Documents/Aeronautique/ADSB/resources/noPicture.jpg";
        pic = QPixmap(picturePath);
    }
    pic=pic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pic);
}
