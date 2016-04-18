#include <QApplication>
#include <QApplication>
#include <mainwindow.h>

#include "selector.h"
#include "framereader.h"

#include "trackerAPI.h"

#include "timer.h"
#include "fpscounter.h"

#include <iostream>


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


int main(int argc, char** argv)
{
#ifdef WINDOWS
    // for fix bug with demo runing on windows
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    QCoreApplication::setLibraryPaths(paths);
#endif
    // contains selected source
    bool useCamera = false, useFile = false, useStereo = false;
    int camera = 0, camera2 = 0;
    std::string videoPath;

    // run dialog for select video source
    QApplication app(argc, argv);
    MainWindow* window = new MainWindow();
    window->init(&camera, &camera2, &videoPath,
                 &useCamera, &useFile, &useStereo);
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    window->show();
    app.exec();

    // opencv frame reader
    FrameReader frameReader;
    bool needPause = false;
    if (useCamera)
    {
        std::cout << "use camera" << std::endl;
        frameReader.initCamera(camera);
    }
    else if (useFile)
    {
        std::cout << "use video: " << videoPath << std::endl;
        frameReader.initVideo(videoPath);
        needPause = true;
    }
    else if (useStereo)
    {
        std::cout << "use stereo" << std::endl;
        frameReader.initStereo(camera, camera2);
    }
    else
    {
        return 0;
    }

    // tracker engine
    bool tracking = false;
    TrackerAPI tracker;
    cv::Rect resultL, resultR;
    while (true)
    {
        cv::Mat leftFrame, rightFrame;
        long long time = 0;
        if (!frameReader.getFrame(leftFrame, rightFrame))
        {
            break;
        }

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

        // show windows with videos
        const char* WINDOW_NAME_L = "Video_LEFT";
        cv::imshow(WINDOW_NAME_L, leftFrame);
        cv::imshow("Video_RIGHT", rightFrame);
        cv::setMouseCallback(WINDOW_NAME_L, mouseCallBack);
        char key = cv::waitKey(30);
        if (key == ' ')
        {
            frameReader.pause(!frameReader.isPause());
        }
        else if (key == 27 || key == 'q' || key == 'Q')
        {
            break;
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
}
