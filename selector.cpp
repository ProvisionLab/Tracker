#include "selector.h"


Selector::Selector() :
    isEndSelection_(false),
    isStartSelection_(false)
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

    if (x1 == x2 || y1 == y2)
    {
        if (isFinal)
        {
            isStartSelection_ = false;
            isEndSelection_ = false;
        }
        return;
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

const cv::Rect& Selector::getRect() const
{
    return rect_;
}

void Selector::draw(cv::Mat &frame)
{
    cv::rectangle(frame, rect_, (isStartSelection_ ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0)), 1, 8);
}

void Selector::reset()
{
    isEndSelection_ = false;
    isStartSelection_ = false;
}
