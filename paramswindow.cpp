#include "paramswindow.h"
#include "ui_params.h"
#include <iostream>
#include <fstream>

ParamsWindow::ParamsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParamsWindow),
    addResolutions(true)
{
    ui->setupUi(this);
}

void ParamsWindow::init(Params* params)
{
    this->params = params;
    loadParams();
}

ParamsWindow::~ParamsWindow()
{
    delete ui;
}

void ParamsWindow::closeEvent(QCloseEvent *)
{
    setParamsValues();
    hide();
    saveParams();
    emit closed();
}

void ParamsWindow::setParamsValues()
{
    params->a1 = ui->valA_3->value();
    params->b1 = ui->valB_3->value();
    params->c1 = ui->valC_3->value();
    params->d1 = ui->valD_3->value();
    params->e1 = ui->valE_3->value();
    params->f1 = ui->valF_3->value();

    params->a2 = ui->valA->value();
    params->b2 = ui->valB->value();
    params->c2 = ui->valC->value();
    params->d2 = ui->valD->value();
    params->e2 = ui->valE->value();
    params->f2 = ui->valF->value();

    params->base = ui->valBase->value();
    params->cam1Ang = ui->cam1Ang->value();
    params->cam2Ang = ui->cam2Ang->value();
    params->focalLength = ui->valFocalLen->value();
    params->HFOV = ui->HFOV->value();
    params->VFOV = ui->VFOV->value();

    params->useDisparity = ui->useDisparity->isChecked();
    params->useTriangulation = ui->useTriangulation->isChecked();
    params->useCorrection1 = ui->useCorrection_1->isChecked();
    params->useCorrection2 = ui->useCorrection_2->isChecked();
    params->useCamCorrection = ui->useCamCorrection->isChecked();
    params->useFPFix = ui->useFPFix->isChecked();
    params->metric = ui->metric->isChecked();

    params->corrA = ui->camCorr_A->value();
    params->corrB = ui->camCorr_B->value();
    params->corrC = ui->camCorr_C->value();
    params->corrKdop = ui->camCorr_Kdop->value();

    params->useCalibration = ui->useCalibration->isChecked();

    if (resolutions.size())
    {
        params->cameraWidth = resolutions[ui->resolutions->currentIndex()].width;
        params->cameraHeight = resolutions[ui->resolutions->currentIndex()].height;
//        std::cout << ">>>> " <<__LINE__ << std::endl;
//        std::cout << params->cameraWidth << std::endl;
    }
}

void ParamsWindow::setParamsCamRes()
{
//    std::cout << ">>>> " <<__LINE__ << std::endl;
//    std::cout << params->cameraWidth << "   " << resolutions.size() << std::endl;
    for (unsigned int i = 0; i < resolutions.size(); i++)
    {
        if (resolutions[i].width == params->cameraWidth
                && resolutions[i].height == params->cameraHeight)
        {
            ui->resolutions->setCurrentIndex(i);
//            std::cout << i << std::endl;
            break;
        }
    }
}

void ParamsWindow::saveParams(std::string path)
{
    std::ofstream file(path.c_str());

    file << params->a1 << " " << params->b1 << " " << params->c1 << " " << params->d1 << " " << params->e1 << " " << params->f1 << std::endl;
    file << params->a2 << " " << params->b2 << " " << params->c2 << " " << params->d2 << " " << params->e2 << " " << params->f2 << std::endl;

    file << params->base << std::endl;
    file << params->cam1Ang << " " << params->cam2Ang << std::endl;
    file << params->focalLength << std::endl;
    file << params->HFOV << " " << params->VFOV << std::endl;

    file << params->useDisparity << std::endl;
    file << params->useTriangulation << std::endl;
    file << params->useCorrection1 << std::endl;
    file << params->useCorrection2 << std::endl;
    file << params->useCamCorrection << std::endl;
    file << params->useFPFix << std::endl;
    file << params->metric << std::endl;

    file << params->cameraWidth << std::endl;
    file << params->cameraHeight << std::endl;

    file << params->corrA << std::endl;
    file << params->corrB << std::endl;
    file << params->corrC << std::endl;
    file << params->corrKdop << std::endl;

    file << params->useCalibration << std::endl;

//    std::cout << ">>>> " <<__LINE__ << std::endl;
//    std::cout << params->cameraWidth << std::endl;

    file.close();
}

void ParamsWindow::loadParams(std::string path)
{
    std::cout << "Loading " << path << std::endl;
    std::ifstream file(path);
    if (file.is_open())
    {
        file >> params->a1 >> params->b1 >> params->c1 >> params->d1 >> params->e1 >> params->f1;
        file >> params->a2 >> params->b2 >> params->c2 >> params->d2 >> params->e2 >> params->f2;

        file >> params->base;
        file >> params->cam1Ang >> params->cam2Ang;
        file >> params->focalLength;
        file >> params->HFOV >> params->VFOV;

        file >> params->useDisparity;
        file >> params->useTriangulation;
        file >> params->useCorrection1;
        file >> params->useCorrection2;
        file >> params->useCamCorrection;
        file >> params->useFPFix;
        file >> params->metric;

        file >> params->cameraWidth;
        file >> params->cameraHeight;

        file >> params->corrA;
        file >> params->corrB;
        file >> params->corrC;
        file >> params->corrKdop;

        file >> params->useCalibration;

        setParamsCamRes();

        ui->valA_3->setValue(params->a1);
        ui->valB_3->setValue(params->b1);
        ui->valC_3->setValue(params->c1);
        ui->valD_3->setValue(params->d1);
        ui->valE_3->setValue(params->e1);
        ui->valF_3->setValue(params->f1);

        ui->valA->setValue(params->a2);
        ui->valB->setValue(params->b2);
        ui->valC->setValue(params->c2);
        ui->valD->setValue(params->d2);
        ui->valE->setValue(params->e2);
        ui->valF->setValue(params->f2);

        ui->valBase->setValue(params->base);
        ui->cam1Ang->setValue(params->cam1Ang);
        ui->cam2Ang->setValue(params->cam2Ang );
        ui->valFocalLen->setValue(params->focalLength);
        ui->HFOV->setValue(params->HFOV);
        ui->VFOV->setValue(params->VFOV);

        ui->useDisparity->setChecked(params->useDisparity);
        ui->useTriangulation->setChecked(params->useTriangulation);
        ui->useCorrection_1->setChecked(params->useCorrection1);
        ui->useCorrection_2->setChecked(params->useCorrection2);
        ui->useCamCorrection->setChecked(params->useCamCorrection);
        ui->useFPFix->setChecked(params->useFPFix);
        ui->metric->setChecked(params->metric);

        ui->camCorr_A->setValue(params->corrA);
        ui->camCorr_B->setValue(params->corrB);
        ui->camCorr_C->setValue(params->corrC);
        ui->camCorr_Kdop->setValue(params->corrKdop);

        ui->useCalibration->setChecked(params->useCalibration);

        file.close();
    }
    else
    {
        params->cameraWidth = 640;
        params->cameraHeight = 480;
        setParamsCamRes();
    }
    setParamsValues();
}

void ParamsWindow::setCamResolutions(const std::vector<cv::Size> &res)
{
    resolutions = res;
//    std::cout << ">>>" << __LINE__ << std::endl;
//    std::cout << resolutions.size() << std::endl;

    addResolutions = true;
    ui->resolutions->clear();
    for (unsigned int i = 0; i < resolutions.size(); i++)
    {
        ui->resolutions->addItem(QString::number(resolutions[i].width) + "x" + QString::number(resolutions[i].height));
    }
    addResolutions = false;
    setParamsCamRes();
}


void ParamsWindow::on_resolutions_currentIndexChanged(int)
{
    if (!addResolutions)
    {
        setParamsValues();
    }
}
