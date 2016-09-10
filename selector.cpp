#include "selector.h"
#include <iostream>


Selector::Selector() :
    isStartSelection_(false),
    isEndSelection_(false),
    scaleX_(1.f),
    scaleY_(1.f)
{
}

void Selector::setStart(int x, int y)
{
    isStartSelection_ = true;
    isEndSelection_ = false;
    x1 = x;
    y1 = y;
}

void Selector::setEnd(int x, int y, bool isFinal)
{
    x2 = x;
    y2 = y;

    const int MIN_SIZE = 25;
    if (std::abs(x1 - x2) < MIN_SIZE || std::abs(y1 - y2) < MIN_SIZE)
    {
        if (isFinal)
        {
            isStartSelection_ = false;
            isEndSelection_ = false;
            return;
        }
    }

    rect_.x = std::min(x1, x2);
    rect_.y = std::min(y1, y2);
    rect_.width = std::max(x1, x2) - rect_.x;
    rect_.height = std::max(y1, y2) - rect_.y;

    isEndSelection_ = isFinal;
    if (isEndSelection_)
    {
        isStartSelection_ = false;
    }
}

bool Selector::isSelected() const
{
    return isEndSelection_;
}

bool Selector::isSelecting() const
{
    return isStartSelection_;
}

void Selector::setScale(float scaleX, float scaleY)
{
    scaleX_ = scaleX;
    scaleY_ = scaleY;
}

cv::Rect Selector::getRect() const
{
    cv::Rect rect = rect_;
    rect.x *= scaleX_;
    rect.y *= scaleY_;
    rect.width *= scaleX_;
    rect.height *= scaleY_;
    return rect;
}

void Selector::draw(cv::Mat &frame)
{
    cv::rectangle(frame, rect_, (isStartSelection_ ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0)), 1, 8);
}

void Selector::reset()
{
    isEndSelection_ = false;
    isStartSelection_ = false;
    x1 = x2 = y1 = y2 = 0;
    rect_ = cv::Rect();
}
