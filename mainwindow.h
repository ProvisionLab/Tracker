#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTimer>
#include "ui_mainwindow.h"


#include "selector.h"
#include "framereader.h"

#include "trackerAPI.h"

#include "timer.h"
#include "fpscounter.h"

#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void init(int* camera, int* camera2, std::string* videoFile,
              bool* useCamera, bool* useFile, bool *useStereo, Ui_MainWindow** uiWindow);
    ~MainWindow();


public slots:
    void DisplayImage();

private slots:

    void on_pushButton_video_clicked();
    void on_pushButtonCamera_clicked();

    void on_pushButtonStereo_clicked();

    void on_btnStart_clicked();

    void on_btnFile_clicked();

    void on_btnStop_clicked();

private:
    Ui::MainWindow *ui;
    int* camera_;
    int* camera2_;
    std::string* videoFile_;
    bool* useCamera_;
    bool* useFile_;
    bool useFile;
    bool* useStereo_;

    QTimer* Timer;
    bool needPause;

    FrameReader frameReader;
    // tracker engine
    bool tracking = false;
    TrackerAPI tracker;
    cv::Rect resultL, resultR;

};

#endif // MAINWINDOW_H
