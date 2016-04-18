#include "fpscounter.h"
#include "timer.h"

FpsCounter::FpsCounter() :
    lastTime_(0),
    prevFps_(-1.)
{
}

int FpsCounter::updateFps()
{
    if (!lastTime_)
    {
        lastTime_ = getTimeMs();
        return 0;
    }

    long long time = getTimeMs() - lastTime_;
    double fps = 1000 / time;
    if (prevFps_ > 0.)
    {
        fps = 0.95 * fps + 0.05 * prevFps_;
    }
    prevFps_ = fps;
    lastTime_ = getTimeMs();

    return fps;
}
