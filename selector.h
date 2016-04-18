#ifndef SELECTOR_H
#define SELECTOR_H

#include <opencv2/imgproc/imgproc.hpp>

class Selector
{
public:
    Selector();

    void setStart(int x, int y);
    void setEnd(int x, int y, bool isFinal = false);
    bool isSelected() const;
    bool isSelecting() const;

    const cv::Rect& getRect() const;
    void draw(cv::Mat& frame);

    void reset();

private:
    int x1, x2, y1, y2;
    cv::Rect rect_;
    bool isStartSelection_;
    bool isEndSelection_;
};


#endif // SELECTOR_H
