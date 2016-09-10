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
    accuracy_ = 1.f;
}

void TrackerAPI::stopTrain(bool needStop)
{
    tracker_->stopTrain(needStop);
}

void TrackerAPI::detect(const cv::Mat& left, cv::Rect2f& resultL, int x, int y, int w)
{
    if (x >= 0 && y >= 0)
    {
        resultL = tracker_->updateZoom(left, x, y, 1.25, 3);
    }
    else if (w > 0)
    {
        resultL = tracker_->update(left, w);
    }
    else
    {
        resultL = tracker_->update(left);
    }

    float currentAccuracy = tracker_->getAccuracy();
    if (accuracy_ > currentAccuracy)
    {
        accuracy_ = currentAccuracy;
    }
    else
    {
        accuracy_ = 0.98 * accuracy_ + 0.02 * currentAccuracy;
    }

//    posX[numFrames % HISTORY_SIZE] = resultL.x;
//    posY[numFrames % HISTORY_SIZE] = resultL.y;
//    sizes[numFrames % HISTORY_SIZE] = resultL.width;
//    numFrames++;

//    float trammor = 0.f;
//    int totalShift = 0.f;
//    float avgX = posX[0];
//    float avgY = posY[0];
//    float avgS = sizes[0];
//    for (int i = 1; i < std::min(HISTORY_SIZE, numFrames); i += 1)
//    {
//        avgX = 0.9 * avgX + 0.1 * posX[i];
//        avgY = 0.9 * avgY + 0.1 * posY[i];
//        avgS = 0.9 * avgS + 0.1 * sizes[i];

//        trammor += fabs(avgX - posX[i]);
//        trammor += fabs(avgY - posY[i]);
//        trammor += fabs(avgS - sizes[i]);

//        totalShift += fabs(posX[i-1] - posX[i]);
//        totalShift += fabs(posY[i-1] - posY[i]);
//        totalShift += fabs(sizes[i-1] - sizes[i]);
//    }
//    trammor /= totalShift;

    //    std::cout << currentAccuracy << " " << accuracy_ << "   " << 1. / trammor << std::endl;
}

void TrackerAPI::setRect(const cv::Rect2f &rect)
{
    tracker_->setRect(rect);
}

float TrackerAPI::getAccuracy() const
{
    return accuracy_;
}

void TrackerAPI::reset()
{
    delete tracker_;
    tracker_ = new KCFTracker(hog_, !multiScale_, multiScale_, lab_);
}
