#include <QString>
#include <QPixmap>

#include "picture.h"

Picture::Picture(QString resourcesFolderPath, QWidget *parent) : QLabel(parent)
{
    this->defaultPicturePath = resourcesFolderPath+"noPicture.jpg";
    this->setMinimumSize(300,200);
    picturePath = defaultPicturePath;
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
        picturePath = defaultPicturePath;
        pic = QPixmap(picturePath);
    }
    pic=pic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pic);
}
