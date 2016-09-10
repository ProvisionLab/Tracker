#ifndef CACTIVEZONEITEM_H
#define CACTIVEZONEITEM_H

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QPixmap>

class CActiveZoneItem
{
public:

    QString path;   // path to directory
    QString tag;    // "YY_MM_DD_HH_mm_SS"

    CActiveZoneItem()
    {
    }

    CActiveZoneItem(QString const & path, QString const & tag)
        : path(path)
        , tag(tag)
    {
    }

    CActiveZoneItem(QFileInfo const & f_etalon);

    bool isEmpty() const { return path.isEmpty() || tag.isEmpty(); }

    QString get_path_etalon() const   { return get_FilePath("__etalon.png"); }
    QString get_path_left() const   { return get_FilePath("__left.png"); }
    QString get_path_right() const   { return get_FilePath("__right.png"); }
    QString get_path_left_txt() const   { return get_FilePath("__left.txt"); }
    QString get_path_right_txt() const   { return get_FilePath("__right.txt"); }

    bool exists() const;

    QPixmap getImageCV(QSize iconSize, bool bFixedSize = false) const;

    QPixmap getImageQt(QSize iconSize, bool bFixedSize = false, QColor bgColor = Qt::black) const;

    void deleteFiles();

private:

    QString get_FilePath(QString const & suff) const
    {
        return path + "/" + tag + suff;
    }

    static bool exist_file( QString const & path)
    {
        QFileInfo f(path);
        return f.exists() && f.isFile();
    }
};

#endif // CACTIVEZONEITEM_H
