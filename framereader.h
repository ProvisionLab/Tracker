#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "videogenerator.h"

class FrameReader
{
public:
    FrameReader();

    std::string initVideo(std::string videoPath);
    std::string initStereo(int cam1, int cam2);

    void setResolution(const cv::Size& resolution);

    void close();

    bool getFrame(cv::Mat& left, cv::Mat& right);
    long getFrameNum();
    long getTotalNumFrames() const;

    void pause(bool pause);
    bool isPause() const;
    bool isFinish() const;

private:
    cv::Size cameraResolution_;
    cv::VideoCapture videoCapture_;
    cv::VideoCapture videoCapture2_;
    VideoGenerator rightVideoGenerator;
    cv::Mat leftFrame_, rightFrame_;
    bool pause_;
    long frameNum_;
    bool isFinish_;
    int activeCam1, activeCam2;
};

#endif // FRAMEREADER_H
