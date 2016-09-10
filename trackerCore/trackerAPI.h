#ifndef TRACKERAPI_H
#define TRACKERAPI_H

#include "kcftracker.hpp"
#include "rdetector.h"

const int HISTORY_SIZE = 30;

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

    void stopTrain(bool needStop);

    /**
     * @brief detect tracker on next frame and project tracker from left frame on right frame
     * @param left - left camera image
     * @param right - right camera image
     * @param resultL - tracker on left image
     * @param resultR - tracker on right image
     */
    void detect(const cv::Mat& left, cv::Rect2f &resultL, int x = -1, int y = -1, int w = -1);

    void setRect(const cv::Rect2f& rect);

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
//    int sizes[HISTORY_SIZE];
//    int posX[HISTORY_SIZE];
//    int posY[HISTORY_SIZE];
//    int numFrames;

    KCFTracker* tracker_;
};

#endif // TRACKERAPI_H
