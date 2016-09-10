#ifndef DETECTORAPI_H
#define DETECTORAPI_H

#include "kcftracker.hpp"
#include "rdetector.h"

class DetectorAPI
{
public:
    /**
     * @brief TrackerAPI constructor
     * @param hog - use HOG features in tracker
     * @param lab - use LAB features in tracker
     * @param fixedWindow - use tracker scaling or not
     */
    DetectorAPI(bool hog = true, bool lab = true, bool fixedWindow = true);

    ~DetectorAPI();

    /**
     * @brief detect tracker on next frame and project tracker from left frame on right frame
     * @param left - left camera image
     * @param right - right camera image
     * @param resultL - tracker on left image
     * @param resultR - tracker on right image
     */
    void detect(const cv::Mat& left, const cv::Mat& right,
                      cv::Rect2f &resultL, cv::Rect2f &resultR);

    float getAccuracy() const;

    /**
     * @brief reset tracker
     */
    void reset();

private:
    const bool hog_;
    const bool lab_;
    const bool multiScale_;

    float accuracy_;

    KCFTracker* detector_;
};

#endif // DETECTORAPI_H
