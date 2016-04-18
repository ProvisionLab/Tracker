#ifndef TRACKERAPI_H
#define TRACKERAPI_H

#include "kcftracker.hpp"
#include "rdetector.h"

class TrackerAPI
{
public:
    /**
     * @brief TrackerAPI constructor
     * @param hog - use HOG features in tracker
     * @param lab - use LAB features in tracker
     * @param fixedWindow - use tracker scaling or not
     */
    TrackerAPI(bool hog = true, bool lab = true, bool fixedWindow = true);

    ~TrackerAPI();

    /**
     * @brief init start tracker
     * @param left - left camera image
     * @param rect - tracker position
     */
    void init(const cv::Mat& left, const cv::Rect& rect);

    /**
     * @brief detect tracker on next frame and project tracker from left frame on right frame
     * @param left - left camera image
     * @param right - right camera image
     * @param resultL - tracker on left image
     * @param resultR - tracker on right image
     */
    void detect(const cv::Mat& left, const cv::Mat& right,
                cv::Rect& resultL, cv::Rect& resultR);

    /**
     * @brief reset tracker
     */
    void reset();

private:
    const bool hog_;
    const bool lab_;
    const bool multiScale_;

    KCFTracker* tracker_;
};

#endif // TRACKERAPI_H
