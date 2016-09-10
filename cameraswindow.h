#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QMainWindow>
#include <string>
#include <opencv2/core/core.hpp>

namespace Ui {
class CameraWindow;
}

class CameraWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraWindow(QWidget *parent = 0);
    void init(int* camera, int* camera2, bool *useStereo);
    ~CameraWindow();
    void saveCameras(std::string path = "cameras.txt");

    std::vector<cv::Size> getResolutions();

private:
    void closeEvent(QCloseEvent *event);
    void loadCameras();

private slots:
    void on_pushButtonStereo_clicked();

signals:
    void closed();

private:
    Ui::CameraWindow *ui;
    int* camera_;
    int* camera2_;
    bool* useStereo_;

    std::vector<cv::Size> cameraResolutions;
    int prevCam1, prevCam2;
};

#endif // CAMERAWINDOW_H
