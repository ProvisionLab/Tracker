#include "framereader.h"
#include <iostream>

FrameReader::FrameReader() :
    videoCapture_(0),
    pause_(false),
    frameNum_(0)
{
}

void FrameReader::initCamera(int cameraNumber)
{
    videoCapture_.open(cameraNumber);
    check();
}

void FrameReader::initVideo(std::string videoPath)
{
    videoCapture_.open(videoPath);
    check();
}

void FrameReader::initStereo(int cam1, int cam2)
{
    videoCapture_.open(cam1);
    check();

    videoCapture2_.open(cam2);
    if(!videoCapture2_.isOpened()) // check if we succeeded
    {
        std::cerr << "Can't open stream form camera #2" << std::endl;
        exit(-1);
    }
}

bool FrameReader::getFrame(cv::Mat& left, cv::Mat& right)
{
    if (!pause_ || leftFrame_.empty())
    {

        videoCapture_ >> leftFrame_;
        frameNum_++;
    }
    if (leftFrame_.empty())
    {

        return false;
    }
    cv::flip(leftFrame_, left, 1);

    if (videoCapture2_.isOpened())
    {
        videoCapture_ >> rightFrame_;
        if (rightFrame_.empty())
        {
            return false;
        }
        cv::flip(rightFrame_, right, 1);
    }
    else
    {
        rightVideoGenerator.update(left, rightFrame_);
        rightFrame_.copyTo(right);
    }
    return true;
}

long FrameReader::getFrameNum()
{
    return frameNum_;
}

long FrameReader::getTotalNumFrames() const
{
    return videoCapture_.get(cv::CAP_PROP_FRAME_COUNT);
}

void FrameReader::pause(bool pause)
{
    pause_ = pause;
}

bool FrameReader::isPause() const
{
    return pause_;
}

void FrameReader::check()
{
    if(!videoCapture_.isOpened()) // check if we succeeded
    {
        std::cerr << "Can't open stream" << std::endl;
        exit(-1);
    }
    std::cout << "Video opened" << std::endl;
}
