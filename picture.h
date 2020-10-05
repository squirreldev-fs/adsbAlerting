#ifndef PICTURE_H
#define PICTURE_H

#include <QLabel>
#include <QSize>
#include <QString>

namespace Ui {
    class Picture;
}

/*!
 * \brief The Picture class is used to handle more easily pictures in the UI, making them resizable.
 */
class Picture : public QLabel
{
    Q_OBJECT

public:
    /*!
     * \brief Picture constructor.
     * \param resourcesFolderPath is the path to the picture to be displayed.
     * \param parent is passed to the QLabel constructor and kept for future additions.
     */
    Picture(QString resourcesFolderPath, QWidget *parent = nullptr);

    /*!
     * \brief resizeEvent is called when the picture is to be resized.
     * \param event passed for signal / slot compatiblity.
     */
    void resizeEvent(QResizeEvent* event);

    /*!
     * \brief setPicturePath to dynamically and easily changed the picture displayed.
     * \param path to the new picture to be displayed.
     */
    void setPicturePath(QString path);
private:
    QString picturePath;
    QString defaultPicturePath;
};

#endif // PICTURE_H
