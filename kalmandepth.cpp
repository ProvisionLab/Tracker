#include "kalmandepth.h"
#include <iostream>


KalmanDepth::KalmanDepth() :
    kalmanFilter(4, 2, 0),
    medianSize(5),
    kalmanSensitivity(0.3)
{
    // intialization of KF...
    kalmanFilter.transitionMatrix = cv::Mat_<float>(4, 4);

    kalmanFilter.transitionMatrix.at<float>(0, 0) = 1;
    kalmanFilter.transitionMatrix.at<float>(0, 1) = 0;
    kalmanFilter.transitionMatrix.at<float>(0, 2) = 1;
    kalmanFilter.transitionMatrix.at<float>(0, 3) = 0;

    kalmanFilter.transitionMatrix.at<float>(1, 0) = 0;
    kalmanFilter.transitionMatrix.at<float>(1, 1) = 1;
    kalmanFilter.transitionMatrix.at<float>(1, 2) = 0;
    kalmanFilter.transitionMatrix.at<float>(1, 3) = 1;

    kalmanFilter.transitionMatrix.at<float>(2, 0) = 0;
    kalmanFilter.transitionMatrix.at<float>(2, 1) = 0;
    kalmanFilter.transitionMatrix.at<float>(2, 2) = 1;
    kalmanFilter.transitionMatrix.at<float>(2, 3) = 0;

    kalmanFilter.transitionMatrix.at<float>(3, 0) = 0;
    kalmanFilter.transitionMatrix.at<float>(3, 1) = 0;
    kalmanFilter.transitionMatrix.at<float>(3, 2) = 0;
    kalmanFilter.transitionMatrix.at<float>(3, 3) = 1;
}

void KalmanDepth::update(float &depth, float &fixedDepth, bool needInit)
{
    if (needInit) // First detection!
    {
        kalmanFilter.statePre.at<float>(0) = depth;
        kalmanFilter.statePre.at<float>(1) = 1;
        kalmanFilter.statePre.at<float>(2) = 0;
        kalmanFilter.statePre.at<float>(3) = 0;
        kalmanFilter.statePre.copyTo(kalmanFilter.statePost);
        cv::setIdentity(kalmanFilter.measurementMatrix);
        cv::setIdentity(kalmanFilter.processNoiseCov, cv::Scalar::all(2.9));
        cv::setIdentity(kalmanFilter.measurementNoiseCov, cv::Scalar::all(kalmanSensitivity));
        cv::setIdentity(kalmanFilter.errorCovPost, cv::Scalar::all(10));

        history.clear();
    }

    // First predict, to update the internal statePre variable
    kalmanFilter.predict();

    cv::Mat_<float> measurement(2, 1);
    measurement(0) = depth;
    measurement(1) = 1;

    // The update phase
    cv::Mat estimated = kalmanFilter.correct(measurement);

    if (!history.size())
    {
        fixedDepth = estimated.at<float>(0);
        history.push_back(estimated.at<float>(0));
    }
    else
    {
        history.push_back(estimated.at<float>(0));
        if (history.size() > medianSize)
        {
            history.erase(history.begin());
        }

//        for (int i = 0; i < history.size(); i++)
//        {
//            std::cout << history[i] << " ";
//        }
//        std::cout << std::endl;

        std::vector<float> t = history;
        std::nth_element(t.begin(), t.begin() + t.size() / 2, t.end());
        fixedDepth = t[t.size() / 2];
    }

//    std::cout << fixedDepth << std::endl;

    //std::cout << needInit << "    " << depth << " " << fixedDepth << std::endl;

    //std::cout << "Measure matrix:" << meas << std::endl;
}

void KalmanDepth::setSensitivity(float val)
{
    kalmanSensitivity = val;
    cv::setIdentity(kalmanFilter.measurementNoiseCov, cv::Scalar::all(kalmanSensitivity));
}

void KalmanDepth::setMedianSize(int size)
{
    medianSize = size;
}
