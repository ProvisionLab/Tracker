#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "framereader.h"
#include "selector.h"
#include "trackerAPI.h"
#include "detectorAPI.h"
#include "timer.h"
#include "fpscounter.h"
#include "trackerthread.h"
#include "posmatcher.h"
#include "kalmanxy.h"
#include "kalmandepth.h"
#include "easychange.h"
#include "cameracalibration.h"
#include "CActiveZoneItem.h"


// tracker parameters
const bool HOG = true;
const bool LAB = true;
const bool FIXEDWINDOW = false;

const float MAX_DEPTH = 25.f;

const int MIN_ARROW_POS = 14;
const int MAX_ARROW_POS = 536;

namespace Ui {
class MainWindow;
}

struct Params
{
    double a1, b1, c1, d1, e1, f1;
    double a2, b2, c2, d2, e2, f2;

    double base;
    double cam1Ang, cam2Ang;
    double focalLength;
    double HFOV, VFOV;

    bool useDisparity;
    bool useTriangulation;
    bool useCorrection1;
    bool useCorrection2;
    bool useCamCorrection;
    bool metric;
    bool useFPFix;

    int cameraWidth;
    int cameraHeight;

    float corrA;
    float corrB;
    float corrC;
    float corrKdop;

    bool useCalibration;
};

struct ArrowLabel
{
    QLabel* label;
    QLabel* labelVal;
    float value;

    bool clicked(const QPoint& pos) const;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject* object, QEvent* event);

    void initSource();

    int getMouseX();
    int getMouseY();

    int getMouseX2();
    int getMouseY2();

    QString round_(float value);

    float getDepthDisparityVal(float x1, float x2, float y1, float y2);
    QString getDepthDisparity(float x1, float x2, float y1, float y2);
    QString getDepthTriangle(float x1, float x2, float y1, float y2, float resX, float resY,
                             float hfov, float vfov);
    float getDepthTriangleVal(float x1, float x2, float y1, float y2, float resX, float resY,
                              float hfov, float vfov);
    float getDepthTriangleValFull(float x1, float x2, float y1, float y2, float resX, float resY,
                                  float hfov, float vfov);

    QString getMetric(float val, bool shortVal = false);

    bool detectObject(int x, int y, bool isLeft);

    float computeArrowPos(float val);
    void setGreenArrow(float val);
    void setRedArrow(float val);
    void setRightArrow(float val, QLabel* base, QLabel* text);
    void setArrowLine(float maxValue);
    void setArrowLine();

    float computeArrowValueFromPos(int pos);

private slots:
    void displayFrame();
    void openCameraWindow();
    void loadResolutions();
    void setParametersAction();

    void on_pushButton_clicked();
    void on_saveObj_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_kalmanSlider_valueChanged(int value);

    void on_focusThreshold_valueChanged(int value);

    void on_focusEasyChange_valueChanged(int value);

    void on_focusChangeTime_valueChanged(int value);

    void on_Main_Tab_currentChanged(int index);

    void on_pushButton_2_clicked();

    void on_calibrationConfirm_clicked();

    void on_calibrationNext_clicked();

    void on_manualChanger_valueChanged(int value);

    void on_loadVideo_clicked();

    void on_pushButtonStereo_clicked();

    void saveCameras(std::string path);
    std::vector<cv::Size> getResolutions();
    void loadCameras();

    void on_resolutions_currentIndexChanged(int index);

    void setParamsValues();
    void setParamsCamRes();
    void saveParams(std::string path);
    void loadParams(std::string path);
    void setCamResolutions(const std::vector<cv::Size> &res);

    void on_pushButton_3_clicked();

    void on_manualChangerSpin_editingFinished();

    void on_calibrationCancel_clicked();

    void on_pauseButton_clicked();

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_setsPathButton_clicked();

    void on_libraryPathButton_clicked();

    void on_pushButtonStop_clicked();

private:
    Ui::MainWindow *ui;
    FrameReader frameReader;

    // additional tools for testing
    FpsCounter fps;
    Selector mouseSelector;

    QTimer *timer; // the timer that will refresh the widget
    cv::Mat left, right; // the frame that we will copy readed images from video
    cv::Mat leftBase, rightBase;
    cv::Mat leftResized, rightResized;

    // source info
    bool useVideo, useStereo;
    int prevCam1, prevCam2;
    int camera, camera2;
    std::string videoPath;

    bool tracking;

    Params params;

    bool clickOnLeft;
    int clickInX, clickInY;

    // KCF tracker
    TrackerAPI tracker, tracker2;
    KalmanXY kalman, kalman2;
    DetectorAPI detector;
    TrackerThread trackerTh, trackerTh2;
    float rect2Scale;
    cv::Rect2f resultL, resultR;
    cv::Rect2f resultKalmanL, resultKalmanR;
    float x1,y1,x2,y2;
    float depthTriangle;
    float x2Base, y2Base;
    int resulutionW, resulutionH;
    cv::Size cameraResolution_;

    bool detectLeft, detectRight;
    int detectPosX, detectPosY;

    // save set
    cv::VideoWriter* video1;
    cv::VideoWriter* video2;
    bool isRecording;
    QString saveDir;
    std::vector<std::pair<int, cv::Rect> > recordedRect_;
    int numRecordedFrames_;
    std::map<int, cv::Rect> loadedRects_;

    // match by key points
    PosMatcher matcher_;
    bool debugFix;
    cv::Point2f fixShifts_[NUM_FEATURE_POINTS];

    // kalman pose prediction
    float kalmanSensitivity_;
    bool kalmanState;
    float prevAccuracy;
    int attempt;

    // focus processing
    bool needInitKalman;
    KalmanDepth kalmanDepth;
    EasyChange easyDepth;

    // calibration
    bool isCalibrationTab;
    bool calibrationMode;
    CameraCalibration calibration;
    double calibrationCenterDistance;
    bool calibrationCollect;

    float arrowScaleParam;

    // resolution in params
    std::vector<cv::Size> resolutions;
    bool addResolutions;

    // detect ojects
    std::vector<CActiveZoneItem> objectsForDetect;

    bool manualMode;
    bool manualEasyChange;
    long long lastClickTime;

    QPoint redArrowDragPosition;

    std::vector<ArrowLabel> arrowLabels;

    float currentTargetFocus;
};

#endif // MAINWINDOW_H
