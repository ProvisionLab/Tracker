#include "rdetector.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


cv::Rect getRect(const cv::Mat& img)
{
    return cv::Rect(0, 0, img.cols, img.rows);
}

cv::Rect getStereoRoi(const cv::Mat& frame, const cv::Rect& rect)
{
    const int SMALL_DELTA = 40;
    const int DELTA_X = 170;

    cv::Rect roi;
    roi.x = std::max(0, rect.x - SMALL_DELTA);
    roi.y = std::max(0, rect.y - SMALL_DELTA);
    roi.width = rect.width + DELTA_X + SMALL_DELTA;
    roi.height = rect.height + SMALL_DELTA * 2;

    return roi & getRect(frame);
}


void detectStereo(const cv::Mat& left, const cv::Mat& right,
                  const cv::Rect& leftRect, cv::Rect& rightRect)
{
    cv::Rect roi = getStereoRoi(left, leftRect);
    cv::Mat templ(left(leftRect & getRect(left)));
    cv::Mat img(right(roi));

    const float MAX_TEMPL_SIZE = 30;
    float zoom = templ.cols > MAX_TEMPL_SIZE ? templ.cols / MAX_TEMPL_SIZE : 1.f;

    cv::resize(templ, templ, cv::Size(templ.cols / zoom, templ.rows / zoom));
    cv::resize(img, img, cv::Size(img.cols / zoom, img.rows / zoom));

    // Create the result matrix
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    cv::Mat result;
    result.create(result_cols, result_rows, CV_32FC1);

    // Do the Matching and Normalize
    const int MATCH_METHOD = cv::TM_SQDIFF;
    cv::matchTemplate(img, templ, result, MATCH_METHOD);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // Localizing the best match with minMaxLoc
    double minVal; double maxVal;
    cv::Point minLoc, maxLoc;
    cv::Point matchLoc;

    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    // For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if(MATCH_METHOD == cv::TM_SQDIFF || MATCH_METHOD == cv::TM_SQDIFF_NORMED)
    {
        matchLoc = minLoc;
    }
    else
    {
        matchLoc = maxLoc;
    }

    // Show me what you got
    rightRect.x = roi.x + matchLoc.x * zoom;
    rightRect.y = roi.y + matchLoc.y * zoom;
    rightRect.width = leftRect.width;
    rightRect.height = leftRect.height;
}
