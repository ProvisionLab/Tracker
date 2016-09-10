#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <map>


class CameraCalibration
{
public:
    CameraCalibration();
    void startCalibration(int maxY, float maxD);

    void save();
    bool load(std::string path, int maxY);
    void print();

    bool isCorrect();

    void addPoint(float d, int y, float computedDepth);

    float getMaxDistance() const;

    float getFixedDepth(float d, int y);

    void resetProgress();
    int getProgress() const;

    void clear();

private:
    void interpolate();
    void print(std::vector<float>& line);
    void interpolate(std::vector<float>& line);

private:
    float maxDistance;
    bool isLoaded;
    int maxY, maxD;
    std::vector<bool> progress;

    std::vector<int> posX, posY;
    std::vector<float> val;

    cv::Mat correction;

    std::string prevCalibrationPath;
    std::string prevMaxY;
};

#endif // CAMERACALIBRATION_H
