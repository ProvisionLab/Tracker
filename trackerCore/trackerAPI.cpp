#include "trackerAPI.h"


TrackerAPI::TrackerAPI(bool hog, bool lab, bool multiScale) :
    hog_(hog),
    lab_(lab),
    multiScale_(multiScale)
{
    tracker_ = new KCFTracker(hog, !multiScale, multiScale, lab);
}

TrackerAPI::~TrackerAPI()
{
    delete tracker_;
}

void TrackerAPI::init(const cv::Mat& left, const cv::Rect& rect)
{
    tracker_->init(rect, left);
}

void TrackerAPI::detect(const cv::Mat& left, const cv::Mat& right,
                        cv::Rect& resultL, cv::Rect& resultR)
{
    resultL = tracker_->update(left);
    detectStereo(left, right, resultL, resultR);
}

void TrackerAPI::reset()
{
    delete tracker_;
    tracker_ = new KCFTracker(hog_, !multiScale_, multiScale_, lab_);
}
