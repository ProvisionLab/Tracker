#include "CSetLibraryTree.h"
#include <QDir>
#include <QDebug>
#include <QMimeData>
#include <QAction>
#include <QMenu>
#include <QListWidgetItem>

CSetLibraryTree::CSetLibraryTree(QWidget *parent)
    : QTreeWidget(parent)
{
    setHeaderHidden(true);

    setAcceptDrops(false);
    setDragEnabled(true);
    setDragDropMode(DragDropMode::DragOnly);
    setDefaultDropAction(Qt::LinkAction);

    QObject::connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(on_directoryChanged(QString)));

    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, &CSetLibraryTree::customContextMenuRequested,this, &CSetLibraryTree::prepareMenu);
}

void CSetLibraryTree::setRoot(QString const & path)
{
    QDir root(path);
    if (!root.exists())
    {
        qDebug() << __FUNCTION__ << ": invalid directory name: " << path;
        return;
    }

    m_watcher.blockSignals(true);
    m_watcher.removePaths(m_watcher.directories());

    clear();

    m_pathRoot = root.absolutePath();

    qDebug() << "curdir: " << QDir::currentPath();

    insertDir(invisibleRootItem(), path);

    this->expandAll();

    for ( auto & d : m_watcher.directories())
        qDebug() << "wdir: " << d;

    m_watcher.blockSignals(false);
}

void CSetLibraryTree::setListView( QListWidget * lv )
{
    m_ListView = lv;

    if ( m_ListView )
    {
        m_ListView->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(m_ListView, &QListWidget::customContextMenuRequested, this, &CSetLibraryTree::prepareListViewMenu);
    }

    updateListView();
}

QVector<CActiveZoneItem> CSetLibraryTree::getDirObjects(QString const & path)
{
    QDir dir(path);
    if ( !dir.exists() )
        return {};

    QVector<CActiveZoneItem> l;

    for ( auto & fi : dir.entryInfoList(QDir::Files) )
    {
        if ( fi.isFile())
        {
            CActiveZoneItem azi(fi);

            if ( !azi.exists() )
                continue;

            l.push_back(azi);
        }
    }

    return l;
}

QString CSetLibraryTree::getItemPath(QTreeWidgetItem* item) const
{
    if ( !item || !item->parent() || item->parent() == this->invisibleRootItem() )
        return m_pathRoot;

    return getItemPath(item->parent()) + "/" + item->text(0);
}

QString CSetLibraryTree::currentDir() const
{
    auto * item = currentItem();
    auto path = getItemPath(item);
    return path;
}

void CSetLibraryTree::insertDir(QTreeWidgetItem * parent, QString const & dirPath)
{
    QDir dir(dirPath);

//    qDebug() << "dir: " << dir.dirName() << ", " << dir.absolutePath();

    auto iDir = new QTreeWidgetItem();
    //iDir->setIcon(0, QIcon(":/res/i_fldr2.png"));
    iDir->setText(0, dir.dirName());
    parent->addChild(iDir);

    for ( auto & fi : dir.entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot) )
    {
        if ( fi.isDir() )
        {
            insertDir(iDir, fi.absoluteFilePath());
        }
    }

    if ( m_ShowImages )
    {
        auto objs = getDirObjects(dirPath);
        for ( auto & obj : objs)
        {
            insertObj(iDir, obj);
        }
    }

    m_watcher.addPath(dir.absolutePath());
}

void CSetLibraryTree::insertObj(QTreeWidgetItem * parent, CActiveZoneItem const & obj)
{
//    qDebug() << __FUNCTION__ << ": " << obj.path << ", tag: " << obj.tag;

    auto item = new QTreeWidgetItem();
    item->setText(0, obj.tag);

    QString pathIcon = obj.get_path_etalon();

    item->setIcon(0, obj.getImageQt({24,24}));

    parent->addChild(item);
}

void CSetLibraryTree::on_directoryChanged(const QString &path)
{
    qDebug() << __FUNCTION__ << ": " << path;

    // get directory item

    auto * iDir = getItemByPath(path);
    if (!iDir)
    {
        qDebug() << __FUNCTION__ << ": invalid path: ignore";
        return;
    }

    // get item list (dirs,files)

    QSet<QString> old_dirs;
    for (int i = 0; i < iDir->childCount(); ++i)
    {
        auto sFile = iDir->child(i)->text(0);
        old_dirs.insert(sFile);
    }

    QDir dir2(path);
    QSet<QString>   cur_dirs;
    for (auto & e : dir2.entryList(QDir::AllDirs|QDir::NoDotAndDotDot))
    {
        cur_dirs.insert(e);
    }

    QSet<QString>   cur_objs;
    if ( m_ShowImages )
    {
        for (auto & e : getDirObjects(path))
        {
            cur_objs.insert(e.tag);
        }
    }

    auto delFiles = old_dirs - cur_dirs - cur_objs;

    for ( auto & s : delFiles )
    {
        qDebug() << "\t-" << s << "/";
        on_delDirectory(iDir, s);
    }

    // new dirs
    for ( auto & s : cur_dirs - old_dirs )
    {
        QString newPath(path + "/" + s);
        if ( QFileInfo(newPath).isDir())
        {
            qDebug() << "\t+" << s << "/";
            on_newDirectory(iDir, newPath);
        }
    }

    // new objs
    if ( m_ShowImages )
    {
        for ( auto & tag : cur_objs - old_dirs )
        {
            CActiveZoneItem newObj(path, tag);

            if ( newObj.exists() )
            {
                qDebug() << "\t+" << tag;
                on_newObject(iDir, newObj);
            }
        }
    }

    if (this->currentItem() == iDir)
        updateListView();
}

QTreeWidgetItem* CSetLibraryTree::getItemByName(QTreeWidgetItem* parent, QString const & name)
{
    for (int i = 0; i < parent->childCount(); ++i)
    {
        auto * item = parent->child(i);
        if ( item->text(0) == name)
            return item;
    }

    return nullptr;
}

QTreeWidgetItem* CSetLibraryTree::getItemByPath(QString const & path)
{
    if ( path.leftRef(m_pathRoot.size()) != m_pathRoot )
        return nullptr;

    QString sDir = path.right(path.size()-m_pathRoot.size());

    QStringList listDir = sDir.split('/');

    // remove starting '/'
    if ( !listDir.isEmpty() && listDir.first().isEmpty())
        listDir.removeFirst();

    auto * item = this->invisibleRootItem()->child(0);
    if (!item)
        return nullptr;

    for (auto s: listDir)
    {
        item = getItemByName(item, s);
        if ( !item )
            return nullptr;
    }

    return item;
}

void CSetLibraryTree::on_newDirectory(QTreeWidgetItem * iDir, QString const & dirPath)
{
    qDebug() << __FUNCTION__ << ": " << dirPath;

    insertDir(iDir, dirPath);

    this->expandItem(iDir);
}

void CSetLibraryTree::on_newObject(QTreeWidgetItem * iDir, CActiveZoneItem const & obj)
{
    qDebug() << __FUNCTION__ << ": " << obj.path << ", " << obj.tag;

    insertObj(iDir, obj);

    this->expandItem(iDir);
}

void CSetLibraryTree::on_delDirectory(QTreeWidgetItem * iDir, QString const & dirName)
{
    auto * item = getItemByName(iDir, dirName);
    if ( item )
    {
        iDir->removeChild(item);
    }
}

bool CSetLibraryTree::deleteDir( QString const & pathDir )
{
    qDebug() << __FUNCTION__ << ": " << pathDir;

    QDir dir(pathDir);
    if ( !dir.exists() )
    {
        qDebug() << "\tnot a dir: " << pathDir;
        return false;
    }

    // delete content
    for ( auto & f : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files))
    {
        qDebug() << "\t" << f.absoluteFilePath();

        if ( f.isDir() )
        {
            if ( !deleteDir(f.absoluteFilePath()) )
                return false;
        } else
        {
            if ( !QFile::remove(f.absoluteFilePath()) )
                return false;
        }
    }

    m_watcher.removePath(pathDir);

    if ( !QDir().rmdir(pathDir) )
    {
        qDebug() << "rmdir " << pathDir << " failed";
        m_watcher.addPath(pathDir);
        return false;
    }

    return true;
}

void CSetLibraryTree::deleteFolder()
{
    qDebug() << __FUNCTION__;

    auto * item = this->currentItem();

    if (!item || !item->parent())
    {
        qDebug() << __FUNCTION__ << ": root";
        return;
    }

    auto path = this->getItemPath(item);

    if ( path == m_pathRoot )
    {
        qDebug() << __FUNCTION__ << ": root";
        return;
    }

    if ( !deleteDir(path) )
    {
        qDebug() << __FUNCTION__ << ": failed, " << path;
    }
}

QMimeData *CSetLibraryTree::mimeData(const QList<QTreeWidgetItem *> items) const
{
    qDebug() << __FUNCTION__ << ": count: " << items.size();

    if ( items.isEmpty() )
        return nullptr;

    auto * item = items.first();

    QString txt;

    QString path = getItemPath(item);
    if (QFileInfo(path).isDir())
    {
        auto objs = getDirObjects(path);
        if (objs.isEmpty())
            return nullptr;

        for (auto & obj : objs)
        {
            txt += obj.path + "/" + obj.tag + "|";
        }

        txt.remove(txt.size()-1,1);
    } else
    {
        txt = path;
    }

    auto * mime = new QMimeData();

    mime->setData("application/x-activezoneitem", txt.toUtf8());

    return mime;
}

void CSetLibraryTree::prepareMenu( const QPoint & pos )
{
    auto * item = this->itemAt(pos);
    if ( !item )
        return;

    qDebug() << __FUNCTION__ << item->text(0);

    auto pathItem = getItemPath(item);

    if ( QFileInfo(pathItem).isDir() )
    {
        QMenu menu(this);

        QAction *actNewFolder = new QAction(tr("&New Folder"), this);
        connect(actNewFolder, SIGNAL(triggered()), this, SLOT(createFolder()));

        menu.addAction(actNewFolder);

        if ( item && item->parent() )
        {
            QAction *actDelFolder = new QAction(tr("&Del Folder"), this);
            connect(actDelFolder, SIGNAL(triggered()), this, SLOT(deleteFolder()));

            menu.addAction(actDelFolder);
        }

        QPoint pt(pos);
        menu.exec( this->mapToGlobal(pos) );
    } else
    {
        return;
    }
}

void CSetLibraryTree::prepareListViewMenu( const QPoint & pos )
{
    qDebug() << __FUNCTION__;

    auto * item = m_ListView->itemAt(pos);
    if ( !item )
        return;

    QMenu menu(this);

    QAction *actDelObj = new QAction(tr("&Delete"), this);
    connect(actDelObj, &QAction::triggered,
    [this, item]()
    {
        CActiveZoneItem obj(currentDir(), item->text());
        if (obj.exists())
        {
            obj.deleteFiles();
        }
    });

    menu.addAction(actDelObj);

    QPoint pt(pos);
    menu.exec( m_ListView->mapToGlobal(pos) );
}

void CSetLibraryTree::createFolder()
{
    auto * item = this->currentItem();
    if ( !item )
        return;

    qDebug() << __FUNCTION__ << ": " << item->text(0);

    QTreeWidgetItem * newItem = new QTreeWidgetItem();
    item->addChild( newItem );
    item->setExpanded(true);

    newItem->setText(0, tr("New Folder"));
    newItem->setFlags(item->flags() | Qt::ItemIsEditable);

    this->editItem(newItem, 0);

    QWidget * editor = itemWidget(newItem, 0);
    if ( editor )
    {
        auto h = visualItemRect(item).height();
        editor->setMinimumHeight(h+4);
    }

    m_itemNew = newItem;
}

void CSetLibraryTree::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
{
    Q_UNUSED(editor);

    if ( !m_itemNew )
        return;

    auto * item = m_itemNew;
    m_itemNew = nullptr;

    QString path = getItemPath(item->parent());
    QString text = item->text(0);

    item->parent()->removeChild(item);

    if ( hint == QAbstractItemDelegate::SubmitModelCache )
    {
        qDebug() << __FUNCTION__ << " new folder submit: " << text;

        if ( !text.isEmpty() )
        {
            QDir dir(path);

            QFileInfo f(dir, text);

            if ( !f.exists() )
            {
                dir.mkdir(text);
            }
        }

    } else if ( hint == QAbstractItemDelegate::RevertModelCache )
    {
        qDebug() << __FUNCTION__ << " new folder cancel";
    }
}

void CSetLibraryTree::updateListView()
{
    qDebug() << __FUNCTION__;

    if ( !m_ListView )
        return;

    m_ListView->clear();

    QSize iconSize(64,64);

    m_ListView->setIconSize(iconSize);

    auto * item = this->currentItem();
    if ( !item )
        return;

    auto pathDir = getItemPath(item);

    QDir dir(pathDir);
    if ( !dir.exists() )
        return;

    auto objs = getDirObjects(pathDir);

    for ( auto & obj : objs )
    {
        auto bgColor = m_ListView->palette().color(m_ListView->backgroundRole());
        auto img(obj.getImageQt(iconSize, true, bgColor));

        new QListWidgetItem(QIcon(img), obj.tag, m_ListView );
    }
}

void CSetLibraryTree::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTreeWidget::currentChanged(current, previous);

    updateListView();
}
