#ifndef KALMANDEPTH_H
#define KALMANDEPTH_H

#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

class KalmanDepth
{
public:
    KalmanDepth();

    void update(float& depth, float& fixedDepth, bool needInit);

    void setSensitivity(float val);
    void setMedianSize(int size);

private:
    cv::KalmanFilter kalmanFilter;

    cv::Mat state;  // [x1,x2,v_x1,v_x2,x3,x4]
    cv::Mat meas;    // [z_x1,z_x2,z_x3,z_x4]

    double ticks;

    std::vector<float> history;

    int medianSize;
    float kalmanSensitivity;
};

#endif // KALMANDEPTH_H
