#ifndef KALMANXY_H
#define KALMANXY_H

#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

class KalmanXY
{
public:
    KalmanXY();
    void setSensitivity(float sensitivity);
    void init();
    void update(const cv::Rect2f& inRect, cv::Rect2f& rect, bool detected);

    bool isPrediction() const;

private:
    cv::KalmanFilter kf;

    cv::Mat state;  // [x,y,v_x,v_y,w,h]
    cv::Mat meas;    // [z_x,z_y,z_w,z_h]

    double ticks;
    bool found;
    int notFoundCount;
};

#endif // KALMANXY_H
