#include "easychange.h"
#include <math.h>
#include <iostream>
#include "timer.h"

#include <opencv2/opencv.hpp>


EasyChange::EasyChange() :
    threshold(0.01f),
    speed(30.f),
    change(0.5),
    isChanging(false),
    easyDepth(0.f),
    needRecalculateTime(true)
{

}

void EasyChange::setThreshold(float threshold)
{
    this->threshold = std::max(0.01f, threshold);
}

void EasyChange::setSpeed(float speed)
{
    this->speed = speed;
    needRecalculateTime = true;
}

void EasyChange::setChange(float change)
{
    this->change = change;
    draw();
}

void EasyChange::reset()
{
    isChanging = false;
}

float EasyChange::getEasyDepth()
{
    return easyDepth;
}

void EasyChange::draw()
{
    cv::Mat img;
    img.create(200, 200, CV_8UC1);
    img = cv ::Scalar(0);
    for (int x = 0; x < 200; x++)
    {
        int y = (1. - getEasyVal(x / 200.)) * 200;
        cv::circle(img, cv::Point(x, y), 1, cv::Scalar::all(255));
    }
    cv::imshow("EasyChange", img);
}

float EasyChange::getEasyVal(float x)
{
    float a = std::pow(200, change - 0.5);
    return std::pow(x, a);
}

void EasyChange::update(float depth)
{
    if (!isChanging && 100 * fabs(easyDepth - depth) / (depth + 0.01) > threshold)
    {
        isChanging = true;
        needIncrease = easyDepth < depth;
        startTime = getTimeMs();
        startDepth = easyDepth;
        //std::cout << "START " << fabs(easyDepth - depth) << std::endl;
        //std::cout << "Easy change time = " << changeTime << std::endl;
        needRecalculateTime = true;
    }

    if (needRecalculateTime)
    {
        changeTime = std::max(0.1, fabs(startDepth - depth) / speed);
        needRecalculateTime = false;
    }

    if (isChanging)
    {
        long long thisTime = getTimeMs();
        //std::cout << "T:  " << (thisTime - startTime) / 1000. << std::endl;
        float changeT = std::min(1., (thisTime - startTime) / changeTime / 1000.);
        float diffChange = getEasyVal(changeT);
        float diff = diffChange * (depth - startDepth);
        //std::cout << "D: " << diffChange << std::endl;

        easyDepth = startDepth + diff;

        const float MIN_THRESHOLD = 0.01;
        if (fabs(easyDepth - depth) < MIN_THRESHOLD)
        {
            isChanging = false;
            //std::cout << "END" << std::endl;
        }
        //std::cout << depth << "   " << easyDepth << "   " << isChanging << std::endl;
    }
}

void EasyChange::setDepthHard(float depth)
{
    easyDepth = depth;
    reset();
}
