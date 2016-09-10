#include "CActiveZoneItem.h"

#include <QDebug>
#include <QFile>
#include <QPainter>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

CActiveZoneItem::CActiveZoneItem(QFileInfo const & f_etalon)
{
    const int tag_len = sizeof("YY_MM_DD_HH_mm_ss")-1;

    QString fn = f_etalon.fileName();

    if ( fn.size() <= tag_len )
        return;

    if( QStringRef(&fn, tag_len, fn.size()-tag_len) != "__etalon.png" )
        return;

    path = f_etalon.absolutePath();
    tag  = fn.left(tag_len);
}

bool CActiveZoneItem::exists() const
{
    if (isEmpty())
        return false;

    QDir dir(path);
    if ( !dir.exists() )
        return false;

    if (!exist_file(get_path_etalon()))
        return false;
    if (!exist_file(get_path_left()))
        return false;
    if (!exist_file(get_path_right()))
        return false;
    if (!exist_file(get_path_left_txt()))
        return false;
    if (!exist_file(get_path_right_txt()))
        return false;

    return true;
}

void CActiveZoneItem::deleteFiles()
{
    qDebug() << __FUNCTION__ << path << tag;

    QFile::remove(get_path_etalon());
    QFile::remove(get_path_left());
    QFile::remove(get_path_left_txt());
    QFile::remove(get_path_right());
    QFile::remove(get_path_right_txt());
}

QPixmap CActiveZoneItem::getImageCV(QSize iconSize, bool bFixedSize) const
{
    qDebug() << __FUNCTION__ << iconSize << bFixedSize;

    cv::Mat ref = cv::imread( get_path_etalon().toStdString() );
    if (ref.empty())
    {
        return {};
    }

    float scale = std::min(iconSize.height() / (float)ref.rows,
                           iconSize.width() / (float)ref.cols);

    cv::Mat resized;
    cv::resize(ref, resized, cv::Size(ref.cols * scale, ref.rows * scale));

    cv::Mat resizedRgb;

    if ( bFixedSize )
    {
        cv::Mat m3(iconSize.width(), iconSize.height(), resized.type());

        m3.setTo(0);

        resized.copyTo(m3(cv::Rect(
            (m3.cols-resized.cols)/2,
            (m3.rows-resized.rows)/2,
            resized.cols, resized.rows)) );

        cvtColor(m3, resizedRgb, CV_BGR2RGB);
    } else
    {
        cvtColor(resized, resizedRgb, CV_BGR2RGB);
    }

    QImage img((unsigned char*)resizedRgb.data, resizedRgb.cols, resizedRgb.rows, resizedRgb.step, QImage::Format_RGB888);

    return QPixmap::fromImage(img);
}

QPixmap CActiveZoneItem::getImageQt(QSize iconSize, bool bFixedSize, QColor bgColor) const
{
    if ( !bFixedSize )
        return QPixmap::fromImage(
                    QImage(get_path_etalon())
                        .scaled(iconSize)
                    );

    auto img = QImage(get_path_etalon())
                    .scaled(iconSize, Qt::KeepAspectRatio);

    QImage img2(iconSize, img.format());

    img2.fill(bgColor);

    QPainter paint;
    paint.begin(&img2);
    paint.drawImage( (iconSize.width()-img.width())/2, (iconSize.height()-img.height())/2, img);
    paint.end();

    return QPixmap::fromImage(img2);
}
