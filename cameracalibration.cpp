#include "cameracalibration.h"
#include <iostream>
#include <fstream>


const float DISCRETIZATION_D = 50;
const int DETALIZATION_Y = 2; // must divide on resolution height

CameraCalibration::CameraCalibration() :
    isLoaded(false),
    maxY(0),
    maxD(0),
    maxDistance(0)
{
}

void CameraCalibration::startCalibration(int maxY, float maxD)
{
    maxDistance = 0;
    this->maxY = maxY / DETALIZATION_Y;
    this->maxD = maxD * DISCRETIZATION_D;

    correction.create(this->maxY, this->maxD, CV_32F);

    clear();
}

void CameraCalibration::save()
{   
    if (!isLoaded && !posX.size())
    {
        return;
    }

    std::stringstream str;
    str << "calib_" << maxY * DETALIZATION_Y;

    if (posX.size())
    {
        interpolate();
    }

    std::ofstream file(str.str().c_str());
    file << maxDistance << std::endl;
    file << correction.cols << " " << correction.rows << std::endl;

    float* pData = (float*)correction.data;
    int n = 0;
    for (long i = correction.rows * correction.cols; i > 0; --i)
    {
        file << *pData++ << " ";
        n++;
        if (n % correction.cols == 0)
        {
            file << std::endl;
        }
    }

    if (val.size())
    {
        file << "\n\n=============DEBUG==============\n";
        for (unsigned int i = 0; i < val.size(); i++)
        {
            file << "#" << i << ":   " << posX[i] << " " << posY[i] << "   => " << val[i] << std::endl;
        }
    }
    file.close();

    std::cout << "Saved calibration: " << str.str() << std::endl;

    posX.clear();
    posY.clear();
    val.clear();
}

bool CameraCalibration::load(std::string path, int maxY)
{
    std::stringstream str;
    if (path.length())
    {
        str << path << "//";
    }
    str << "calib_" << maxY;

    std::ifstream file(str.str().c_str());
    if (!file.is_open())
    {
        std::cout << "can't load calibration: " << str.str() << std::endl;
        isLoaded = false;
        return false;
    }
    isLoaded = true;

    prevCalibrationPath = path;
    prevMaxY = maxY;

    file >> this->maxDistance;
    file >> this->maxY >> maxD;

    correction.create(this->maxY, this->maxD, CV_32F);

    float* pData = (float*)correction.data;
    for (long i = correction.rows * correction.cols; i > 0; --i)
    {
        float c;
        file >> c;
        *pData++ = c;
    }
    file.close();

    std::cout << "Loaded calibration: " << str.str() << std::endl;
    return true;
}

void CameraCalibration::print()
{
    float* pData = (float*)correction.data;

    int n = 0;
    for (long i = correction.rows * correction.cols; i > 0; --i)
    {
        std::cout << *pData++ << "\t";
        n++;
        if (n % correction.cols == 0)
        {
            std::cout << std::endl;
        }
    }
}

bool CameraCalibration::isCorrect()
{
    return maxDistance > 0.1;
}


void CameraCalibration::addPoint(float d, int y, float computedDepth)
{
    maxDistance = std::max(maxDistance, d);
    if (y < 0 || computedDepth < 0.1)
    {
        return;
    }

    int fixedY = std::max(0, std::min((int)std::round(y / (float)DETALIZATION_Y), maxY - 1));
    int fixedComputedDepth = std::max(0, std::min((int)std::round(computedDepth * DISCRETIZATION_D), maxD - 1));

    posX.push_back(fixedComputedDepth);
    posY.push_back(fixedY);
    val.push_back(d / computedDepth);

    //std::cout << d << " " << computedDepth << std::endl;
    //std::cout << "#" << val.size() - 1 << ":   " << posX[val.size() - 1] << " " << posY[val.size() - 1] << "   => " << val[val.size() - 1] << std::endl;

    progress[fixedY] = true;
}

float CameraCalibration::getMaxDistance() const
{
    return maxDistance;
}

float CameraCalibration::getFixedDepth(float d, int y)
{
    if (!isLoaded)
    {
        return 0.;
    }

    int fixedY = std::max(0, std::min((int)std::round(y / (float)DETALIZATION_Y), maxY - 1));
    int fixedD = std::max(0, std::min((int)std::round(d * DISCRETIZATION_D), maxD - 1));

    float* pData = (float*)correction.data;
    int pos = fixedY * correction.cols + fixedD;
    return pData[pos] * d;
}

void CameraCalibration::resetProgress()
{
    progress.clear();
    progress.resize(maxY, false);
}

int CameraCalibration::getProgress() const
{
    float sum = 0;
    for (unsigned int i = 0; i < progress.size(); i++)
    {
        sum += progress[i] ? 1 : 0;
    }
    return std::round(100 * sum / progress.size());
}

void CameraCalibration::clear()
{
    posX.clear();
    posY.clear();
    val.clear();
}

void CameraCalibration::print(std::vector<float>& line)
{
    for (int i = 0; i < line.size(); i++)
    {
        std::cout << line[i] << " ";
    }
    std::cout << std::endl;
}

void CameraCalibration::interpolate(std::vector<float>& line)
{
    // extrapolate left
    int minI = 0;
    int i = minI;
    for (; i < line.size(); i++)
    {
        if (line[i] > 0.)
        {
            break;
        }
    }
    if (i == line.size())
    {
        return;
    }
    minI = i;

    for (i = 0; i < minI; i++)
    {
        line[i] = line[minI];
    }

    // extrapolate right
    int maxI = line.size() - 1;
    i = maxI;
    for (; i >= 0; i--)
    {
        if (line[i] > 0.)
        {
            break;
        }
    }
    maxI = i;

    for (i = maxI; i < line.size(); i++)
    {
        line[i] = line[maxI];
    }

    int s = 0, e = 0;
    for (int i = 0; i < line.size() - 2; i++)
    {
        if (line[i] > 0. && line[i+1] < 0.)
        {
            s = i;
        }
        if (line[i] < 0. && line[i+1] > 0.)
        {
            e = i + 1;
            for (int j = s + 1; j <= e - 1; j++)
            {
                line[j] = (line[s] * (e - j) + line[e] * (j - s)) / float(e - s);
            }
        }
    }

}

void CameraCalibration::interpolate()
{
    const int h = this->maxY;
    const int w = this->maxD;

    float* pData = (float*)correction.data;
    std::vector<std::vector<int> > nums;
    nums.resize(h, std::vector<int>(w));  //int nums[h][w];
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int pos = y * correction.cols + x;
            pData[pos] = 0.;
            nums[y][x] = 0;
        }
    }

    for (unsigned int i = 0; i < posX.size(); i++)
    {
        int y = posY[i];
        int x = posX[i];
        int pos = y * correction.cols + x;
        pData[pos] += val[i];
        nums[y][x]++;

        std::cout << x << std::endl;
        std::cout << y << std::endl;
        std::cout << val[i] << std::endl;
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int pos = y * correction.cols + x;
            if (nums[y][x] > 0)
            {
                pData[pos] /= nums[y][x];
            }
            else
            {
                pData[pos] = -1;
            }
        }
    }

//    for (int y = 0; y < h; y++)
//    {
//        for (int x = 0; x < w; x++)
//        {
//            int pos = y * correction.cols + x;
//            std::cout << pData[pos] << " ";
//        }
//        //std::cout << std::endl;
//    }

    // horisontal
    std::vector<float> lineY;
    lineY.resize(h);
    for (int i = 0; i < w; i++)
    {
        for (int y = 0; y < h; y++)
        {
            int pos = y * correction.cols + i;
            lineY[y] = pData[pos];
        }
        interpolate(lineY);
        for (int y = 0; y < h; y++)
        {
            int pos = y * correction.cols + i;
            pData[pos] = lineY[y];
        }
    }

    // vertical
    std::vector<float> lineD;
    lineD.resize(w);
    for (int i = 0; i < h; i++)
    {
        for (int x = 0; x < w; x++)
        {
            int pos = i * correction.cols + x;
            lineD[x] = pData[pos];
        }
        interpolate(lineD);
        for (int x = 0; x < w; x++)
        {
            int pos = i * correction.cols + x;
            pData[pos] = lineD[x];
        }
    }
    isLoaded = true;
}
