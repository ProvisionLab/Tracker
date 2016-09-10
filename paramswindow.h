#ifndef PARAMSWINDOW_H
#define PARAMSWINDOW_H

#include <QMainWindow>
#include <string>
#include <opencv2/core/core.hpp>

namespace Ui {
class ParamsWindow;
}

struct Params
{
    double a1, b1, c1, d1, e1, f1;
    double a2, b2, c2, d2, e2, f2;

    double base;
    double cam1Ang, cam2Ang;
    double focalLength;
    double HFOV, VFOV;

    bool useDisparity;
    bool useTriangulation;
    bool useCorrection1;
    bool useCorrection2;
    bool useCamCorrection;
    bool metric;
    bool useFPFix;

    int cameraWidth;
    int cameraHeight;

    float corrA;
    float corrB;
    float corrC;
    float corrKdop;

    bool useCalibration;
};

class ParamsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParamsWindow(QWidget *parent = 0);
    void init(Params *params);
    void saveParams(std::string path = "params.txt");
    void loadParams(std::string path = "params.txt");

    void setCamResolutions(const std::vector<cv::Size>& res);

    ~ParamsWindow();

private:
    void closeEvent(QCloseEvent *event);

    void setParamsValues();

    void setParamsCamRes();


signals:
    void closed();

private slots:
    void on_resolutions_currentIndexChanged(int index);

private:
    Ui::ParamsWindow *ui;
    Params* params;
    std::vector<cv::Size> resolutions;
    bool addResolutions;
};

#endif // PARAMSWINDOW_H
