#ifndef VIDEOGENERATOR_H
#define VIDEOGENERATOR_H

#include <opencv2/core.hpp>

class VideoGenerator
{
public:
    VideoGenerator();

    void update(const cv::Mat& source, cv::Mat& target);

private:
    cv::Mat prevMat_;
    int iterationNumber_;
};

#endif // VIDEOGENERATOR_H
