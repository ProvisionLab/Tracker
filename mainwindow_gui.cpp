#include <QtCore>
#include <QCursor>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsColorizeEffect>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <stdio.h>


const QString CALIBRATION_STEP_1 = "Select object and set it on center. Then click 'Confirm center'...";
const QString CALIBRATION_STEP_2 = "Move object. Then click 'Next distance'...";


bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    if((object == ui->greenArrow || object == ui->greenArrowVal)
            && event->type() == QEvent::MouseButtonPress)
    {
        manualMode = false;
    }
    else if (object == ui->manualChanger && event->type() == QEvent::MouseButtonDblClick)
    {
        manualMode = false;
    }
    else if (object == ui->manualChanger && event->type() == QEvent::MouseButtonPress)
    {
        manualMode = true;
        lastClickTime = getTimeMs();
        //std::cout << "BEFORE: " << ui->manualChanger->value() << std::endl;
    }

    QPalette palette;
    palette.setColor(QPalette::Button, manualMode ? Qt::gray : Qt::green);
    ui->manualChanger->setPalette(palette);

    return false;
}

float MainWindow::computeArrowValueFromPos(int pos)
{
    return std::pow(10., float(MAX_ARROW_POS - pos + 4) * std::log10(arrowScaleParam) / 65.) - 1.; // TODO: 4 - it's fix
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    // process red arrow move
    if (redArrowDragPosition.x() > 0)
    {
        QPoint cursorPos = ui->focusWidget->mapFromGlobal(event->pos());
        if (event->buttons() & Qt::LeftButton)
        {
            int newPos = std::max(13, std::min(537, cursorPos.y() + 22));
            ui->redArrow->move(ui->redArrow->pos().x(), newPos);
            ui->redArrowVal->move(ui->redArrowVal->pos().x(), ui->redArrow->pos().y() + 2);
            float val = computeArrowValueFromPos(newPos);
            ui->redArrowVal->setText(getMetric(val));
            ui->manualChanger->setValue(val * 1000);
        }
        return;
    }

    // process rectangle selection
    int x, y;
    if (clickOnLeft)
    {
        x = getMouseX();
        y = getMouseY();
    }
    else
    {
        x = getMouseX2();
        y = getMouseY2();
    }

    if (!timer->isActive() || frameReader.isFinish() || x < 0 || y < 0)
    {
        return;
    }
    if (mouseSelector.isSelecting())
    {
        mouseSelector.setEnd(x, y);
    }
}

bool ArrowLabel::clicked(const QPoint &pos) const
{
    QPoint arrowPos = label->mapFromGlobal(pos);
    return arrowPos.x() > 0 && arrowPos.y() > 0
            && arrowPos.x() < label->width() && arrowPos.y() < label->height();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    bool isCreatedLabel = false;

    // process red arrow move
    QPoint arrowPos = ui->redArrow->mapFromGlobal(QCursor::pos());
    if (arrowPos.x() > 0 && arrowPos.y() > 0
            && arrowPos.x() < ui->redArrow->width() && arrowPos.y() < ui->redArrow->height())
    {
        if (event->button() == Qt::LeftButton)
        {
            redArrowDragPosition = event->pos();
            manualMode = true;
            QPalette palette;
            palette.setColor(QPalette::Button, Qt::gray);
            ui->manualChanger->setPalette(palette);
            return;
        }
        else if (event->button() == Qt::RightButton)
        {
            QLabel* label = new QLabel(ui->focusWidget);
            QPixmap pixmap;
            pixmap.load(":/qss_icons/focusImg/arrow_red2.png");
            label->setPixmap(pixmap);
            label->setStyleSheet("background-color: rgba(0,0,0,0%)");
            label->setBaseSize(QSize(ui->redArrow->width(), ui->redArrow->height()));
            label->setGeometry(ui->redArrow->geometry());
            int pos = ui->focusWidget->mapFromGlobal(QCursor::pos()).y() - 11;
            //std::cout << pos << std::endl;
            label->move(ui->redArrow->geometry().x(), pos);
            label->show();

            QLabel* labelVal = new QLabel(ui->focusWidget);
            labelVal->setStyleSheet("background-color: rgba(0,0,0,0%); color: rgba(0, 0, 0, 100%)");
            labelVal->setBaseSize(QSize(ui->redArrowVal->width(), ui->redArrowVal->height()));
            labelVal->setGeometry(ui->redArrowVal->geometry());
            float value = ui->manualChanger->value() / 1000.;
            setRightArrow(value, label, labelVal);
            labelVal->show();

            ArrowLabel arrowStruct;
            arrowStruct.label = label;
            arrowStruct.labelVal = labelVal;
            arrowStruct.value = value;
            arrowLabels.push_back(arrowStruct);

            ui->redArrow->raise();
            ui->redArrowVal->raise();

            isCreatedLabel = true;
        }
    }

    // delete labels
    for (unsigned int i = 0; i < arrowLabels.size(); i++)
    {
        ArrowLabel& t = arrowLabels[i];
        if (t.label && t.labelVal && t.clicked(QCursor::pos()))
        {
            if (event->button() == Qt::RightButton && !isCreatedLabel)
            {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Delete label with value " + getMetric(t.value) + "?",
                                                                          QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    t.label->hide();
                    t.labelVal->hide();
                    t.label = t.labelVal = NULL;
                }
            }
            else if(event->button() == Qt::LeftButton)
            {
                manualMode = true;
                manualEasyChange = true;
                QPalette palette;
                palette.setColor(QPalette::Button, Qt::gray);
                ui->manualChanger->setPalette(palette);
                currentTargetFocus = t.value;
                easyDepth.reset();
                std::cout << "Manually changed: " << currentTargetFocus << std::endl;
            }
            return;
        }
    }

    // process rectangle selection
    int x = getMouseX(), y = getMouseY();
    clickOnLeft = true;

    if (x < 0 || y < 0)
    {
        x = getMouseX2();
        y = getMouseY2();
        clickOnLeft = false;
    }

    if (!timer->isActive() || frameReader.isFinish() || x < 0 || y < 0)
    {
        return;
    }
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        clickInX = x;
        clickInY = y;
    }
    else
    {
        clickInX = clickInY = -100;
    }

    if (timer->isActive() && clickOnLeft)
    {
        // reset tracker
        tracking = false;
    }
    mouseSelector.reset();
    mouseSelector.setStart(x, y);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    redArrowDragPosition.setX(-1);

    int x, y;
    if (clickOnLeft)
    {
        x = getMouseX();
        y = getMouseY();
    }
    else
    {
        x = getMouseX2();
        y = getMouseY2();
    }

    if (!timer->isActive() || frameReader.isFinish() || x < 0 || y < 0)
    {
        return;
    }
    if (event->button() == Qt::MouseButton::LeftButton
            && std::abs(clickInX - x) + std::abs(clickInY - y) < 15)
    {
        if (clickOnLeft || (!clickOnLeft && tracking))
        {
            objectsForDetect.clear();
            detectObject(x, y, clickOnLeft);
        }
    }
    mouseSelector.setEnd(x, y, true);
}

void MainWindow::closeEvent(QCloseEvent*)
{
    if (isRecording)
    {
        on_pushButton_clicked();
    }
    saveCameras("cameras.txt");
    setParametersAction();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch ((char)event->key())
    {
    case ' ':
        frameReader.pause(!frameReader.isPause());
        break;
    case 'd':
    case 'D':
        debugFix = !debugFix;
        break;
    default:
        break;
    }
}


int MainWindow::getMouseX()
{
    int fixedX = ui->leftCam->mapFromGlobal(QCursor::pos()).x();
    if (fixedX >= ui->leftCam->size().width())
    {
        return -1;
    }
    return fixedX;
}

int MainWindow::getMouseY()
{
    int fixedY = ui->leftCam->mapFromGlobal(QCursor::pos()).y();
    if (fixedY >= ui->leftCam->size().height())
    {
        return -1;
    }
    return fixedY;
}

int MainWindow::getMouseX2()
{
    int fixedX = ui->rightCam->mapFromGlobal(QCursor::pos()).x();
    if (fixedX >= ui->rightCam->size().width())
    {
        return -1;
    }
    return fixedX;
}

int MainWindow::getMouseY2()
{
    int fixedY = ui->rightCam->mapFromGlobal(QCursor::pos()).y();
    if (fixedY >= ui->rightCam->size().height())
    {
        return -1;
    }
    return fixedY;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label->setText("Tracker accuracy threshold\n(" + QString::number(value / 100.) + ")");
}


void MainWindow::on_kalmanSlider_valueChanged(int value)
{
    kalmanSensitivity_ = value / 1000.;
    ui->kalmanLabel->setText("Kalman sensitivity\n(" + QString::number(kalmanSensitivity_) + ")");
    kalman.setSensitivity(kalmanSensitivity_);
    kalman2.setSensitivity(kalmanSensitivity_);
}

void MainWindow::on_focusThreshold_valueChanged(int value)
{
    ui->label_2->setText("Threshold\n(" + QString::number(value / 100.) + " %)");
    easyDepth.setThreshold(value / 100.);
}

void MainWindow::on_focusEasyChange_valueChanged(int value)
{
    ui->label_3->setText("EasyChange\n(" + QString::number(value / 100.) + ")");
    easyDepth.setChange(value / 100.);
}

void MainWindow::on_focusChangeTime_valueChanged(int value)
{
    ui->label_4->setText("ChangeSpeed\n(" + QString::number(value / 100.) + " m/s)");
    easyDepth.setSpeed(value / 100.);
}

void MainWindow::on_Main_Tab_currentChanged(int index)
{
    if (index != 0 && index != 1)
    {
        ui->leftCam->setVisible(false);
        ui->rightCam->setVisible(false);
    }
    else
    {
        ui->leftCam->setVisible(true);
        ui->rightCam->setVisible(true);
    }

    if (index == 1)
    {
        isCalibrationTab = true;
//        ui->leftCam->move(ui->leftCam->geometry().x(), 63);
//        ui->rightCam->move(ui->rightCam->geometry().x(), 63);
    }
    else
    {
        isCalibrationTab = false;
//        ui->leftCam->move(ui->leftCam->geometry().x(), 80);
//        ui->rightCam->move(ui->rightCam->geometry().x(), 80);
        if (index == 3)
        {
            openCameraWindow();
            std::cout << "Params tab opened" << std::endl;
        }
    }

    if (index != 3) // params
    {
        std::cout << "Process new params" << std::endl;
        setParametersAction();
    }
    else
    {
        loadResolutions();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    calibrationMode = !calibrationMode;
    if (calibrationMode)
    {
        calibrationCollect = false;
        ui->calibrationConfirm->setVisible(true);
        ui->calibrationNext->setVisible(true);
        ui->calibrationPprogressBar->setVisible(true);
        ui->calibrationLabel->setVisible(true);
        ui->calibrationCancel->setVisible(true);
        ui->pushButton_2->setText("Calibrate");
        ui->calibrationLabel->setText(CALIBRATION_STEP_1);
        calibration.startCalibration(resulutionH, MAX_DEPTH);
        std::cout << "Calibration maxY = " << resulutionH << ", maxD = " << MAX_DEPTH << std::endl;
        setArrowLine(15);
        ui->calibrationCancel->setEnabled(true);
    }
    else
    {
        ui->calibrationLabel->setVisible(false);
        ui->calibrationPprogressBar->setVisible(false);
        ui->calibrationCancel->setVisible(false);
        ui->calibrationConfirm->setVisible(false);
        ui->calibrationNext->setVisible(false);
        ui->pushButton_2->setText("Start calibration");
        if (calibration.isCorrect())
        {
            if (ui->useCalibration->isChecked())
            {
                setArrowLine(calibration.getMaxDistance());
            }
            else
            {
                setArrowLine(15);
            }
            calibration.save();
            QMessageBox::information(0, "Information", "Calibration created!");
        }
        else
        {
            on_calibrationCancel_clicked();
            QMessageBox::information(0, "Information", "Not enought distances for create calibration!");
        }
        calibrationCollect = false;
    }
    ui->calibrationConfirm->setEnabled(false);
    ui->calibrationNext->setEnabled(false);
}

void MainWindow::on_calibrationCancel_clicked()
{
    ui->calibrationLabel->setVisible(false);
    ui->calibrationPprogressBar->setVisible(false);
    ui->calibrationConfirm->setVisible(false);
    ui->calibrationNext->setVisible(false);
    ui->calibrationCancel->setVisible(false);
    ui->calibrationConfirm->setEnabled(false);
    ui->calibrationNext->setEnabled(false);
    ui->pushButton_2->setText("Start calibration");
    calibration.load("", cameraResolution_.height);
    if (ui->useCalibration->isChecked())
    {
        setArrowLine(calibration.getMaxDistance());
    }
    else
    {
        setArrowLine(15);
    }
    calibrationCollect = false;
    calibrationMode = false;
    calibrationCollect = false;
}


void MainWindow::on_calibrationConfirm_clicked()
{
    if (tracking)
    {
        ui->calibrationConfirm->setEnabled(false);
        ui->calibrationNext->setEnabled(true);
        calibrationCenterDistance = ui->manualChangerSpin->value();
        ui->calibrationLabel->setText(CALIBRATION_STEP_2);
        calibrationCollect = true;
        calibration.resetProgress();
        ui->calibrationPprogressBar->setValue(0);
        std::cout << "Center depth = " << calibrationCenterDistance << std::endl;
    }
}

void MainWindow::on_calibrationNext_clicked()
{
    calibrationCollect = false;
    ui->calibrationLabel->setText(CALIBRATION_STEP_1);
    ui->calibrationPprogressBar->setValue(0);
}


void MainWindow::on_pushButtonStop_clicked()
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
    ui->loadVideo->setText("Load");
    ui->loadVideo->setStyleSheet("");
    ui->pushButtonStop->setVisible(false);
}


void MainWindow::on_loadVideo_clicked()
{    
    if (isRecording)
    {
        on_pushButton_clicked();
    }

    static QString prevPath;
    QString filename;
    if (ui->loadVideo->text() == "Reload")
    {
        filename = prevPath;
    }
    else
    {
        filename = QFileDialog::getExistingDirectory(this, tr("Open Video Set"), ui->setsPathEdit->text(),
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    }

    if (!filename.isEmpty())
    {
        ui->pushButtonStop->setVisible(true);
        prevPath = filename;
        useVideo = true;
        useStereo = false;
        videoPath = filename.toStdString();
        loadParams(videoPath + "//params.txt");
        saveParams("params.txt");

        loadedRects_.clear();
        int size = 0;
        std::ifstream rectsStream((videoPath + "//rects.txt"));
        if (!rectsStream.is_open())
        {
            std::cout << "Can't load " << saveDir.toStdString() << "/rects.txt";
        }
        else
        {
            rectsStream >> size;
//            std::cout << size << std::endl;
            for (int i = 0; i < size; i++)
            {
                int frameNum;
                cv::Rect rect;
                rectsStream >> frameNum >> rect.x >> rect.y >> rect.width >> rect.height;
                loadedRects_[frameNum] = rect;
//                std::cout << frameNum << "   " << rect << std::endl;
            }
            rectsStream.close();

            ui->loadVideo->setText("Reload");
            ui->loadVideo->setStyleSheet("background-color: green");
        }

        initSource();
        ui->Main_Tab->setCurrentIndex(0);
    }
}



float MainWindow::computeArrowPos(float val)
{
    int newPos = 65 * std::log10(val + 1) / std::log10(arrowScaleParam);
    newPos = std::max(0, newPos);
    newPos = std::min(MAX_ARROW_POS - newPos, MAX_ARROW_POS);
    newPos = std::max(newPos, MIN_ARROW_POS);

    return newPos;
}

void MainWindow::setGreenArrow(float val)
{
    const float ARROW_LABEL_SHIFT = 3;

    int newPos = computeArrowPos(val);

    ui->greenArrow->move(ui->greenArrow->geometry().x(), newPos);
    ui->greenArrowVal->move(ui->greenArrowVal->geometry().x(), ARROW_LABEL_SHIFT + newPos);
    if (val > MAX_DEPTH - 0.01)
    {
        ui->greenArrowVal->setText("far");
    }
    else
    {
        ui->greenArrowVal->setText(getMetric(val));
    }
}

void MainWindow::setRedArrow(float val)
{
    setRightArrow(val, ui->redArrow, ui->redArrowVal);
}

void MainWindow::setRightArrow(float val, QLabel* base, QLabel* text)
{
    const float ARROW_LABEL_SHIFT = 3;

    int newPos = computeArrowPos(val);

    base->move(base->geometry().x(), newPos);
    text->move(text->geometry().x(), ARROW_LABEL_SHIFT + newPos);
    if (val > MAX_DEPTH - 0.01)
    {
        text->setText("far");
    }
    else
    {
        text->setText(getMetric(val));
    }
}


void MainWindow::setArrowLine(float maxValue)
{
    std::cout << "Max arrow value: " << maxValue << std::endl;

    arrowScaleParam = std::pow(10, std::log10(maxValue + 1) / 7);

//    for (int i = 1; i < 8; i++)
//    {
//        std::cout << std::pow(a, i) - 1. << " ";
//    }
    setArrowLine();
}

void MainWindow::setArrowLine()
{
    ui->l1->setText( getMetric( (std::pow(arrowScaleParam, 7) - 1.), true ));
    ui->l2->setText( getMetric( (std::pow(arrowScaleParam, 6) - 1.), true ));
    ui->l3->setText( getMetric( (std::pow(arrowScaleParam, 5) - 1.), true ));
    ui->l4->setText( getMetric( (std::pow(arrowScaleParam, 4) - 1.), true ));
    ui->l5->setText( getMetric( (std::pow(arrowScaleParam, 3) - 1.), true ));
    ui->l6->setText( getMetric( (std::pow(arrowScaleParam, 2) - 1.), true ));
    ui->l7->setText( getMetric( (std::pow(arrowScaleParam, 1) - 1.), true ));


    for (int i = 0; i < arrowLabels.size(); i++)
    {
        ArrowLabel& label = arrowLabels[i];
        setRightArrow(label.value, label.label, label.labelVal);
    }
}


void MainWindow::saveCameras(std::string path)
{
    std::ofstream file(path.c_str());
    file << ui->cam1->value() << " " << ui->cam2->value() << std::endl;
    file.close();
}

std::vector<cv::Size> MainWindow::getResolutions()
{
    static std::vector<cv::Size> cameraResolutions;

    if ((prevCam1 == camera || prevCam2 == camera
        || prevCam1 == camera2 || prevCam2 == camera2)
        && cameraResolutions.size())
    {
        return cameraResolutions;
    }

    std::cout << "Loading camera resolutions" << std::endl;

    prevCam1 = camera;
    prevCam2 = camera2;

    const int NUM_RES = 12;
    const cv::Size RES[NUM_RES] = {
        cv::Size(160, 120),
        cv::Size(176, 144),
        cv::Size(320, 240),
        cv::Size(352, 240),
        cv::Size(640, 480),
        cv::Size(800, 600),
        cv::Size(854, 480),
        cv::Size(1024, 576),
        cv::Size(1024, 768),
        cv::Size(1280, 720),
        cv::Size(1280, 1024),
        cv::Size(1920, 1080)
    };

    cv::VideoCapture camera(prevCam1);
    if (!camera.isOpened())
    {
        return cameraResolutions;
    }
    cameraResolutions.clear();
    for (int i = 0; i < NUM_RES; i++)
    {
//        camera.set(CV_CAP_PROP_FRAME_WIDTH, RES[i].width);
//        camera.set(CV_CAP_PROP_FRAME_HEIGHT, RES[i].height);

//        double width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
//        double height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);

//        if (RES[i].width == width && RES[i].height == height)
        {
//            std::cout << "Possible camera resolution: " << width << " x " << height << std::endl;

            std::cout << "MUST RESTORE CAMERAS SCANING!!!" << std::endl;

            cameraResolutions.push_back(RES[i]);
        }
//        else
//        {
//            std::cout << "Not possible camera resolution: " << RES[i].width << " x " << RES[i].height << std::endl;
//        }
    }

    return cameraResolutions;
}

void MainWindow::loadCameras()
{
    std::ifstream file("cameras.txt");
    if (file.is_open())
    {
        file >> camera >> camera2;

        ui->cam1->setValue(camera);
        ui->cam2->setValue(camera2);

        file.close();
    }
    getResolutions();
}


void MainWindow::on_pushButtonStereo_clicked()
{
    useStereo = true;
    camera = ui->cam1->value();
    camera2 = ui->cam2->value();
    saveCameras("cameras.txt");
    loadResolutions();
    initSource();
    ui->Main_Tab->setCurrentIndex(0);
}

void MainWindow::on_resolutions_currentIndexChanged(int)
{
    if (!addResolutions)
    {
        setParamsValues();
    }
}

void MainWindow::setParamsValues()
{
    params.a1 = ui->valA_3->value();
    params.b1 = ui->valB_3->value();
    params.c1 = ui->valC_3->value();
    params.d1 = ui->valD_3->value();
    params.e1 = ui->valE_3->value();
    params.f1 = ui->valF_3->value();

    params.a2 = ui->valA->value();
    params.b2 = ui->valB->value();
    params.c2 = ui->valC->value();
    params.d2 = ui->valD->value();
    params.e2 = ui->valE->value();
    params.f2 = ui->valF->value();

    params.base = ui->valBase->value();
    params.cam1Ang = ui->cam1Ang->value();
    params.cam2Ang = ui->cam2Ang->value();
    params.focalLength = ui->valFocalLen->value();
    params.HFOV = ui->HFOV->value();
    params.VFOV = ui->VFOV->value();

    params.useDisparity = ui->useDisparity->isChecked();
    params.useTriangulation = ui->useTriangulation->isChecked();
    params.useCorrection1 = ui->useCorrection_1->isChecked();
    params.useCorrection2 = ui->useCorrection_2->isChecked();
    params.useCamCorrection = ui->useCamCorrection->isChecked();
    params.useFPFix = ui->useFPFix->isChecked();
    params.metric = ui->metric->isChecked();

    params.corrA = ui->camCorr_A->value();
    params.corrB = ui->camCorr_B->value();
    params.corrC = ui->camCorr_C->value();
    params.corrKdop = ui->camCorr_Kdop->value();

    params.useCalibration = ui->useCalibration->isChecked();

    if (calibration.isCorrect() && ui->useCalibration->isChecked())
    {
        setArrowLine(calibration.getMaxDistance());
    }
    else
    {
        setArrowLine(15.);
    }


    if (resolutions.size())
    {
        params.cameraWidth = resolutions[ui->resolutions->currentIndex()].width;
        params.cameraHeight = resolutions[ui->resolutions->currentIndex()].height;
//        std::cout << ">>>> " <<__LINE__ << std::endl;
//        std::cout << params.cameraWidth << std::endl;
    }
}

void MainWindow::setParamsCamRes()
{
//    std::cout << ">>>> " <<__LINE__ << std::endl;
//    std::cout << params.cameraWidth << "   " << resolutions.size() << std::endl;
    for (unsigned int i = 0; i < resolutions.size(); i++)
    {
        if (resolutions[i].width == params.cameraWidth
                && resolutions[i].height == params.cameraHeight)
        {
            ui->resolutions->setCurrentIndex(i);
//            std::cout << i << std::endl;
            break;
        }
    }
}

void MainWindow::saveParams(std::string path)
{
    std::ofstream file(path.c_str());

    file << params.a1 << " " << params.b1 << " " << params.c1 << " " << params.d1 << " " << params.e1 << " " << params.f1 << std::endl;
    file << params.a2 << " " << params.b2 << " " << params.c2 << " " << params.d2 << " " << params.e2 << " " << params.f2 << std::endl;

    file << params.base << std::endl;
    file << params.cam1Ang << " " << params.cam2Ang << std::endl;
    file << params.focalLength << std::endl;
    file << params.HFOV << " " << params.VFOV << std::endl;

    file << params.useDisparity << std::endl;
    file << params.useTriangulation << std::endl;
    file << params.useCorrection1 << std::endl;
    file << params.useCorrection2 << std::endl;
    file << params.useCamCorrection << std::endl;
    file << params.useFPFix << std::endl;
    file << params.metric << std::endl;

    file << params.cameraWidth << std::endl;
    file << params.cameraHeight << std::endl;

    file << params.corrA << std::endl;
    file << params.corrB << std::endl;
    file << params.corrC << std::endl;
    file << params.corrKdop << std::endl;

    file << params.useCalibration << std::endl;

    file << ui->setsPathEdit->text().toStdString() << std::endl;
    file << ui->libraryPathEdit->text().toStdString() << std::endl;

//    std::cout << ">>>> " <<__LINE__ << std::endl;
//    std::cout << params.cameraWidth << std::endl;

    file.close();
}

void MainWindow::loadParams(std::string path)
{
    std::cout << "Loading " << path << std::endl;
    std::ifstream file(path);
    if (file.is_open())
    {
        file >> params.a1 >> params.b1 >> params.c1 >> params.d1 >> params.e1 >> params.f1;
        file >> params.a2 >> params.b2 >> params.c2 >> params.d2 >> params.e2 >> params.f2;

        file >> params.base;
        file >> params.cam1Ang >> params.cam2Ang;
        file >> params.focalLength;
        file >> params.HFOV >> params.VFOV;

        file >> params.useDisparity;
        file >> params.useTriangulation;
        file >> params.useCorrection1;
        file >> params.useCorrection2;
        file >> params.useCamCorrection;
        file >> params.useFPFix;
        file >> params.metric;

        file >> params.cameraWidth;
        file >> params.cameraHeight;

        file >> params.corrA;
        file >> params.corrB;
        file >> params.corrC;
        file >> params.corrKdop;

        file >> params.useCalibration;

        char setsPath[10000], libraryPath[10000], t[10];
        file.getline(t, 10);
        file.getline(setsPath, 10000);
        file.getline(libraryPath, 10000);
        ui->setsPathEdit->setText(setsPath);
        ui->libraryPathEdit->setText(libraryPath);

        setParamsCamRes();

        ui->valA_3->setValue(params.a1);
        ui->valB_3->setValue(params.b1);
        ui->valC_3->setValue(params.c1);
        ui->valD_3->setValue(params.d1);
        ui->valE_3->setValue(params.e1);
        ui->valF_3->setValue(params.f1);

        ui->valA->setValue(params.a2);
        ui->valB->setValue(params.b2);
        ui->valC->setValue(params.c2);
        ui->valD->setValue(params.d2);
        ui->valE->setValue(params.e2);
        ui->valF->setValue(params.f2);

        ui->valBase->setValue(params.base);
        ui->cam1Ang->setValue(params.cam1Ang);
        ui->cam2Ang->setValue(params.cam2Ang );
        ui->valFocalLen->setValue(params.focalLength);
        ui->HFOV->setValue(params.HFOV);
        ui->VFOV->setValue(params.VFOV);

        ui->useDisparity->setChecked(params.useDisparity);
        ui->useTriangulation->setChecked(params.useTriangulation);
        ui->useCorrection_1->setChecked(params.useCorrection1);
        ui->useCorrection_2->setChecked(params.useCorrection2);
        ui->useCamCorrection->setChecked(params.useCamCorrection);
        ui->useFPFix->setChecked(params.useFPFix);
        ui->metric->setChecked(params.metric);

        ui->camCorr_A->setValue(params.corrA);
        ui->camCorr_B->setValue(params.corrB);
        ui->camCorr_C->setValue(params.corrC);
        ui->camCorr_Kdop->setValue(params.corrKdop);

        ui->useCalibration->setChecked(params.useCalibration);

        if (calibration.isCorrect() && ui->useCalibration->isChecked())
        {
            setArrowLine(calibration.getMaxDistance());
        }
        else
        {
            setArrowLine(15.);
        }

        file.close();
    }
    else
    {
        params.cameraWidth = 640;
        params.cameraHeight = 480;
        setParamsCamRes();
    }
    setParamsValues();
}

void MainWindow::setCamResolutions(const std::vector<cv::Size> &res)
{
    resolutions = res;
//    std::cout << ">>>" << __LINE__ << std::endl;
//    std::cout << resolutions.size() << std::endl;

    addResolutions = true;
    ui->resolutions->clear();
    for (unsigned int i = 0; i < resolutions.size(); i++)
    {
        ui->resolutions->addItem(QString::number(resolutions[i].width) + "x" + QString::number(resolutions[i].height));
    }
    addResolutions = false;
    setParamsCamRes();
}


void MainWindow::on_pauseButton_clicked()
{
    if (ui->pauseButton->text() == "Pause")
    {
        ui->pauseButton->setText("Play");
        ui->pauseButton->setStyleSheet("background-color: green");
    }
    else
    {
        ui->pauseButton->setText("Pause");
        ui->pauseButton->setStyleSheet("");
    }
    frameReader.pause(!frameReader.isPause());
}
