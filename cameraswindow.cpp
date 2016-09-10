#include "cameraswindow.h"
#include "ui_cameras.h"
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

CameraWindow::CameraWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraWindow),
    prevCam1(-1),
    prevCam2(-1)
{
    ui->setupUi(this);
}

void CameraWindow::init(int *camera, int *camera2, bool* useStereo)
{
    camera_ = camera;
    camera2_ = camera2;
    useStereo_ = useStereo;
    *useStereo_ = false;
    loadCameras();
}

CameraWindow::~CameraWindow()
{
    delete ui;
}

void CameraWindow::closeEvent(QCloseEvent *)
{
    saveCameras();
    emit closed();
}

void CameraWindow::on_pushButtonStereo_clicked()
{
    *useStereo_ = true;
    *camera_ = ui->cam1->value();
    *camera2_ = ui->cam2->value();
    saveCameras();
    hide();
    emit closed();
}

void CameraWindow::saveCameras(std::string path)
{
    std::ofstream file(path.c_str());
    file << ui->cam1->value() << " " << ui->cam2->value() << std::endl;
    file.close();
}

std::vector<cv::Size> CameraWindow::getResolutions()
{
    if ((prevCam1 == *camera_ || prevCam2 == *camera_
        || prevCam1 == *camera2_ || prevCam2 == *camera2_)
        && cameraResolutions.size())
    {
        return cameraResolutions;
    }

    std::cout << "Loading camera resolutions" << std::endl;

    prevCam1 = *camera_;
    prevCam2 = *camera2_;

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

void CameraWindow::loadCameras()
{
    std::ifstream file("cameras.txt");
    if (file.is_open())
    {
        file >> *camera_ >> *camera2_;

        ui->cam1->setValue(*camera_);
        ui->cam2->setValue(*camera2_);

        file.close();
    }
    getResolutions();
}
