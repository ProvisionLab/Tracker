#include "kalmanxy.h"
#include <iostream>

const int stateSize = 6;
const int measSize = 4;
const int contrSize = 0;

unsigned int type = CV_32F;


KalmanXY::KalmanXY() :
    kf(stateSize, measSize, contrSize, type),
    state(stateSize, 1, type),  // [x,y,v_x,v_y,w,h]
    meas(measSize, 1, type),    // [z_x,z_y,z_w,z_h]
    ticks(0.),
    found(false),
    notFoundCount(0)
{
    //cv::Mat procNoise(stateSize, 1, type)
    // [E_x,E_y,E_v_x,E_v_y,E_w,E_h]

    // Transition State Matrix A
    // Note: set dT at each processing step!
    // [ 1 0 dT 0  0 0 ]
    // [ 0 1 0  dT 0 0 ]
    // [ 0 0 1  0  0 0 ]
    // [ 0 0 0  1  0 0 ]
    // [ 0 0 0  0  1 0 ]
    // [ 0 0 0  0  0 1 ]
    cv::setIdentity(kf.transitionMatrix);

    // Measure Matrix H
    // [ 1 0 0 0 0 0 ]
    // [ 0 1 0 0 0 0 ]
    // [ 0 0 0 0 1 0 ]
    // [ 0 0 0 0 0 1 ]
    kf.measurementMatrix = cv::Mat::zeros(measSize, stateSize, type);
    kf.measurementMatrix.at<float>(0) = 1.0f;
    kf.measurementMatrix.at<float>(7) = 1.0f;
    kf.measurementMatrix.at<float>(16) = 1.0f;
    kf.measurementMatrix.at<float>(23) = 1.0f;

    // Process Noise Covariance Matrix Q
    // [ Ex   0   0     0     0    0  ]
    // [ 0    Ey  0     0     0    0  ]
    // [ 0    0   Ev_x  0     0    0  ]
    // [ 0    0   0     Ev_y  0    0  ]
    // [ 0    0   0     0     Ew   0  ]
    // [ 0    0   0     0     0    Eh ]
    //cv::setIdentity(kf.processNoiseCov, cv::Scalar(1e-2));
    kf.processNoiseCov.at<float>(0) = 1e-2;
    kf.processNoiseCov.at<float>(7) = 1e-2;
    kf.processNoiseCov.at<float>(14) = 5.0f;
    kf.processNoiseCov.at<float>(21) = 5.0f;
    kf.processNoiseCov.at<float>(28) = 1e-2;
    kf.processNoiseCov.at<float>(35) = 1e-2;

    // Measures Noise Covariance Matrix R
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));
    // <<<< Kalman Filter
}

void KalmanXY::setSensitivity(float sensitivity)
{
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(sensitivity));
}

void KalmanXY::init()
{
    found = false;
    notFoundCount = 0;
}

void KalmanXY::update(const cv::Rect2f& inRect, cv::Rect2f &rect, bool detected)
{
    double precTick = ticks;
    ticks = (double) cv::getTickCount();

    double dT = (ticks - precTick) / cv::getTickFrequency(); //seconds

    cv::Rect predRect;
    if (found)
    {
        // >>>> Matrix A
        kf.transitionMatrix.at<float>(2) = dT;
        kf.transitionMatrix.at<float>(9) = dT;
        // <<<< Matrix A

        //std::cout << "dT: " << dT << std::endl;

        state = kf.predict();
        //std::cout << "State post: " << state << std::endl;

        predRect.width = state.at<float>(4);
        predRect.height = state.at<float>(5);
        predRect.x = state.at<float>(0) - predRect.width / 2;
        predRect.y = state.at<float>(1) - predRect.height / 2;
    }

    // >>>>> Kalman Update
    if (!detected)
    {
        notFoundCount++;
        //std::cout << "notFoundCount: " << notFoundCount << std::endl;
        if( notFoundCount >= 100)
        {
            found = false;
        }
        /*else kf.statePost = state;*/
    }
    else
    {
        notFoundCount = 0;

        meas.at<float>(0) = inRect.x + inRect.width / 2;
        meas.at<float>(1) = inRect.y + inRect.height / 2;
        meas.at<float>(2) = (float)inRect.width;
        meas.at<float>(3) = (float)inRect.height;

        if (!found) // First detection!
        {
            // >>>> Initialization
            kf.errorCovPre.at<float>(0) = 1; // px
            kf.errorCovPre.at<float>(7) = 1; // px
            kf.errorCovPre.at<float>(14) = 1;
            kf.errorCovPre.at<float>(21) = 1;
            kf.errorCovPre.at<float>(28) = 1; // px
            kf.errorCovPre.at<float>(35) = 1; // px

            state.at<float>(0) = meas.at<float>(0);
            state.at<float>(1) = meas.at<float>(1);
            state.at<float>(2) = 0;
            state.at<float>(3) = 0;
            state.at<float>(4) = meas.at<float>(2);
            state.at<float>(5) = meas.at<float>(3);
            // <<<< Initialization

            found = true;
        }
        else
        {
            kf.correct(meas); // Kalman Correction
        }

        //std::cout << "Measure matrix:" << meas << std::endl;
    }
    // <<<<< Kalman Update

    rect = predRect;
}

bool KalmanXY::isPrediction() const
{
    return notFoundCount > 0;
}
