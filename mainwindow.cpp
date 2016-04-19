#include "mainwindow.h"


// tracker parameters
const bool HOG = true;
const bool LAB = true;
const bool FIXEDWINDOW = false;

// additional tools for testing
FpsCounter fps;
Selector mouseSelector;

static void mouseCallBack(int event, int x, int y, int, void*)
{
    if( event == cv::EVENT_LBUTTONDOWN )
    {
        mouseSelector.setStart(x, y);
    }
    if( event == cv::EVENT_LBUTTONUP )
    {
        mouseSelector.setEnd(x, y, true);
    }
    if (mouseSelector.isSelecting())
    {
        mouseSelector.setEnd(x, y);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::init(int *camera, int *camera2, std::string *videoFile,
                      bool *useCamera, bool *useFile, bool* useStereo, Ui_MainWindow** uiWindow)
{
    camera_ = camera;
    camera2_ = camera2;
    videoFile_ = videoFile;
    useCamera_ = useCamera;
    useFile_ = useFile;
    useStereo_ = useStereo;
    *useCamera_ = *useFile_ = *useStereo_ = false;
    *uiWindow = (this->ui);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(DisplayImage()));
    Timer->setInterval(15); //60fps
    Timer->start(); //Will


}

void MainWindow::DisplayImage(){

        cv::Mat leftFrame, rightFrame;
        long long time = 0;
        if (!frameReader.getFrame(leftFrame, rightFrame))
        {
            return;
        }

        frameReader.pause((false));

        if (needPause && frameReader.getFrameNum() == 5)
        {
            frameReader.pause(true);
        }


        if (tracking)
        {
            if (!frameReader.isPause())
            {
                time = getTimeMs();
                tracker.detect(leftFrame, rightFrame, resultL, resultR);
                time = getTimeMs() - time;
            }
            // draw tracker result
            cv::rectangle(leftFrame, resultL, cv::Scalar(0, 255, 0), 1, 8);
        }
        else if (mouseSelector.isSelected())
        {
            tracker.init(leftFrame, mouseSelector.getRect());
            if (useFile)
            {
                frameReader.pause(false);
                needPause = false;
            }
            tracking = true;
        }
        else if (mouseSelector.isSelecting())
        {
            mouseSelector.draw(leftFrame);
        }

        const int NUM_TEST_LINES = 6;
        std::stringstream s[NUM_TEST_LINES];
        s[0] << "Frame: " << frameReader.getFrameNum();
        if (frameReader.getTotalNumFrames() > 0)
        {
            s[0] << " / " << frameReader.getTotalNumFrames();
        }
        s[1] << "Tracker time: " << time << " ms.";
        s[2] << "Camera Fps: " << fps.updateFps();
        s[3] << "Press 'Esc' or 'q' for exit.";
        s[4] << "Press ' ' (space for pause).";
        s[5] << "Press 'r' for reset tracker.";
        for(int i = 0; i < NUM_TEST_LINES; i++)
        {
            cv::putText(leftFrame, s[i].str() , cvPoint(30, 30 * (i + 1)),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 0, 0), 1, CV_AA);
        }

        // draw object ROI on right image
        if (tracking)
        {
            // draw left image result on right image
            cv::rectangle(rightFrame, resultL, cv::Scalar(255, 0, 0));
            // draw ROI
            cv::rectangle(rightFrame, getStereoRoi(leftFrame, resultL), cv::Scalar(0, 0, 0));
            // draw right image result
            cv::rectangle(rightFrame, resultR, cv::Scalar(0, 255, 0));
        }


        cv::resize(leftFrame, leftFrame, cv::Size(315, 205), 0, 0, cv::INTER_LINEAR);
        cv::cvtColor(leftFrame,leftFrame,CV_BGR2RGB); //Qt reads in RGB whereas CV in BGR


        QImage imdisplay((uchar*)leftFrame.data, leftFrame.cols, leftFrame.rows, leftFrame.step, QImage::Format_RGB888);
        QPixmap pxMap = QPixmap::fromImage(imdisplay);
        ui->lblLeft->setPixmap(pxMap);


        QImage imdisplay2((uchar*)rightFrame.data, rightFrame.cols, rightFrame.rows, rightFrame.step, QImage::Format_RGB888);
         ui->lblRight->setPixmap(QPixmap::fromImage(imdisplay2));

        //cv::setMouseCallback(WINDOW_NAME_L, mouseCallBack);
        char key = cv::waitKey(30);
        if (key == ' ')
        {
            frameReader.pause(!frameReader.isPause());
        }
        else if (key == 27 || key == 'q' || key == 'Q')
        {
            return;
        }
        else if (key == 'r' || key == 'R')
        {
            mouseSelector.reset();
            tracking = false;
            tracker.reset();
            if (useFile)
            {
                frameReader.pause(true);
            }
        }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_video_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Video"), ".",
                                                    tr("Video Files (*.avi *.mpg *.mp4 *.mov)"));
    if (!filename.isEmpty())
    {
        *useFile_ = true;
        *videoFile_ = filename.toStdString();
        close();
    }

    bool needPause = true;
    if (useFile)
    {
        std::cout << "use video: " << *videoFile_ << std::endl;
        frameReader.initVideo(*videoFile_);
        needPause = true;
    }
    else
    {
        return;
    }

    needPause = false;
}

void MainWindow::on_pushButtonCamera_clicked()
{
    *useCamera_ = true;
    //*camera_ = ui->cameraNumber->value();
    close();
}

void MainWindow::on_pushButtonStereo_clicked()
{
    *useStereo_ = true;
    //*camera_ = ui->cam1->value();
    //*camera_ = ui->cam2->value();
    close();
}

void MainWindow::on_btnStart_clicked()
{

}

void MainWindow::on_btnFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Video"), ".",
                                                    tr("Video Files (*.avi *.mpg *.mp4 *.mov)"));
    if (!filename.isEmpty())
    {
        useFile = true;
        *videoFile_ = filename.toStdString();
        //close();
    }

    bool needPause = true;
    if (useFile)
    {
        std::cout << "use video: " << *videoFile_ << std::endl;
        frameReader.initVideo(*videoFile_);
        needPause = true;
    }
    else
    {
        return;
    }

    needPause = false;
}

void MainWindow::on_btnStop_clicked()
{

}
