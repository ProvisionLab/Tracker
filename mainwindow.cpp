#include <QtCore>
#include <QCursor>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <stdio.h>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    useVideo(false),
    useStereo(false),
    prevCam1(-1),
    prevCam2(-1),
    camera(0),
    camera2(0),
    tracking(false),
    detectLeft(false),
    detectRight(false),
    video1(NULL),
    video2(NULL),
    isRecording(false),
    debugFix(false),
    kalmanSensitivity_(0.01),
    needInitKalman(true),
    isCalibrationTab(false),
    manualMode(true),
    lastClickTime(0),
    currentTargetFocus(0),
    manualEasyChange(false)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(displayFrame()));

    setMouseTracking(true);

    loadParams("params.txt");
    loadResolutions();

    x1 = x2 = y1 = y2 = 0;

    cameraResolution_.width = params.cameraWidth;
    cameraResolution_.height = params.cameraHeight;
    frameReader.setResolution(cameraResolution_);

    ui->kalmanSlider->setValue(kalmanSensitivity_ * 1000);

    ui->leftCam->setVisible(false);
    ui->rightCam->setVisible(false);

    ui->calibrationConfirm->setVisible(false);
    ui->calibrationNext->setVisible(false);
    ui->calibrationPprogressBar->setVisible(false);
    ui->calibrationLabel->setVisible(false);
    ui->calibrationCancel->setVisible(false);

    setArrowLine(15);
    setGreenArrow(0.);
    setRedArrow(0.);

    loadCameras();

    ui->SetLibrary->setRoot(ui->libraryPathEdit->text());
    ui->SetLibrary->setListView(ui->SetLibraryCurrent);

    ui->greenArrow->installEventFilter(this);
    ui->greenArrowVal->installEventFilter(this);
    ui->redArrow->installEventFilter(this);
    ui->redArrowVal->installEventFilter(this);
    ui->manualChanger->installEventFilter(this);
    ui->focusPanel->installEventFilter(this);
    ui->l1->installEventFilter(this);
    ui->l2->installEventFilter(this);
    ui->l3->installEventFilter(this);
    ui->l4->installEventFilter(this);
    ui->l5->installEventFilter(this);
    ui->l6->installEventFilter(this);
    ui->l7->installEventFilter(this);

    QPalette palette;
    palette.setColor(QPalette::Button, Qt::green);
    ui->manualChanger->setPalette(palette);

    ui->pushButtonStop->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::round_(float value)
{
    char str[100];
    sprintf(str, "%.2f", value);
    return str;
}

void MainWindow::initSource()
{
    frameReader.close();
    tracker.reset();
    resultR = resultR = cv::Rect();
    tracking = false;
    if (useStereo)
    {
        std::string err = frameReader.initStereo(camera, camera2);
        if (err.length())
        {
            QMessageBox::information(0, "Error with stereo camera", QString::fromStdString(err));
            useStereo = false;
        }
    }
    else if (useVideo)
    {
        std::string err = frameReader.initVideo(videoPath);
        if (err.length())
        {
            QMessageBox::information(0, "Error with video file", QString::fromStdString(err));
            useVideo = false;
        }
    }
    if (useVideo || useStereo)
    {
        if (calibration.load(useVideo ? videoPath : "", cameraResolution_.height) && ui->useCalibration)
        {
            setArrowLine(calibration.getMaxDistance());
        }
        else
        {
            setArrowLine(15.);
        }
        timer->start(30);
        tracking = false;
    }
    else
    {
        timer->stop();
        ui->saveObj->setEnabled(false);
        ui->text->setText("Waiting source...");
    }
}

float MainWindow::getDepthDisparityVal(float x1, float x2, float y1, float y2)
{
    float disparity = std::sqrt(std::pow(x1 - x2, 2.) + std::pow(y1 - y2, 2.));

    if (disparity < 0.1)
    {
        return 1e10;
    }

    float c = params.base * params.focalLength;
    float x = c / disparity;
    if (params.useCorrection1)
    {
        x = params.a1 + params.b1 * x + params.c1 * std::pow(x, 2) + params.d1 * std::pow(x, 3) + params.e1 * std::pow(x, 4) + params.f1 * std::pow(x, 5);
    }
    return x;
}


QString MainWindow::getDepthDisparity(float x1, float x2, float y1, float y2)
{
    QString res;

    float x = getDepthDisparityVal(x1, x2, y1, y2);
    if (x > MAX_DEPTH - 0.01)
    {
        return "far";
    }

    if (params.useFPFix)
    {
        std::vector<float> results;
        for (int i = 0; i < NUM_FEATURE_POINTS; i++)
        {
            float t = getDepthDisparityVal(x1, x2 + fixShifts_[i].x, y1, y2 + fixShifts_[i].y);
            results.push_back(t);
        }
        std::nth_element(results.begin(), results.begin() + results.size() / 2, results.end());
        x = results[results.size() / 2];
    }

    res += getMetric(x);
    return res;
}

QString MainWindow::getDepthTriangle(float x1, float x2, float y1, float y2, float resX, float resY, float hfov, float vfov)
{
    QString res;

    float resF = getDepthTriangleValFull(x1, x2, y1, y2, resX, resY, hfov, vfov);

    if (resF > MAX_DEPTH - 0.01)
    {
        return "far";
    }

    res += getMetric(resF);

    return res;
}


float MainWindow::getDepthTriangleValFull(float x1, float x2, float y1, float y2, float resX, float resY, float hfov, float vfov)
{
    float resF = getDepthTriangleVal(x1, x2, y1, y2, resX, resY, hfov, vfov);

    if (params.useFPFix)
    {
        std::vector<float> results;
        for (int i = 0; i < NUM_FEATURE_POINTS; i++)
        {
            float t = getDepthTriangleVal(x1, x2 + fixShifts_[i].x, y1, y2 + fixShifts_[i].y, resX, resY, hfov, vfov);
            results.push_back(t);
        }
        std::nth_element(results.begin(), results.begin() + results.size() / 2, results.end());
        resF = results[results.size() / 2];
    }

    return resF;
}

float MainWindow::getDepthTriangleVal(float x1, float x2, float y1, float y2, float resX, float resY, float hfov, float vfov)
{
    //    x1=   600;
    //    x2=	224;
    //    y1=	45;
    //    y2=	55;

    //std::cout << hfov << " " << vfov << std::endl;

    const float pi = std::acos(-1);

    // see task 3 image
    float alfa = pi / 2 - std::atan(((x1 - resX / 2) / ( resX / 2)) * std::tan(hfov * pi / 360.)) + params.cam2Ang * pi / 180.; // cam2 angle
    //std::cout << ((x1 - resX / 2) / ( resX / 2)) << std::endl;
    //std::cout << "alfa" << alfa << std::endl;
    float beta = pi/2 + std::atan(((x2 - resX / 2) / (resX / 2)) * std::tan(hfov * pi / 360.)) - params.cam1Ang * pi / 180.; // cam1 angle
    //std::cout << "beta" << beta << std::endl;
    float gamma = std::fabs(pi - alfa - beta);
    //std::cout << "gamma" << gamma << std::endl;

    if (std::sin(gamma) < 0.01)
    {
        return 1e10;
    }

    float c = params.base / 1000.; // base line in meters
    //std::cout << "c" << c << std::endl;
    float a = std::sin(alfa) * c / std::sin(gamma);
    //std::cout << "a" << a << std::endl;
    float b = std::sin(beta) * c / std::sin(gamma);
    //std::cout << "b" << b << std::endl;

    float t = 2 * a * a + 2 * b * b - c * c;
    t = std::max(0.001f, t);
    float m = std::sqrt(t) / 2.;
    //std::cout << "m" << m << std::endl;

    float y = (y1 + y2) / 2;
    //std::cout << "y" << y << std::endl;
    float angleY = std::atan(((y - resY / 2) /(resY / 2)) * std::tan(vfov * pi / 360.));
    //std::cout << "angleY" << angleY << std::endl;
    float cosAngleY = std::cos(angleY);
    //std::cout << "cosAngleY" << cosAngleY << std::endl;

    if (cosAngleY < 0.001)
    {
        return 1e10;
    }

    float x = m / cosAngleY;
    //std::cout << "distance " << x << std::endl;
    //std::cout << "=============================" << std::endl;
    if (params.useCorrection2)
    {
        x = params.a2 + params.b2 * x + params.c2 * std::pow(x, 2) + params.d2 * std::pow(x, 3) + params.e2 * std::pow(x, 4) + params.f2 * std::pow(x, 5);
    }

    return x;
}


QString MainWindow::getMetric(float val, bool shortVal)
{
    if (params.metric)
    {
        if (shortVal)
        {
            return round_(val);
        }
        return round_(val) + "M";
    }

    float f = val * 3.28084;

    int fi = int(f);
    float fd = 12 * (f - fi);

    if (shortVal)
    {
        return QString::number(fi) + "'" + QString::number(std::round(fd)) + "''";
    }

    return QString::number(fi) + "' " + round_(fd) + "''";
}

void MainWindow::openCameraWindow()
{
    loadedRects_.clear();
    if (isRecording)
    {
        on_pushButton_clicked();
    }
    loadResolutions();
}

void MainWindow::loadResolutions()
{
    std::vector<cv::Size> camResolution = getResolutions();
    setCamResolutions(camResolution);
}

void MainWindow::displayFrame()
{
    long long fullTime = getTimeMs();
    long long loadTime = getTimeMs();

    if (!frameReader.getFrame(left, right))
    {
        ui->pushButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        // reset tracker
        mouseSelector.reset();
        tracking = false;
        tracker.reset();
        timer->stop();
        ui->saveObj->setEnabled(false);
        if (isRecording)
        {
            on_pushButton_clicked();
        }
        QMessageBox::information(0, "Information", "End of stream");
        ui->loadVideo->setText("Load");
        ui->loadVideo->setStyleSheet("");
        ui->pushButtonStop->setVisible(false);
        return;
    }

    bool isLostLeft = false, isLostRight = false;

    ui->pushButton->setEnabled(true);
    ui->pauseButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    resulutionW = left.cols;
    resulutionH = left.rows;

    loadTime = getTimeMs() - loadTime;

    left.copyTo(leftBase);
    right.copyTo(rightBase);

    if (isRecording)
    {
        if (video1)
        {
            video1->write(left);
        }
        if (video2)
        {
            video2->write(right);
        }
        numRecordedFrames_++;
    }

    long long trackerTime = 0, fixTime = 0;

    cv::resize(left, leftResized, cv::Size(ui->leftCam->width(), ui->leftCam->height()));
    cv::resize(right, rightResized, cv::Size(ui->rightCam->width(), ui->rightCam->height()));

    if (frameReader.isPause())
    {
        cv::putText(leftResized, "PAUSE", cv::Point(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1., cv::Scalar(0, 255, 0));
    }

    if (isCalibrationTab)
    {
        const int STEP = 20;
        const cv::Scalar COLOR(100, 100, 100);
        for(int i = 0; i < leftResized.rows / STEP; i++)
        {
            cv::line(leftResized, cv::Point(0, i * STEP), cv::Point(leftResized.cols, i * STEP), COLOR, 1);
            cv::line(rightResized, cv::Point(0, i * STEP), cv::Point(rightResized.cols, i * STEP), COLOR, 1);
        }
        for(int i = 0; i < leftResized.cols / STEP; i++)
        {
            cv::line(leftResized, cv::Point(i * STEP, 0), cv::Point( i * STEP, leftResized.rows), COLOR, 1);
            cv::line(rightResized, cv::Point(i * STEP, 0), cv::Point(i * STEP, rightResized.rows), COLOR, 1);
        }
        cv::line(leftResized, cv::Point(0, leftResized.rows / 2), cv::Point(leftResized.cols, leftResized.rows / 2), cv::Scalar(0, 255, 0), 2);
        cv::line(rightResized, cv::Point(0, rightResized.rows / 2), cv::Point(rightResized.cols, rightResized.rows / 2), cv::Scalar(0, 255, 0), 2);

        cv::circle(leftResized, cv::Point(leftResized.cols / 2, leftResized.rows / 2), 2, cv::Scalar(0, 255, 0), 3);
        cv::circle(rightResized, cv::Point(leftResized.cols / 2, leftResized.rows / 2), 2, cv::Scalar(0, 255, 0), 3);
    }

    float scaleX = left.cols / (float)leftResized.cols;
    float scaleY = left.rows / (float)leftResized.rows;
    mouseSelector.setScale(scaleX, scaleY);

    bool isLoaded = false;
    cv::Rect loadedRect;
    if (loadedRects_.find(frameReader.getFrameNum()) != loadedRects_.end())
    {
        isLoaded = true;
        loadedRect = loadedRects_[frameReader.getFrameNum()];
        std::cout << loadedRect << std::endl;
    }

    bool badDetect = false;
    if (detectLeft)
    {
        tracker.detect(left, resultL, detectPosX, detectPosY);

        if (tracker.getAccuracy() >= ui->horizontalSlider->value() / 100.)
        {
            detector.detect(left, right, resultL, resultR); // detect on right
            rect2Scale = resultR.width / (float)resultL.width;
            tracker2.init(right, resultR); // init right tracker
            kalman.init();
            kalman2.init();
            detectLeft = false;
            needInitKalman = true;
            easyDepth.reset();
            objectsForDetect.clear();
            ui->searchLabel->setText("");
        }
        else
        {
            badDetect = true;
            if (!detectObject(detectPosX, detectPosY, true))
            {
                tracking = false;
            }
        }
    }
    else if (detectRight)
    {
        tracker.detect(left, resultL);
        tracker2.detect(right, resultR, detectPosX, detectPosY);
        kalman2.init();
        rect2Scale = resultR.width / (float)resultL.width;
        detectRight = false;
        needInitKalman = true;
        easyDepth.reset();
        ui->searchLabel->setText("");
    }
    else
    {
        if (tracking && !isLoaded)
        {
            if (!frameReader.isPause())
            {
                tracker.stopTrain(kalmanState && ui->useKalman->isChecked());

                trackerTime = getTimeMs();
                int t1 = -1;
                trackerTh.set(&tracker, &left, &resultL, &t1);
                int t2 = rect2Scale * resultL.width;
                trackerTh2.set(&tracker2, &right, &resultR, &t2);
                trackerTh.start();
                trackerTh2.start();
                trackerTh.wait();
                trackerTh2.wait();
                trackerTh.quit();
                trackerTh2.quit();
                trackerTime = getTimeMs() - trackerTime;

                tracker.stopTrain(false);

                float accuracy = tracker.getAccuracy();


                bool needPredict = accuracy < ui->horizontalSlider->value() / 100.;
                isLostLeft = needPredict;
                if (!kalmanState && needPredict)
                {
                    prevAccuracy = tracker.getAccuracy();
                }
                bool needPredict2 = tracker2.getAccuracy() < ui->horizontalSlider->value() / 100.;
                isLostRight = needPredict2;

                if (needPredict)
                {
                    attempt = std::max(attempt - 1, 0);
                }
                else
                {
                    attempt = 0;
                }

                kalmanState = needPredict && !attempt;

                float diff = accuracy - prevAccuracy * 0.7;
                const float TH = 0.001;
                if (kalmanState && diff > TH)
                {
                    attempt = (diff + TH) * 10 + 10;
                }

//                std::cout << kalmanState << "==>  " << attempt << "  " << tracker.getAccuracy() << "   " << diff << std::endl;

                if (ui->useKalman->isChecked())
                {
                    kalman.update(resultL, resultKalmanL, !kalmanState);
                }
                else
                {
                    kalman.update(resultL, resultKalmanL, !needPredict);
                }
                kalman2.update(resultR, resultKalmanR, !needPredict2);

                if (kalmanState && ui->useKalman->isChecked())
                {
                    resultL = resultKalmanL;
                    tracker.setRect(resultL);
                }
            }
        }
        if (mouseSelector.isSelected() || isLoaded)
        {
            if (clickOnLeft || isLoaded)
            {
                if (isRecording)
                {
                    recordedRect_.push_back(std::make_pair(numRecordedFrames_, mouseSelector.getRect()));
                }

                resultL = isLoaded ? loadedRect : mouseSelector.getRect();
                tracker.init(left, resultL); // init left tracker
                manualMode = ui->isManulMode->isChecked();
                kalman.init();
                detector.detect(left, right, resultL, resultR); // detect on right
                rect2Scale = resultR.width / (float)resultL.width;
                tracker2.init(right, resultR); // init right tracker
                kalman2.init();

                needInitKalman = true;
                tracking = true;
            }
            else if (tracking)
            {
                resultR = mouseSelector.getRect();
                tracker2.init(right, resultR); // init right tracker
                kalman2.init();
                needInitKalman = true;
                rect2Scale = resultR.width / (float)resultL.width;
            }
            easyDepth.reset();
            mouseSelector.reset();
        }
        else if (mouseSelector.isSelecting())
        {
            if (clickOnLeft)
            {
                mouseSelector.draw(leftResized);
            }
            else
            {
                mouseSelector.draw(rightResized);
            }
        }
    }

    if (!badDetect)
    {
        // draw object ROI on right image
        if (tracking)
        {
            static int numProcessedFrames = 0;
            numProcessedFrames++;
            fixTime = getTimeMs();
            if (params.useFPFix)
            {
                if (numProcessedFrames % 5 == 1)
                {
                    std::vector<std::pair<cv::Point2f, cv::Point2f> > pairs;
                    bool isFixed = matcher_.match(leftBase, rightBase, resultL, resultR, pairs, debugFix);
                    if (isFixed && pairs.size() == NUM_FEATURE_POINTS)
                    {
                        cv::Point2f cl(resultL.width / 2, resultL.height / 2);
                        cv::Point2f cr(resultR.width / 2, resultR.height / 2);
                        for (int i = 0; i < NUM_FEATURE_POINTS; i++)
                        {
                            fixShifts_[i] = ((pairs[i].second - cr)) - (pairs[i].first - cl);
                        }
                    }
                    else
                    {
                        for (int i = 0; i < NUM_FEATURE_POINTS; i++)
                        {
                            fixShifts_[i] = cv::Point2f();
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < NUM_FEATURE_POINTS; i++)
                {
                    fixShifts_[i] = cv::Point2f();
                }
            }
            fixTime = getTimeMs() - fixTime;

            if (tracker.getAccuracy() < ui->horizontalSlider->value() / 100.)
            {
                cv::rectangle(leftResized, cv::Rect(0, 0, leftResized.cols, leftResized.rows), cv::Scalar(0,255,0), 7);

                // draw kalman prediction
                cv::Rect resultKalmanLResized = resultKalmanL;
                resultKalmanLResized.x /= scaleX;
                resultKalmanLResized.y /= scaleY;
                resultKalmanLResized.width /= scaleX;
                resultKalmanLResized.height /= scaleY;
                cv::rectangle(leftResized, resultKalmanLResized, cv::Scalar(0, 0, 255), 3);
            }

            if (tracker2.getAccuracy() < ui->horizontalSlider->value() / 100.)
            {
                cv::rectangle(rightResized, cv::Rect(0, 0, rightResized.cols, rightResized.rows), cv::Scalar(0,255,0), 7);

                // draw kalman prediction
                cv::Rect resultKalmanRResized = resultKalmanR;
                resultKalmanRResized.x /= scaleX;
                resultKalmanRResized.y /= scaleY;
                resultKalmanRResized.width /= scaleX;
                resultKalmanRResized.height /= scaleY;
                cv::rectangle(rightResized, resultKalmanRResized, cv::Scalar(0, 0, 255), 3);
            }

            // draw tracker result (on left image)
            cv::Rect resultLResized = resultL;
            resultLResized.x /= scaleX;
            resultLResized.y /= scaleY;
            resultLResized.width /= scaleX;
            resultLResized.height /= scaleY;
            cv::Rect resultRResized = resultR;
            resultRResized.x /= scaleX;
            resultRResized.y /= scaleY;
            resultRResized.width /= scaleX;
            resultRResized.height /= scaleY;

            cv::rectangle(leftResized, resultLResized, cv::Scalar(0, 255, 0), 1, 8);
            cv::circle(leftResized, cv::Point(resultLResized.x + resultLResized.width / 2, resultLResized.y + resultLResized.height / 2), 1, cv::Scalar(0,255,0), 2);

            // draw right image result
            cv::rectangle(rightResized, resultRResized, cv::Scalar(0, 255, 0));
            cv::circle(rightResized, cv::Point(resultRResized.x + resultRResized.width / 2, resultRResized.y + resultRResized.height / 2), 1, cv::Scalar(0,255,0), 2);

            // save results
            x1 = resultL.x + resultL.width / 2;
            y1 = resultL.y + resultL.height / 2;
            x2 = resultR.x + resultR.width / 2;
            y2 = resultR.y + resultR.height / 2;
            ui->saveObj->setEnabled(true);

            ui->calibrationConfirm->setEnabled(true);
        }
        else // !tracking
        {
            x1 = x2 = y1 = y2 = 0;
            for (int i = 0; i < NUM_FEATURE_POINTS; i++)
            {
                fixShifts_[i] = cv::Point2f();
            }
            ui->saveObj->setEnabled(false);

            ui->calibrationConfirm->setEnabled(false);
        }
    }

    fullTime = getTimeMs() - fullTime;

    static float avgTime = fullTime;
    avgTime = 0.8 * avgTime + 0.2 * fullTime;

    static float avgTrackerTime = trackerTime;
    avgTrackerTime = 0.8 * avgTrackerTime + 0.2 * trackerTime;

    static float avgFixTime = fixTime;
    avgFixTime = 0.8 * avgFixTime + 0.2 * fixTime;

    static float avgLoadTime = loadTime;
    avgLoadTime = 0.8 * avgLoadTime + 0.2 * loadTime;

    long long guiTime = fullTime - trackerTime - fixTime - loadTime;
    static float avgGUITime = guiTime;
    avgGUITime = 0.8 * avgGUITime + 0.2 * guiTime;

    // show left video
    cvtColor(leftResized, left, CV_BGR2RGB);
    QImage imgLeft = QImage((unsigned char*)left.data, left.cols, left.rows, left.step, QImage::Format_RGB888);
    QPixmap pixLeft = QPixmap::fromImage(imgLeft);
    ui->leftCam->setPixmap(pixLeft);

    // show right video
    cvtColor(rightResized, right, CV_BGR2RGB);
    QImage imgRight = QImage((unsigned char*)right.data, right.cols, right.rows, right.step, QImage::Format_RGB888);
    QPixmap pixRight = QPixmap::fromImage(imgRight);
    ui->rightCam->setPixmap(pixRight);

    // draw text
    QString text;
    if (useVideo)
    {
        text = "Video: " + QString::fromStdString(videoPath);
    }
    else if (useStereo)
    {
        text = "Stereo camera using";
    }
    text += "\nFrame: " + QString::number(frameReader.getFrameNum());
    if (frameReader.getTotalNumFrames() > 0)
    {
        text += " / " + QString::number(frameReader.getTotalNumFrames());
    }
    text += "\nTime: " + QString::number(int(avgTime)) + " ms";
//    text += "\t(T: " + QString::number(int(avgTrackerTime)) + " ms, ";
//    text += "F: " + QString::number(int(avgFixTime)) + " ms, ";
//    text += "C: " + QString::number(int(avgLoadTime)) + " ms, ";
//    text += "GUI: " + QString::number(int(avgGUITime)) + " ms)";
    text += "\nCamera Fps: " + QString::number(fps.updateFps());

    QString text2;
    text2 = "Frame resolution: " + QString::number(resulutionW) + "x" + QString::number(resulutionH) + "\n";
    text2 += "Left: " + round_(x1) + ", " + round_(y1) + "\n";
    text2 += "Right: " + round_(x2) + ", " + round_(y2) + "\n";
    if (params.useDisparity)
    {
        float disparity = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
        text2 += "Disparity = " + round_(disparity) + "\n";
    }
    text2 += "Left Tracker Accuracy = " + QString::number(tracker.getAccuracy(), 'g', 2) + "\n";
    text2 += "Right Tracker Accuracy = " + QString::number(tracker2.getAccuracy(), 'g', 2) + "\n";

    // new correction algorithm
    float Distance = getDepthTriangleVal(x1, x2, y1, y2, resulutionW, resulutionH, params.HFOV, 1);
    float Kvertical = (y1 + y2 - resulutionH) / (2 * resulutionH);
    float Kperspective = Kvertical * (params.corrA + params.corrB * Distance + params.corrC * Distance * Distance) / 100.;
    float FOVH1 = params.HFOV / (1. + Kperspective * params.corrKdop);

    text2 += "Distance with FOVV=1: " + round_(Distance) + "\n";
    text2 += "New FOVH1: " + round_(FOVH1);

    QString text3;
    if (params.useDisparity)
    {
        text3 += getDepthDisparity(x1, x2, y1, y2);
    }
    if (params.useTriangulation)
    {
        if (text3.length())
        {
            text3 += "\n";
        }
        text3 += getDepthTriangle(x1, x2, y1, y2, resulutionW, resulutionH, params.HFOV, params.VFOV);
    }
    if (params.useCamCorrection)
    {
        if (text3.length())
        {
            text3 += "\n";
        }
        text3 += getDepthTriangle(x1, x2, y1, y2, resulutionW, resulutionH, FOVH1, params.VFOV);
    }
    if (params.useCalibration && !calibrationCollect && !calibrationMode)
    {
        if (text3.length())
        {
            text3 += "\n";
        }
        float depth = getDepthTriangleVal(x1, x2, y1, y2, resulutionW, resulutionH, params.HFOV, params.VFOV);;
        if (depth > MAX_DEPTH - 0.01)
        {
            text3 += "far";
        }
        else
        {
            text3 += "C: " + getMetric(calibration.getFixedDepth(depth, y1));
        }
    }

    if (tracking)
    {
        depthTriangle = getDepthTriangleValFull(x1, x2, y1, y2, resulutionW, resulutionH, params.HFOV, params.VFOV);

        if (tracking)
        {
            if (calibrationCollect)
            {
                if (tracker.getAccuracy() < ui->horizontalSlider->value() / 100.
                        || tracker2.getAccuracy() < ui->horizontalSlider->value() / 100.)
                {
                    on_calibrationNext_clicked();
                    QMessageBox::information(0, "Information", "Tracker lost.\nRestart calibration for this distance\n"
                                                               "or if for this distance collected enought data - calibrate next distance.");
                }
                else
                {
                    calibration.addPoint(calibrationCenterDistance, y1, depthTriangle);
                    ui->calibrationPprogressBar->setValue(calibration.getProgress());
                }
            }
        }


        float depth = depthTriangle;
        if (params.useCalibration && !calibrationCollect && !calibrationMode)
        {
//            std::cout << depth << "   ";
            depth = calibration.getFixedDepth(depth, y1);
//            std::cout << depth << "   ";
        }
        depth = std::min(depth, MAX_DEPTH);

        float kalmanDepthVal;
        kalmanDepth.update(depth, kalmanDepthVal, needInitKalman);
//        std::cout << kalmanDepthVal << "   ";
        needInitKalman = false;

        setGreenArrow(kalmanDepthVal);

        // analize tracking lost
        static float prevDistance = -1.;
        float diff = prevDistance < 0. ? 0. : fabs(prevDistance - kalmanDepthVal);
        float diffThreshold = 0.2 * (1. + std::pow(kalmanDepthVal, 2) / 15);// + 1000;
        //std::cout << "Diff = " << diff << "    Th = " << diffThreshold << (diff > diffThreshold ? " LOST" : "") << std::endl;
        if (isLostLeft || isLostRight || kalmanDepthVal > 15. || diff > diffThreshold || calibrationCollect || calibrationMode)
        {
            manualMode = true;
        }

        if (!manualMode)
        {
            //std::cout << kalmanDepthVal << std::endl;
            //for (int i = 0; i < arrowLabels.size(); i++)
            //{
                //std::cout << arrowLabels[i].value << " ";
            //}

//            float newVal = -1.;
//            float minDist = 10000.;
//            for (unsigned int i = 0; i < arrowLabels.size(); i++)
//            {
//                float dist = fabs(arrowLabels[i].value - kalmanDepthVal);
//                if (dist < 0.3 && dist < minDist && arrowLabels[i].label)
//                {
//                    newVal = arrowLabels[i].value;
//                    minDist = dist;
//                }
//            }

//            //std::cout << "Found VALUE = " << newVal << std::endl;
//            if (newVal > 0.)
//            {
//                currentTargetFocus = newVal;
//            }
            currentTargetFocus = kalmanDepthVal;
        }

        if (manualEasyChange || !manualMode)
        {
            easyDepth.update(currentTargetFocus);
            double easyVal = easyDepth.getEasyDepth();

            if (fabs(easyVal - currentTargetFocus) < 0.001)
            {
                manualEasyChange = false;
            }

            ui->text3->setStyleSheet("color: #00aa00;  font-size: 18pt; font-weight:600;\nfont: \"8514oem\";");
            QPalette palette;
            palette.setColor(QPalette::Button, Qt::green);
            ui->manualChanger->setPalette(palette);
            setRedArrow(easyVal);
            ui->manualChanger->setValue(std::max((float)ui->manualChanger->minimum(),
                                                     std::min((float)ui->manualChanger->maximum(), (float)easyVal * 1000.f)));
        }
        prevDistance = kalmanDepthVal;

//        std::cout << easyVal << std::endl;
    }
    else
    {
        if (manualEasyChange)
        {
            easyDepth.update(currentTargetFocus);
            double easyVal = easyDepth.getEasyDepth();

            if (fabs(easyVal - currentTargetFocus) < 0.001)
            {
                manualEasyChange = false;
            }

            ui->text3->setStyleSheet("color: #00aa00;  font-size: 18pt; font-weight:600;\nfont: \"8514oem\";");
            QPalette palette;
            palette.setColor(QPalette::Button, Qt::green);
            ui->manualChanger->setPalette(palette);
            setRedArrow(easyVal);
            ui->manualChanger->setValue(std::max((float)ui->manualChanger->minimum(),
                                                 std::min((float)ui->manualChanger->maximum(), (float)easyVal * 1000.f)));
        }
    }

    if (manualMode && !manualEasyChange && getTimeMs() - lastClickTime > 500)
    {
        float val = ui->manualChanger->value() / 1000.;
        easyDepth.setDepthHard(val);
        currentTargetFocus = val;
    }

    //std::cout << getTimeMs() - lastClickTime << std::endl;
    if (manualMode && getTimeMs() - lastClickTime > 500)
    {
        ui->text3->setStyleSheet("color: #aa0000;  font-size: 18pt; font-weight:600;\nfont: \"8514oem\";");
        QPalette palette;
        palette.setColor(QPalette::Button, Qt::gray);
        ui->manualChanger->setPalette(palette);
        setRedArrow(ui->manualChanger->value() / 1000.);
    }

    //std::cout << "AFTER: " << ui->manualChanger->value() << std::endl;

    ui->text->setText(text);
    ui->text2->setText(text2);
    ui->text3->setText(text3);
}

void MainWindow::setParametersAction()
{
    setParamsValues();
    saveParams("params.txt");

    setEnabled(true);
    setArrowLine();
    if (cameraResolution_.width != params.cameraWidth
            || cameraResolution_.height != params.cameraHeight)
    {
        cameraResolution_.width = params.cameraWidth;
        cameraResolution_.height = params.cameraHeight;
        frameReader.setResolution(cameraResolution_);
        tracking = false;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (isRecording)
    {
        ui->pushButton->setText("Record");
        ui->pushButton->setStyleSheet("");
        ui->recLabel->setText("");
        isRecording = false;
        video1->release();
        std::cout << "Stop recording" << std::endl;

        std::ofstream rectsStream((saveDir + "//rects.txt").toStdString());
        rectsStream << recordedRect_.size() << std::endl;
        for (unsigned int i = 0; i < recordedRect_.size(); i++)
        {
            rectsStream << recordedRect_[i].first << " ";
            rectsStream << recordedRect_[i].second.x << " ";
            rectsStream << recordedRect_[i].second.y << " ";
            rectsStream << recordedRect_[i].second.width << " ";
            rectsStream << recordedRect_[i].second.height << std::endl;
        }
        recordedRect_.clear();
        rectsStream.close();

        // save calibration parameters
        QString calibFile = "calib_" + QString::number(cameraResolution_.height);
        std::cout << (calibFile).toStdString() << "  ==>  " << saveDir.toStdString() << std::endl;
        QFile::copy(calibFile, saveDir + calibFile);

        if (video1)
        {
            delete video1;
        }
        video2->release();
        if (video2)
        {
            delete video2;
        }
    }
    else
    {
        numRecordedFrames_ = 0;
        QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory for save set"),
                                                        ui->setsPathEdit->text(),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
        if (!dir.length())
        {
            return;
        }
        QDateTime dateTime = dateTime.currentDateTime();
        QString dateTimeString = dateTime.toString("yyyy_MM_dd__hh_mm_ss");
        saveDir = dir + "//1ACpro_set_" + dateTimeString + "//";
        if (QDir(saveDir).exists())
        {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Info", "Set was already recorded in " + dir +
                                                                      "\nReplace set?",
                                                                      QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                QDir(saveDir).removeRecursively();
            }
            else
            {
                return;
            }
        }
        QDir().mkdir(saveDir);
        Sleep(1000);
        std::cout << "Start recording to " << saveDir.toStdString() << std::endl;
        saveCameras((saveDir + "cameras.txt").toStdString());
        saveParams((saveDir + "params.txt").toStdString());
        video1 = new cv::VideoWriter((saveDir + "left.avi").toStdString(), CV_FOURCC('M','J','P','G'), 30, cv::Size(resulutionW, resulutionH), true);
        video2 = new cv::VideoWriter((saveDir + "right.avi").toStdString(), CV_FOURCC('M','J','P','G'), 30, cv::Size(resulutionW, resulutionH), true);
        ui->pushButton->setText("Stop");
        ui->pushButton->setStyleSheet("background-color: green");
        ui->recLabel->setText("Recording to " + saveDir);
        isRecording = true;
    }
}

void MainWindow::on_saveObj_clicked()
{
    QString path = ui->SetLibrary->currentDir();

    if ( !QDir(path).exists() )
    {
        qDebug() << "directory not exists: " << path;
        return;
    }

    qDebug() << "save object in: " << path;

    CActiveZoneItem obj(path, QDateTime::currentDateTime().toString("yy_MM_dd_hh_mm_ss"));

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    cv::imwrite( obj.get_path_left().toStdString(), leftBase, compression_params);
    cv::imwrite( obj.get_path_right().toStdString(), rightBase, compression_params);

    std::ofstream fileL(obj.get_path_left_txt().toStdString());
    fileL << resultL.x << " " << resultL.y << " " << resultL.width << " " << resultL.height << std::endl;
    fileL.close();

    std::ofstream fileR(obj.get_path_right_txt().toStdString());
    fileR << resultR.x << " " << resultR.y << " " << resultR.width << " " << resultR.height << std::endl;
    fileR.close();

    cv::Mat ref(leftBase, resultL);
    cv::imwrite(obj.get_path_etalon().toStdString(), ref, compression_params);
}

bool MainWindow::detectObject(int x, int y, bool isLeft)
{
    static int currentObjectId;
    static int numAttempts;
    if (!objectsForDetect.size())
    {
        for ( auto & obj : ui->azone->getSelectedItems() )
        {
            qDebug() << "selected item: " << obj.get_path_etalon();
            objectsForDetect.push_back(obj);
        }

        for ( auto & obj : ui->azone->getNotSelectedItems() )
        {
            qDebug() << "not selected item: " << obj.get_path_etalon();
            objectsForDetect.push_back(obj);
        }

        currentObjectId = 0;
        numAttempts = 0;
    }

    numAttempts++;
    if (!objectsForDetect.size() || numAttempts > 30)
    {
        detectLeft = detectRight = false;
        ui->searchLabel->setText("");
        return false;
    }

    if (currentObjectId >= (int)objectsForDetect.size())
    {
        currentObjectId = 0;
    }
    const CActiveZoneItem& currentObj = objectsForDetect[currentObjectId];

    std::cout << "Search step #" << numAttempts << ":  " << currentObj.get_path_etalon().toStdString() << std::endl;
    ui->searchLabel->setText("Object detection...");

    cv::Mat pattern = cv::imread((isLeft ? currentObj.get_path_left() : currentObj.get_path_right()).toStdString());

    if (pattern.cols != cameraResolution_.width || pattern.rows != cameraResolution_.height)
    {
        QMessageBox::information(0, "Information", "Can't read object: " + currentObj.get_path_etalon());
        return false;
    }

    cv::Rect2f rect;
    std::ifstream file((isLeft ? currentObj.get_path_left_txt() : currentObj.get_path_right_txt()).toStdString().c_str());
    if (!pattern.rows || !pattern.cols || !file.is_open())
    {
        QMessageBox::information(0, "Information", "Can't find any saved objects!");
        return false;
    }

    file >> rect.x >> rect.y >> rect.width >> rect.height;
    file.close();

    if (isLeft)
    {
        tracker.init(pattern, rect);
        manualMode = ui->isManulMode->isChecked();
        kalman.init();
        detectLeft = true;
    }
    else
    {
        tracker2.init(pattern, rect);
        kalman2.init();
        detectRight = true;
    }
    needInitKalman = true;

    detectPosX = x * pattern.cols / ui->leftCam->width();
    detectPosY = y * pattern.rows / ui->leftCam->height();
    tracking = true;

    currentObjectId++;

    return true;
}


void MainWindow::on_pushButton_3_clicked()
{
    setParametersAction();
}


void MainWindow::on_manualChanger_valueChanged(int value)
{
    ui->manualChangerSpin->setValue(value / 1000.);
}

void MainWindow::on_manualChangerSpin_editingFinished()
{
    int newVal = ui->manualChangerSpin->value() * 1000;
    manualMode = true;
    if (std::abs(ui->manualChanger->value() - newVal) > 5)
    {
        ui->manualChanger->setValue(newVal);
    }
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    kalmanDepth.setSensitivity(value / 100.);
    ui->label_32->setText("Kalman sensitivity (" + QString::number(value / 100.) + "):");
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    kalmanDepth.setMedianSize(value);
    ui->label_33->setText("Madian filter smoothing (" + QString::number(value) + "):");
}

void MainWindow::on_setsPathButton_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this, tr("Select path"), ui->setsPathEdit->text(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!filename.isEmpty())
    {
        ui->setsPathEdit->setText(filename);
    }
}

void MainWindow::on_libraryPathButton_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this, tr("Select path"), ui->libraryPathEdit->text(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!filename.isEmpty())
    {
        ui->libraryPathEdit->setText(filename);
        ui->SetLibrary->setRoot(ui->libraryPathEdit->text());
        ui->SetLibrary->setListView(ui->SetLibraryCurrent);
    }
}
