#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "videogenerator.h"

class FrameReader
{
public:
    FrameReader();

    void initCamera(int cameraNumber);
    void initVideo(std::string videoPath);
    void initStereo(int cam1, int cam2);

    bool getFrame(cv::Mat& left, cv::Mat& right);
    long getFrameNum();
    long getTotalNumFrames() const;

    void pause(bool pause);
    bool isPause() const;

private:
    void check();

private:
    cv::VideoCapture videoCapture_;
    cv::VideoCapture videoCapture2_;
    VideoGenerator rightVideoGenerator;
    cv::Mat leftFrame_, rightFrame_;
    bool pause_;
    long frameNum_;
};

#endif // FRAMEREADER_H
