#ifndef COBJECTLIBRARYTREE_H
#define COBJECTLIBRARYTREE_H

#include <QTreeWidget>
#include <QFileSystemWatcher>
#include <QVector>
#include <QListWidget>
#include "CActiveZoneItem.h"

class CSetLibraryTree : public QTreeWidget
{
    Q_OBJECT

public:

    explicit CSetLibraryTree(QWidget *parent = 0);

    void setRoot(QString const & path);
    void setListView( QListWidget * lv );

    // return selected absolute path
    QString currentDir() const;

private:

    QString         m_pathRoot;             // root directory
    QListWidget *   m_ListView = nullptr;   // listview for current directory images
    bool            m_ShowImages = false;   // show images in directory list

    QTreeWidgetItem * m_itemNew = nullptr;  // used while new folder creation

private:

    void insertDir(QTreeWidgetItem * parent, QString const & dir);
    void insertObj(QTreeWidgetItem * parent, CActiveZoneItem const & obj);

    QFileSystemWatcher  m_watcher;

    void on_newDirectory(QTreeWidgetItem * iDir, QString const & dirPath);
    void on_newObject(QTreeWidgetItem * iDir, CActiveZoneItem const & obj);
    void on_delDirectory(QTreeWidgetItem * iDir, QString const & dirName);

    QTreeWidgetItem* getItemByName(QTreeWidgetItem* parent, QString const & name);
    QTreeWidgetItem* getItemByPath(QString const & path);

    static QVector<CActiveZoneItem> getDirObjects(QString const & path);

    QString getItemPath(QTreeWidgetItem* item) const;

    bool deleteDir( QString const & pathDir );

    void prepareMenu( const QPoint & pos );
    void prepareListViewMenu( const QPoint & pos );

    void updateListView();

protected:

    QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const override;
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

signals:

public slots:

    void deleteFolder();
    void createFolder();

protected slots:

    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
    void on_directoryChanged(const QString &path);
};

#endif // COBJECTLIBRARYTREE_H
