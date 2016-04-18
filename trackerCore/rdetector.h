#ifndef RDETECTOR_H
#define RDETECTOR_H

#include <opencv2/core.hpp>

cv::Rect getStereoRoi(const cv::Mat &frame, const cv::Rect& rect);

void detectStereo(const cv::Mat& left, const cv::Mat& right,
            const cv::Rect& leftRect, cv::Rect& rightRect);

#endif // RDETECTOR_H
