#ifndef CACTIVEZONE_H
#define CACTIVEZONE_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include "CActiveZoneItem.h"
#include <QDebug>

class CActiveZonePicture : public QPushButton
{
    Q_OBJECT

public:

    explicit CActiveZonePicture(QWidget *parent, CActiveZoneItem const & obj, QSize iconSize)
        : QPushButton(parent)
        , m_obj(obj)
    {
        setIcon(obj.getImageCV(iconSize));
        setIconSize(iconSize);

        setFlat(true);
        setCheckable(true);
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

        QSize bnSize(iconSize.width() + 6, iconSize.height() + 6);
        setMinimumSize(bnSize);
        setMaximumSize(bnSize);
    }

    QObject * columnId() const
    {
        return m_col;
    }

    void setColumnId( QObject * colId )
    {
        m_col = colId;
    }

public:

    CActiveZoneItem     m_obj;

private:

    QObject *              m_col = nullptr; // used as id

protected:

    void checkStateSet() override
    {
        QPushButton::checkStateSet();

        if ( isChecked() )
            emit pictureSelected(this);
    }

    void nextCheckState() override
    {
        QPushButton::nextCheckState();

        if ( isChecked() )
            emit pictureSelected(this);
    }

signals:

    void pictureSelected(CActiveZonePicture * pic);
};



class CActiveZone : public QWidget
{
    Q_OBJECT

public:
    explicit CActiveZone(QWidget *parent = 0);

    QSize iconSize() const
    {
        return m_iconSize;
    }

    void setIconSize(QSize sz);


    QList<CActiveZoneItem>  getSelectedItems() const;
    QList<CActiveZoneItem>  getNotSelectedItems() const;

protected:

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:

    QHBoxLayout*    m_Row = nullptr;
    QSize           m_iconSize = {64, 64};

    void prepareMenu( const QPoint & pos );
    void removeColumn( QObject * columnId );

signals:

public slots:

protected slots:

    void pictureSelected(CActiveZonePicture * pic);
};

#endif // CACTIVEZONE_H
