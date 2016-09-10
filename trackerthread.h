#ifndef TRACKERTHREAD_H
#define TRACKERTHREAD_H

#include <QThread>
#include "trackerAPI.h"

class TrackerThread : public QThread
{
public:
    TrackerThread();

    void set(TrackerAPI* tracker, cv::Mat* image, cv::Rect2f *result, int* w);
    void run();

private:
    TrackerAPI* tracker;
    cv::Mat* image;
    cv::Rect2f* result;
    int* w;
};

#endif // TRACKERTHREAD_H
