#include "CActiveZone.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QLayout>
#include <QPushButton>
#include <QMenu>
#include "CActiveZoneItem.h"

CActiveZone::CActiveZone(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);

    m_Row = new QHBoxLayout(this);
    m_Row->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_Row->setSpacing(3);
    m_Row->setContentsMargins(6,6,6,6);

    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, &CActiveZone::customContextMenuRequested,this, &CActiveZone::prepareMenu);
}

void CActiveZone::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << __FUNCTION__ << "mime: " << event->mimeData()->formats();

    if (event->mimeData()->hasFormat("application/x-activezoneitem"))
    {
        event->setDropAction(Qt::LinkAction);

        event->acceptProposedAction();
    }
}

void CActiveZone::dropEvent(QDropEvent *event)
{
    qDebug() << __FUNCTION__ << "mime: " << event->mimeData()->formats();

    auto data = event->mimeData()->data("application/x-activezoneitem");

    auto txt = QString::fromUtf8(data);

    auto objs = txt.split('|');

    if (objs.isEmpty())
        return;

    QVBoxLayout* col = new QVBoxLayout();

    for (auto & obj : objs )
    {
        CActiveZoneItem azi;

        azi.tag = obj.section('/', -1);
        azi.path = azi.tag.size() < obj.size() ? obj.left(obj.size()-azi.tag.size()-1) : "";

        QString pathIcon = azi.get_path_etalon();

        qDebug() << "\t" << obj << ", " << pathIcon;

        CActiveZonePicture * ic = new CActiveZonePicture(this, azi, m_iconSize);
        ic->setFocusPolicy(Qt::NoFocus);
        col->addWidget(ic);
        ic->setColumnId(col);

        connect(ic, SIGNAL(pictureSelected(CActiveZonePicture*)), this, SLOT(pictureSelected(CActiveZonePicture*)));
    }

    col->addItem(new QSpacerItem(1,1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    auto pos = event->pos();

    int index = 0;
    if ( m_Row->count() > 0 )
    for (; index < m_Row->count()-1; ++index)
    {
        auto * item = m_Row->itemAt(index);
        if ( !item )
        {
            break;
        }

        auto g = item->geometry();
        if ( pos.x() < g.center().x() )
            break;
    }

    m_Row->insertLayout(index, col);

    event->acceptProposedAction();
}

void CActiveZone::setIconSize(QSize sz)
{
    m_iconSize = sz;
}

void CActiveZone::pictureSelected(CActiveZonePicture * selpic)
{
    // reset sel state on pics at other cols

    for (auto * pic : this->findChildren<CActiveZonePicture*>())
    {
        if ( pic->columnId() != selpic->columnId() && pic->isChecked() )
        {
            pic->setChecked(false);
        }
    }
}

QList<CActiveZoneItem>  CActiveZone::getSelectedItems() const
{
    QList<CActiveZoneItem> l;

    for (auto * p : this->findChildren<CActiveZonePicture*>())
    {
        if ( p->isChecked() )
        {
            l.push_back(p->m_obj);
        }
    }

    return l;
}

QList<CActiveZoneItem>  CActiveZone::getNotSelectedItems() const
{
    QList<CActiveZoneItem> l;

    QObject* selectesColumn = 0;
    for (auto * p : this->findChildren<CActiveZonePicture*>())
    {
        if ( p->isChecked() )
        {
            selectesColumn = p->columnId();
            break;
        }
    }

    for (auto * p : this->findChildren<CActiveZonePicture*>())
    {
        if ( !p->isChecked() && p->columnId() == selectesColumn )
        {
            l.push_back(p->m_obj);
        }
    }

    return l;
}


void CActiveZone::prepareMenu( const QPoint & pos )
{
    auto * pic = dynamic_cast<CActiveZonePicture*>(this->childAt(pos.x(), pos.y()));

    if ( !pic )
        return;

    auto * col = pic->columnId();

    QMenu menu(this);

    QAction *actDelCol = new QAction(tr("&Remove Column"), this);
    connect(actDelCol, &QAction::triggered,
        [this, col]() {
            qDebug() << "remove column";
            this->removeColumn(col);
        }
    );

    menu.addAction(actDelCol);

    QPoint pt(pos);
    menu.exec( this->mapToGlobal(pos) );
}

void CActiveZone::removeColumn( QObject * columnId )
{
    for (auto * pic : this->findChildren<CActiveZonePicture*>())
    {
        if ( pic->columnId() == columnId )
        {
            pic->setParent(nullptr);
        }
    }

    auto * col = dynamic_cast<QVBoxLayout*>(columnId);
    if ( col )
    {
        col->setParent(nullptr);
    }
}
