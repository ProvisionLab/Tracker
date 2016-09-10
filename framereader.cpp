#include "framereader.h"
#include <iostream>

FrameReader::FrameReader() :
    videoCapture_(0),
    pause_(false),
    frameNum_(0),
    isFinish_(false),
    activeCam1(-1),
    activeCam2(-1)
{
}

std::string FrameReader::initVideo(std::string videoPath)
{
    activeCam1 = activeCam2 = -1;
    isFinish_ = false;
    videoCapture_.open(videoPath + "/left.avi");
    if(!videoCapture_.isOpened()) // check if we succeeded
    {
        return "Set " + videoPath + " is incorrect.\nCan't open "
                + videoPath + "/left.avi";
    }
    videoCapture2_.open(videoPath + "/right.avi");
    if(!videoCapture2_.isOpened()) // check if we succeeded
    {
        return "Set " + videoPath + " is incorrect.\nCan't open "
                + videoPath + "/right.avi";
    }
    return "";
}

std::string FrameReader::initStereo(int cam1, int cam2)
{
    std::cout << "init stereo" << std::endl;

    isFinish_ = false;
    videoCapture_.open(cam1);
    if(!videoCapture_.isOpened()) // check if we succeeded
    {
        return "Can't open stream form camera #2";
    }

    videoCapture2_.open(cam2);
    if(!videoCapture2_.isOpened()) // check if we succeeded
    {
        return "Can't open stream form camera #1";
    }

    activeCam1 = cam1;
    activeCam2 = cam2;

    videoCapture_.set(CV_CAP_PROP_FRAME_WIDTH, cameraResolution_.width);
    videoCapture_.set(CV_CAP_PROP_FRAME_HEIGHT, cameraResolution_.height);
    videoCapture2_.set(CV_CAP_PROP_FRAME_WIDTH, cameraResolution_.width);
    videoCapture2_.set(CV_CAP_PROP_FRAME_HEIGHT, cameraResolution_.height);

    return "";
}

void FrameReader::setResolution(const cv::Size& resolution)
{
    std::cout << "Set new resolution: " << resolution << std::endl;
    cameraResolution_ = resolution;
    if (activeCam1 >= 0 && activeCam2 >= 0)
    {
        close();
        initStereo(activeCam1, activeCam2);
    }
}

void FrameReader::close()
{
    if (videoCapture_.isOpened())
    {
        videoCapture_.release();
    }
    if (videoCapture2_.isOpened())
    {
        videoCapture2_.release();
    }
    frameNum_ = 0;
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
        isFinish_ = true;
        return false;
    }
    //cv::flip(leftFrame_, left, 1);
    leftFrame_.copyTo(left);

    if (videoCapture2_.isOpened())
    {
        if (!pause_ || rightFrame_.empty())
        {
            videoCapture2_ >> rightFrame_;
        }
        if (rightFrame_.empty())
        {
            return false;
        }
        //cv::flip(rightFrame_, right, 1);
        rightFrame_.copyTo(right);
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

bool FrameReader::isFinish() const
{
    return isFinish_;
}
