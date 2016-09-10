/********************************************************************************
** Form generated from reading UI file 'params.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMS_H
#define UI_PARAMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParamsWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QDoubleSpinBox *valBase;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_22;
    QDoubleSpinBox *cam1Ang;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_23;
    QDoubleSpinBox *cam2Ang;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_24;
    QHBoxLayout *horizontalLayout_4;
    QLabel *runFromSingleCameraLabel;
    QDoubleSpinBox *valFocalLen;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *runFromSingleCameraLabel_2;
    QDoubleSpinBox *HFOV;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_9;
    QLabel *text_21;
    QDoubleSpinBox *VFOV;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_10;
    QLabel *text_22;
    QComboBox *resolutions;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_3;
    QCheckBox *useFPFix;
    QCheckBox *useDisparity;
    QCheckBox *useCorrection_1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_16;
    QDoubleSpinBox *valA_3;
    QLabel *label_17;
    QDoubleSpinBox *valB_3;
    QLabel *label_18;
    QDoubleSpinBox *valC_3;
    QLabel *label_19;
    QDoubleSpinBox *valD_3;
    QLabel *label_20;
    QDoubleSpinBox *valE_3;
    QLabel *label_21;
    QDoubleSpinBox *valF_3;
    QCheckBox *useTriangulation;
    QCheckBox *useCorrection_2;
    QCheckBox *useCalibration;
    QHBoxLayout *horizontalLayout;
    QLabel *label_9;
    QDoubleSpinBox *valA;
    QLabel *label_8;
    QDoubleSpinBox *valB;
    QLabel *label_7;
    QDoubleSpinBox *valC;
    QLabel *label_6;
    QDoubleSpinBox *valD;
    QLabel *label_5;
    QDoubleSpinBox *valE;
    QLabel *label_4;
    QDoubleSpinBox *valF;
    QCheckBox *metric;
    QLabel *label_10;
    QCheckBox *useCamCorrection;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_25;
    QDoubleSpinBox *camCorr_A;
    QLabel *label_26;
    QDoubleSpinBox *camCorr_B;
    QLabel *label_27;
    QDoubleSpinBox *camCorr_C;
    QLabel *label_28;
    QDoubleSpinBox *camCorr_Kdop;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ParamsWindow)
    {
        if (ParamsWindow->objectName().isEmpty())
            ParamsWindow->setObjectName(QStringLiteral("ParamsWindow"));
        ParamsWindow->resize(650, 560);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ParamsWindow->sizePolicy().hasHeightForWidth());
        ParamsWindow->setSizePolicy(sizePolicy);
        ParamsWindow->setMinimumSize(QSize(560, 540));
        ParamsWindow->setMaximumSize(QSize(650, 600));
        centralwidget = new QWidget(ParamsWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalWidget = new QWidget(centralwidget);
        verticalWidget->setObjectName(QStringLiteral("verticalWidget"));
        verticalWidget->setGeometry(QRect(0, 0, 651, 541));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(verticalWidget->sizePolicy().hasHeightForWidth());
        verticalWidget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(verticalWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(verticalWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setUnderline(true);
        label->setFont(font);

        verticalLayout->addWidget(label);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_2 = new QLabel(verticalWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setMinimumSize(QSize(135, 0));
        label_2->setMaximumSize(QSize(135, 16777215));

        horizontalLayout_5->addWidget(label_2);

        valBase = new QDoubleSpinBox(verticalWidget);
        valBase->setObjectName(QStringLiteral("valBase"));
        valBase->setMinimumSize(QSize(85, 0));
        valBase->setMaximumSize(QSize(85, 16777215));
        valBase->setDecimals(1);
        valBase->setMinimum(1);
        valBase->setMaximum(2000);
        valBase->setSingleStep(0.1);
        valBase->setValue(1);

        horizontalLayout_5->addWidget(valBase);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_22 = new QLabel(verticalWidget);
        label_22->setObjectName(QStringLiteral("label_22"));
        sizePolicy2.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy2);
        label_22->setMinimumSize(QSize(135, 0));
        label_22->setMaximumSize(QSize(135, 16777215));

        horizontalLayout_6->addWidget(label_22);

        cam1Ang = new QDoubleSpinBox(verticalWidget);
        cam1Ang->setObjectName(QStringLiteral("cam1Ang"));
        cam1Ang->setMinimumSize(QSize(85, 0));
        cam1Ang->setMaximumSize(QSize(85, 16777215));
        cam1Ang->setDecimals(2);
        cam1Ang->setMinimum(-180);
        cam1Ang->setMaximum(180);
        cam1Ang->setSingleStep(0.01);
        cam1Ang->setValue(0);

        horizontalLayout_6->addWidget(cam1Ang);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_23 = new QLabel(verticalWidget);
        label_23->setObjectName(QStringLiteral("label_23"));
        sizePolicy2.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy2);
        label_23->setMinimumSize(QSize(135, 0));
        label_23->setMaximumSize(QSize(135, 16777215));

        horizontalLayout_7->addWidget(label_23);

        cam2Ang = new QDoubleSpinBox(verticalWidget);
        cam2Ang->setObjectName(QStringLiteral("cam2Ang"));
        cam2Ang->setMinimumSize(QSize(85, 0));
        cam2Ang->setMaximumSize(QSize(85, 16777215));
        cam2Ang->setDecimals(2);
        cam2Ang->setMinimum(-180);
        cam2Ang->setMaximum(180);
        cam2Ang->setSingleStep(0.01);
        cam2Ang->setValue(0);

        horizontalLayout_7->addWidget(cam2Ang);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_7);

        label_24 = new QLabel(verticalWidget);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setFont(font);

        verticalLayout->addWidget(label_24);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        runFromSingleCameraLabel = new QLabel(verticalWidget);
        runFromSingleCameraLabel->setObjectName(QStringLiteral("runFromSingleCameraLabel"));
        sizePolicy2.setHeightForWidth(runFromSingleCameraLabel->sizePolicy().hasHeightForWidth());
        runFromSingleCameraLabel->setSizePolicy(sizePolicy2);
        runFromSingleCameraLabel->setMinimumSize(QSize(145, 0));
        runFromSingleCameraLabel->setMaximumSize(QSize(145, 16777215));

        horizontalLayout_4->addWidget(runFromSingleCameraLabel);

        valFocalLen = new QDoubleSpinBox(verticalWidget);
        valFocalLen->setObjectName(QStringLiteral("valFocalLen"));
        valFocalLen->setMinimumSize(QSize(85, 0));
        valFocalLen->setMaximumSize(QSize(85, 16777215));
        valFocalLen->setDecimals(1);
        valFocalLen->setMinimum(0.1);
        valFocalLen->setMaximum(2000);
        valFocalLen->setSingleStep(0.1);
        valFocalLen->setValue(0.1);

        horizontalLayout_4->addWidget(valFocalLen);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        runFromSingleCameraLabel_2 = new QLabel(verticalWidget);
        runFromSingleCameraLabel_2->setObjectName(QStringLiteral("runFromSingleCameraLabel_2"));
        sizePolicy2.setHeightForWidth(runFromSingleCameraLabel_2->sizePolicy().hasHeightForWidth());
        runFromSingleCameraLabel_2->setSizePolicy(sizePolicy2);
        runFromSingleCameraLabel_2->setMinimumSize(QSize(145, 0));
        runFromSingleCameraLabel_2->setMaximumSize(QSize(145, 16777215));

        horizontalLayout_8->addWidget(runFromSingleCameraLabel_2);

        HFOV = new QDoubleSpinBox(verticalWidget);
        HFOV->setObjectName(QStringLiteral("HFOV"));
        HFOV->setMinimumSize(QSize(85, 0));
        HFOV->setMaximumSize(QSize(85, 16777215));
        HFOV->setDecimals(2);
        HFOV->setMinimum(1);
        HFOV->setMaximum(180);
        HFOV->setSingleStep(0.01);
        HFOV->setValue(1);

        horizontalLayout_8->addWidget(HFOV);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        text_21 = new QLabel(verticalWidget);
        text_21->setObjectName(QStringLiteral("text_21"));
        sizePolicy2.setHeightForWidth(text_21->sizePolicy().hasHeightForWidth());
        text_21->setSizePolicy(sizePolicy2);
        text_21->setMinimumSize(QSize(145, 0));
        text_21->setMaximumSize(QSize(145, 16777215));

        horizontalLayout_9->addWidget(text_21);

        VFOV = new QDoubleSpinBox(verticalWidget);
        VFOV->setObjectName(QStringLiteral("VFOV"));
        VFOV->setMinimumSize(QSize(85, 0));
        VFOV->setMaximumSize(QSize(85, 16777215));
        VFOV->setReadOnly(false);
        VFOV->setDecimals(2);
        VFOV->setMinimum(1);
        VFOV->setMaximum(180);
        VFOV->setSingleStep(0.01);
        VFOV->setValue(1);

        horizontalLayout_9->addWidget(VFOV);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        text_22 = new QLabel(verticalWidget);
        text_22->setObjectName(QStringLiteral("text_22"));
        sizePolicy2.setHeightForWidth(text_22->sizePolicy().hasHeightForWidth());
        text_22->setSizePolicy(sizePolicy2);
        text_22->setMinimumSize(QSize(145, 0));
        text_22->setMaximumSize(QSize(145, 16777215));

        horizontalLayout_10->addWidget(text_22);

        resolutions = new QComboBox(verticalWidget);
        resolutions->setObjectName(QStringLiteral("resolutions"));
        resolutions->setMinimumSize(QSize(85, 0));
        resolutions->setMaximumSize(QSize(85, 16777215));
        resolutions->setBaseSize(QSize(85, 0));

        horizontalLayout_10->addWidget(resolutions);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_10);

        label_3 = new QLabel(verticalWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        verticalLayout->addWidget(label_3);

        useFPFix = new QCheckBox(verticalWidget);
        useFPFix->setObjectName(QStringLiteral("useFPFix"));

        verticalLayout->addWidget(useFPFix);

        useDisparity = new QCheckBox(verticalWidget);
        useDisparity->setObjectName(QStringLiteral("useDisparity"));
        useDisparity->setChecked(true);

        verticalLayout->addWidget(useDisparity);

        useCorrection_1 = new QCheckBox(verticalWidget);
        useCorrection_1->setObjectName(QStringLiteral("useCorrection_1"));

        verticalLayout->addWidget(useCorrection_1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_16 = new QLabel(verticalWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        sizePolicy2.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy2);
        label_16->setMinimumSize(QSize(8, 0));

        horizontalLayout_3->addWidget(label_16);

        valA_3 = new QDoubleSpinBox(verticalWidget);
        valA_3->setObjectName(QStringLiteral("valA_3"));
        valA_3->setDecimals(5);
        valA_3->setMinimum(-10000);
        valA_3->setMaximum(10000);
        valA_3->setSingleStep(0.001);
        valA_3->setValue(0);

        horizontalLayout_3->addWidget(valA_3);

        label_17 = new QLabel(verticalWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        sizePolicy2.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy2);
        label_17->setMinimumSize(QSize(8, 0));

        horizontalLayout_3->addWidget(label_17);

        valB_3 = new QDoubleSpinBox(verticalWidget);
        valB_3->setObjectName(QStringLiteral("valB_3"));
        valB_3->setDecimals(5);
        valB_3->setMinimum(-10000);
        valB_3->setMaximum(10000);
        valB_3->setSingleStep(0.001);
        valB_3->setValue(0);

        horizontalLayout_3->addWidget(valB_3);

        label_18 = new QLabel(verticalWidget);
        label_18->setObjectName(QStringLiteral("label_18"));
        sizePolicy2.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy2);
        label_18->setMinimumSize(QSize(8, 0));

        horizontalLayout_3->addWidget(label_18);

        valC_3 = new QDoubleSpinBox(verticalWidget);
        valC_3->setObjectName(QStringLiteral("valC_3"));
        valC_3->setDecimals(5);
        valC_3->setMinimum(-10000);
        valC_3->setMaximum(10000);
        valC_3->setSingleStep(0.001);
        valC_3->setValue(0);

        horizontalLayout_3->addWidget(valC_3);

        label_19 = new QLabel(verticalWidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        sizePolicy2.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy2);
        label_19->setMinimumSize(QSize(8, 0));

        horizontalLayout_3->addWidget(label_19);

        valD_3 = new QDoubleSpinBox(verticalWidget);
        valD_3->setObjectName(QStringLiteral("valD_3"));
        valD_3->setDecimals(5);
        valD_3->setMinimum(-10000);
        valD_3->setMaximum(10000);
        valD_3->setSingleStep(0.001);
        valD_3->setValue(0);

        horizontalLayout_3->addWidget(valD_3);

        label_20 = new QLabel(verticalWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        sizePolicy2.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy2);
        label_20->setMinimumSize(QSize(8, 0));

        horizontalLayout_3->addWidget(label_20);

        valE_3 = new QDoubleSpinBox(verticalWidget);
        valE_3->setObjectName(QStringLiteral("valE_3"));
        valE_3->setDecimals(5);
        valE_3->setMinimum(-10000);
        valE_3->setMaximum(10000);
        valE_3->setSingleStep(0.001);
        valE_3->setValue(0);

        horizontalLayout_3->addWidget(valE_3);

        label_21 = new QLabel(verticalWidget);
        label_21->setObjectName(QStringLiteral("label_21"));
        sizePolicy2.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy2);
        label_21->setMinimumSize(QSize(8, 0));

        horizontalLayout_3->addWidget(label_21);

        valF_3 = new QDoubleSpinBox(verticalWidget);
        valF_3->setObjectName(QStringLiteral("valF_3"));
        valF_3->setDecimals(5);
        valF_3->setMinimum(-10000);
        valF_3->setMaximum(10000);
        valF_3->setSingleStep(0.001);
        valF_3->setValue(0);

        horizontalLayout_3->addWidget(valF_3);


        verticalLayout->addLayout(horizontalLayout_3);

        useTriangulation = new QCheckBox(verticalWidget);
        useTriangulation->setObjectName(QStringLiteral("useTriangulation"));
        useTriangulation->setEnabled(true);

        verticalLayout->addWidget(useTriangulation);

        useCorrection_2 = new QCheckBox(verticalWidget);
        useCorrection_2->setObjectName(QStringLiteral("useCorrection_2"));

        verticalLayout->addWidget(useCorrection_2);

        useCalibration = new QCheckBox(verticalWidget);
        useCalibration->setObjectName(QStringLiteral("useCalibration"));

        verticalLayout->addWidget(useCalibration);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_9 = new QLabel(verticalWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy2.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy2);
        label_9->setMinimumSize(QSize(8, 0));

        horizontalLayout->addWidget(label_9);

        valA = new QDoubleSpinBox(verticalWidget);
        valA->setObjectName(QStringLiteral("valA"));
        valA->setDecimals(5);
        valA->setMinimum(-10000);
        valA->setMaximum(10000);
        valA->setSingleStep(0.001);
        valA->setValue(0);

        horizontalLayout->addWidget(valA);

        label_8 = new QLabel(verticalWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy2.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy2);
        label_8->setMinimumSize(QSize(8, 0));

        horizontalLayout->addWidget(label_8);

        valB = new QDoubleSpinBox(verticalWidget);
        valB->setObjectName(QStringLiteral("valB"));
        valB->setDecimals(5);
        valB->setMinimum(-10000);
        valB->setMaximum(10000);
        valB->setSingleStep(0.001);
        valB->setValue(0);

        horizontalLayout->addWidget(valB);

        label_7 = new QLabel(verticalWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy2.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy2);
        label_7->setMinimumSize(QSize(8, 0));

        horizontalLayout->addWidget(label_7);

        valC = new QDoubleSpinBox(verticalWidget);
        valC->setObjectName(QStringLiteral("valC"));
        valC->setDecimals(5);
        valC->setMinimum(-10000);
        valC->setMaximum(10000);
        valC->setSingleStep(0.001);
        valC->setValue(0);

        horizontalLayout->addWidget(valC);

        label_6 = new QLabel(verticalWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy2.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy2);
        label_6->setMinimumSize(QSize(8, 0));

        horizontalLayout->addWidget(label_6);

        valD = new QDoubleSpinBox(verticalWidget);
        valD->setObjectName(QStringLiteral("valD"));
        valD->setDecimals(5);
        valD->setMinimum(-10000);
        valD->setMaximum(10000);
        valD->setSingleStep(0.001);
        valD->setValue(0);

        horizontalLayout->addWidget(valD);

        label_5 = new QLabel(verticalWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setMinimumSize(QSize(8, 0));

        horizontalLayout->addWidget(label_5);

        valE = new QDoubleSpinBox(verticalWidget);
        valE->setObjectName(QStringLiteral("valE"));
        valE->setDecimals(5);
        valE->setMinimum(-10000);
        valE->setMaximum(10000);
        valE->setSingleStep(0.001);
        valE->setValue(0);

        horizontalLayout->addWidget(valE);

        label_4 = new QLabel(verticalWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setMinimumSize(QSize(8, 0));

        horizontalLayout->addWidget(label_4);

        valF = new QDoubleSpinBox(verticalWidget);
        valF->setObjectName(QStringLiteral("valF"));
        valF->setDecimals(5);
        valF->setMinimum(-10000);
        valF->setMaximum(10000);
        valF->setSingleStep(0.001);
        valF->setValue(0);

        horizontalLayout->addWidget(valF);


        verticalLayout->addLayout(horizontalLayout);

        metric = new QCheckBox(verticalWidget);
        metric->setObjectName(QStringLiteral("metric"));
        metric->setChecked(true);

        verticalLayout->addWidget(metric);

        label_10 = new QLabel(verticalWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font);

        verticalLayout->addWidget(label_10);

        useCamCorrection = new QCheckBox(verticalWidget);
        useCamCorrection->setObjectName(QStringLiteral("useCamCorrection"));

        verticalLayout->addWidget(useCamCorrection);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_25 = new QLabel(verticalWidget);
        label_25->setObjectName(QStringLiteral("label_25"));
        sizePolicy2.setHeightForWidth(label_25->sizePolicy().hasHeightForWidth());
        label_25->setSizePolicy(sizePolicy2);
        label_25->setMinimumSize(QSize(20, 0));
        label_25->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_11->addWidget(label_25);

        camCorr_A = new QDoubleSpinBox(verticalWidget);
        camCorr_A->setObjectName(QStringLiteral("camCorr_A"));
        camCorr_A->setMinimumSize(QSize(120, 0));
        camCorr_A->setMaximumSize(QSize(120, 16777215));
        camCorr_A->setDecimals(4);
        camCorr_A->setMinimum(-10000);
        camCorr_A->setMaximum(1000);
        camCorr_A->setSingleStep(0.01);
        camCorr_A->setValue(0);

        horizontalLayout_11->addWidget(camCorr_A);

        label_26 = new QLabel(verticalWidget);
        label_26->setObjectName(QStringLiteral("label_26"));
        sizePolicy2.setHeightForWidth(label_26->sizePolicy().hasHeightForWidth());
        label_26->setSizePolicy(sizePolicy2);
        label_26->setMinimumSize(QSize(20, 0));
        label_26->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_11->addWidget(label_26);

        camCorr_B = new QDoubleSpinBox(verticalWidget);
        camCorr_B->setObjectName(QStringLiteral("camCorr_B"));
        camCorr_B->setMinimumSize(QSize(120, 0));
        camCorr_B->setMaximumSize(QSize(120, 16777215));
        camCorr_B->setDecimals(4);
        camCorr_B->setMinimum(-10000);
        camCorr_B->setMaximum(1000);
        camCorr_B->setSingleStep(0.01);
        camCorr_B->setValue(0);

        horizontalLayout_11->addWidget(camCorr_B);

        label_27 = new QLabel(verticalWidget);
        label_27->setObjectName(QStringLiteral("label_27"));
        sizePolicy2.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy2);
        label_27->setMinimumSize(QSize(20, 0));
        label_27->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_11->addWidget(label_27);

        camCorr_C = new QDoubleSpinBox(verticalWidget);
        camCorr_C->setObjectName(QStringLiteral("camCorr_C"));
        camCorr_C->setMinimumSize(QSize(120, 0));
        camCorr_C->setMaximumSize(QSize(120, 16777215));
        camCorr_C->setDecimals(4);
        camCorr_C->setMinimum(-10000);
        camCorr_C->setMaximum(1000);
        camCorr_C->setSingleStep(0.01);
        camCorr_C->setValue(0);

        horizontalLayout_11->addWidget(camCorr_C);

        label_28 = new QLabel(verticalWidget);
        label_28->setObjectName(QStringLiteral("label_28"));
        sizePolicy2.setHeightForWidth(label_28->sizePolicy().hasHeightForWidth());
        label_28->setSizePolicy(sizePolicy2);
        label_28->setMinimumSize(QSize(30, 0));
        label_28->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_11->addWidget(label_28);

        camCorr_Kdop = new QDoubleSpinBox(verticalWidget);
        camCorr_Kdop->setObjectName(QStringLiteral("camCorr_Kdop"));
        camCorr_Kdop->setMinimumSize(QSize(120, 0));
        camCorr_Kdop->setMaximumSize(QSize(120, 16777215));
        camCorr_Kdop->setDecimals(4);
        camCorr_Kdop->setMinimum(-10000);
        camCorr_Kdop->setMaximum(1000);
        camCorr_Kdop->setSingleStep(0.01);
        camCorr_Kdop->setValue(0);

        horizontalLayout_11->addWidget(camCorr_Kdop);


        verticalLayout->addLayout(horizontalLayout_11);

        ParamsWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(ParamsWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ParamsWindow->setStatusBar(statusbar);

        retranslateUi(ParamsWindow);

        QMetaObject::connectSlotsByName(ParamsWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ParamsWindow)
    {
        ParamsWindow->setWindowTitle(QApplication::translate("ParamsWindow", "Set Up", 0));
        label->setText(QApplication::translate("ParamsWindow", "Set Up", 0));
        label_2->setText(QApplication::translate("ParamsWindow", "Base Distance (mm)", 0));
        label_22->setText(QApplication::translate("ParamsWindow", "Cam1 angle (degree)", 0));
        label_23->setText(QApplication::translate("ParamsWindow", "Cam2 angle (degree)", 0));
        label_24->setText(QApplication::translate("ParamsWindow", "Cameras", 0));
        runFromSingleCameraLabel->setText(QApplication::translate("ParamsWindow", "Camera focus length (mm)", 0));
        runFromSingleCameraLabel_2->setText(QApplication::translate("ParamsWindow", "Horizontal Field of View (deg)", 0));
        text_21->setText(QApplication::translate("ParamsWindow", "Verticall Field of View (deg)", 0));
        text_22->setText(QApplication::translate("ParamsWindow", "Resolution (PXxPX)", 0));
        label_3->setText(QApplication::translate("ParamsWindow", "Algorithm", 0));
        useFPFix->setText(QApplication::translate("ParamsWindow", "Use feature points for fix", 0));
        useDisparity->setText(QApplication::translate("ParamsWindow", "Disparity", 0));
        useCorrection_1->setText(QApplication::translate("ParamsWindow", "Disparity correction", 0));
        label_16->setText(QApplication::translate("ParamsWindow", "a", 0));
        label_17->setText(QApplication::translate("ParamsWindow", "b", 0));
        label_18->setText(QApplication::translate("ParamsWindow", "c", 0));
        label_19->setText(QApplication::translate("ParamsWindow", "d", 0));
        label_20->setText(QApplication::translate("ParamsWindow", "e", 0));
        label_21->setText(QApplication::translate("ParamsWindow", "f", 0));
        useTriangulation->setText(QApplication::translate("ParamsWindow", "Triangulation", 0));
        useCorrection_2->setText(QApplication::translate("ParamsWindow", "Triangulation correction", 0));
        useCalibration->setText(QApplication::translate("ParamsWindow", "Use calibration", 0));
        label_9->setText(QApplication::translate("ParamsWindow", "a", 0));
        label_8->setText(QApplication::translate("ParamsWindow", "b", 0));
        label_7->setText(QApplication::translate("ParamsWindow", "c", 0));
        label_6->setText(QApplication::translate("ParamsWindow", "d", 0));
        label_5->setText(QApplication::translate("ParamsWindow", "e", 0));
        label_4->setText(QApplication::translate("ParamsWindow", "f", 0));
        metric->setText(QApplication::translate("ParamsWindow", "Result in meters / foot", 0));
        label_10->setText(QApplication::translate("ParamsWindow", "Camera correction", 0));
        useCamCorrection->setText(QApplication::translate("ParamsWindow", "Use camera correction", 0));
        label_25->setText(QApplication::translate("ParamsWindow", "A", 0));
        label_26->setText(QApplication::translate("ParamsWindow", "B", 0));
        label_27->setText(QApplication::translate("ParamsWindow", "C", 0));
        label_28->setText(QApplication::translate("ParamsWindow", "Kdop", 0));
    } // retranslateUi

};

namespace Ui {
    class ParamsWindow: public Ui_ParamsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMS_H
