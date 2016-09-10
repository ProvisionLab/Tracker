#include "trackerthread.h"

TrackerThread::TrackerThread()
{

}

void TrackerThread::set(TrackerAPI *tracker, cv::Mat *image, cv::Rect2f *result, int *w)
{
    this->tracker = tracker;
    this->image = image;
    this->result = result;
    this->w = w;
}

void TrackerThread::run()
{
    if (!tracker || !image || !result || !w)
    {
        std::cerr << "Not inited tracker thread" << std::endl;
    }

    tracker->detect(*image, *result, -1, -1, *w);
}
