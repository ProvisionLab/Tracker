#ifndef POSMATCHER_H
#define POSMATCHER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>

const int NUM_FEATURE_POINTS = 5;

class PosMatcher
{
public:
    PosMatcher();

    bool match(const cv::Mat& left, const cv::Mat& right,
               const cv::Rect& lectRoi, const cv::Rect& rightRoi,
               std::vector<std::pair<cv::Point2f, cv::Point2f> > &pos, bool debug = false);

private:
    cv::Ptr<cv::FeatureDetector> detector;
    cv::Ptr<cv::DescriptorExtractor> extractor;
    cv::FlannBasedMatcher matcher;
};


#endif // POSMATCHER_H
