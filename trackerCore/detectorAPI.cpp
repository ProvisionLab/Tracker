#include "detectorAPI.h"

DetectorAPI::DetectorAPI(bool hog, bool lab, bool multiScale) :
    hog_(hog),
    lab_(lab),
    multiScale_(multiScale)
{
    detector_ = new KCFTracker(hog, !multiScale, multiScale, lab);
}

DetectorAPI::~DetectorAPI()
{
    delete detector_;
}

void DetectorAPI::detect(const cv::Mat& left, const cv::Mat& right,
                        cv::Rect2f& resultL, cv::Rect2f& resultR)
{
    //detectStereo(left, right, resultL, resultR);
    detector_->init(resultL, left);
    resultR = detector_->detectAll(right, resultL, 1.1, 0);
    accuracy_ = detector_->getAccuracy();
}

float DetectorAPI::getAccuracy() const
{
    return accuracy_;
}

void DetectorAPI::reset()
{
    delete detector_;
    detector_ = new KCFTracker(hog_, !multiScale_, multiScale_, lab_);
}
