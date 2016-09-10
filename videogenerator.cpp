#include "videogenerator.h"

VideoGenerator::VideoGenerator() :
    iterationNumber_(0)
{

}

void VideoGenerator::update(const cv::Mat& source, cv::Mat& target)
{
    iterationNumber_++;
    if (prevMat_.empty())
    {
        source.copyTo(target);
    }
    else
    {
        target = cv::Mat::zeros(source.size(), source.type());

        int maxShiftX = std::min(0, source.cols / 4);
        int shiftX = maxShiftX * (sin(iterationNumber_ / 100.) + 1.) / 2;

        int maxShiftY = std::min(0, source.rows / 20);
        int shiftY = maxShiftY * (sin(iterationNumber_ / 200.) + 1.) / 2;

        cv::Rect sourceRect(0, shiftY, source.cols - shiftX, source.rows - shiftY);
        cv::Rect targetRect(cv::Rect(shiftX, 0, source.cols - shiftX, source.rows - shiftY));

        source(sourceRect).copyTo(target(targetRect));
    }

    source.copyTo(prevMat_);
}
