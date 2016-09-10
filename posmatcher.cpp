#include "posmatcher.h"
#include "timer.h"


//bool comparator(std::pair<float, cv::Point2f>& i, std::pair<float, cv::Point2f>& j)
//{
//    return i.first < j.first;
//}

PosMatcher::PosMatcher()
{
    detector = cv::ORB::create(500, 1.1, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 10);
    extractor = cv::ORB::create();
}

bool PosMatcher::match(const cv::Mat& left, const cv::Mat& right,
                       const cv::Rect& leftRoi, const cv::Rect& rightRoi,
                       std::vector<std::pair<cv::Point2f, cv::Point2f> >& pos,
                       bool debug)
{
    pos.clear();

    if (!debug)
    {
        cv::destroyWindow("Matches");
    }

    cv::Rect globalRoi;
    globalRoi.x = globalRoi.y = 0;
    globalRoi.width = left.cols;
    globalRoi.height = left.rows;

    cv::Mat leftRef = left(leftRoi & globalRoi);
    cv::Mat rightRef = right(rightRoi & globalRoi);

    if (leftRef.empty() || rightRef.empty())
    {
        return false;
    }

    cv::Mat leftImg, rightImg;
    cvtColor(leftRef, leftImg, CV_BGR2GRAY);
    cvtColor(rightRef, rightImg, CV_BGR2GRAY);
    leftRef.copyTo(leftImg);
    rightRef.copyTo(rightImg);

    std::vector<cv::KeyPoint> leftKeys;
    std::vector<cv::KeyPoint> rightKeys;
    cv::Mat leftObj;
    cv::Mat rightObj;

    detector->detect(leftImg, leftKeys);
    detector->detect(rightImg, rightKeys);

    const int MIN_MATCHES = NUM_FEATURE_POINTS + 1;

    if (leftKeys.size() < MIN_MATCHES || rightKeys.size() < MIN_MATCHES)
    {
        cv::destroyWindow("Matches");
        return false;
    }

    extractor->compute(leftImg, leftKeys, leftObj);
    extractor->compute(rightImg, rightKeys, rightObj);

    if(leftObj.type() != CV_32F)
    {
        leftObj.convertTo(leftObj, CV_32F);
    }

    if(rightObj.type() != CV_32F)
    {
        rightObj.convertTo(rightObj, CV_32F);
    }

//    std::cout << leftObj.type() << " " << rightObj.type() << std::endl;
//    std::cout << leftObj.rows << " " << leftObj.cols << std::endl;
//    std::cout << rightObj.rows << " " << rightObj.cols << std::endl;
//    std::cout << "===" << std::endl;

    if(rightObj.empty() || leftObj.empty())
    {
        cv::destroyWindow("Matches");
        return false;
    }

    const float MATCH_THRESHOLD = 400.f;

    std::vector<std::vector<cv::DMatch> > matches;
    matcher.radiusMatch(leftObj, rightObj, matches, MATCH_THRESHOLD);

//    std::cout << leftObj.cols << "  " << leftObj.rows << std::endl;
//    std::cout << rightObj.cols << "  " << rightObj.rows << std::endl;
//    std::cout << "-----" << std::endl;

    int dy = std::abs(leftRoi.y + leftRoi.height / 2 - rightRoi.y - rightRoi.height / 2);
    const float THRESHOLD_Y = dy + 15;
    const float THRESHOLD_XY = (leftRoi.width + rightRoi.width) / 2. + 5;

//    std::cout << THRESHOLD_Y << " " << THRESHOLD_XY << std::endl;

    std::vector<cv::DMatch> goodMatches;
    for (unsigned int i = 0; i < matches.size(); i++)
    {
        int bestPos = 0;
        float minError = MATCH_THRESHOLD + 1.;
        for (unsigned int j = 0; j < matches[i].size(); j++)
        {
            cv::Point2f p1 = leftKeys[matches[i][j].queryIdx].pt;
            cv::Point2f p2 = rightKeys[matches[i][j].trainIdx].pt;

            if (fabs(p1.y - p2.y) < THRESHOLD_Y
                && std::sqrt(std::pow(p1.x - p2.x, 2.) + std::pow(p1.y - p2.y, 2.) < THRESHOLD_XY))
            {
                float error = matches[i][j].distance;
                if (error < minError)
                {
                    minError = error;
                    bestPos = j;
                }
            }
            if (minError < MATCH_THRESHOLD)
            {
                goodMatches.push_back(matches[i][bestPos]);
            }
        }
    }

    std::sort(goodMatches.begin(), goodMatches.end());

    if (goodMatches.size() < MIN_MATCHES)
    {
        cv::destroyWindow("Matches");
        return false;
    }

    std::vector<cv::DMatch> bestMatches;
    for (unsigned int i = 0; i < goodMatches.size() && bestMatches.size() < NUM_FEATURE_POINTS; i++)
    {
        cv::Point2f p1 = leftKeys[goodMatches[i].queryIdx].pt;
        cv::Point2f p2 = rightKeys[goodMatches[i].trainIdx].pt;

        bool isUniq = true;
        for (unsigned int j = 0; j < pos.size(); j++)
        {
            if (std::sqrt(std::pow(p1.x - pos[j].first.x, 2) + std::pow(p1.y - pos[j].first.y, 2)) < 10)
            {
                isUniq = false;
                break;
            }
        }

        if (isUniq)
        {
            bestMatches.push_back(goodMatches[i]);
            pos.push_back(std::make_pair(p1, p2));
        }
    }

    if (goodMatches.size() < NUM_FEATURE_POINTS)
    {
        cv::destroyWindow("Matches");
        if (debug)
        {
            std::cout << "Not enought matches: " << bestMatches.size() << std::endl;
        }
        return false;
    }

    if (debug)
    {
        std::cout << "Matches: " << bestMatches.size() << std::endl;
        for (unsigned int i = 0; i < leftKeys.size(); i++)
        {
            cv::circle(leftImg, leftKeys[i].pt, leftKeys[i].size * 0 + 1, cv::Scalar::all(250), 1);
        }
        for (unsigned int i = 0; i < rightKeys.size(); i++)
        {
            cv::circle(rightImg, rightKeys[i].pt, rightKeys[i].size * 0 + 1, cv::Scalar::all(250), 1);
        }

        //    cv::circle(leftImg, centerImgL, 1, cv::Scalar(255, 0, 0), 3);
        //    cv::circle(rightImg, centerImgR, 1, cv::Scalar(255, 0, 0), 3);

        cv::Mat img_matches;
        cv::drawMatches( leftImg, leftKeys, rightImg, rightKeys,
                         bestMatches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
                         std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        cv::imshow("Matches", img_matches);
        //cv::waitKey(0);
    }

    return true;
}
