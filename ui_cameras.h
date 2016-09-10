/********************************************************************************
** Form generated from reading UI file 'cameras.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAS_H
#define UI_CAMERAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalWidget;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *runStereoLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSpinBox *cam2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_3;
    QSpinBox *cam1;
    QPushButton *pushButtonStereo;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CameraWindow)
    {
        if (CameraWindow->objectName().isEmpty())
            CameraWindow->setObjectName(QStringLiteral("CameraWindow"));
        CameraWindow->resize(425, 100);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CameraWindow->sizePolicy().hasHeightForWidth());
        CameraWindow->setSizePolicy(sizePolicy);
        CameraWindow->setMinimumSize(QSize(425, 100));
        CameraWindow->setMaximumSize(QSize(425, 100));
        centralwidget = new QWidget(CameraWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalWidget = new QWidget(centralwidget);
        verticalWidget->setObjectName(QStringLiteral("verticalWidget"));
        verticalWidget->setGeometry(QRect(0, 0, 421, 81));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(verticalWidget->sizePolicy().hasHeightForWidth());
        verticalWidget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(verticalWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        runStereoLabel = new QLabel(verticalWidget);
        runStereoLabel->setObjectName(QStringLiteral("runStereoLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, runStereoLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(verticalWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label_2);

        cam2 = new QSpinBox(verticalWidget);
        cam2->setObjectName(QStringLiteral("cam2"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(cam2->sizePolicy().hasHeightForWidth());
        cam2->setSizePolicy(sizePolicy3);
        cam2->setValue(0);

        horizontalLayout_3->addWidget(cam2);

        horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label_3 = new QLabel(verticalWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label_3);

        cam1 = new QSpinBox(verticalWidget);
        cam1->setObjectName(QStringLiteral("cam1"));
        sizePolicy3.setHeightForWidth(cam1->sizePolicy().hasHeightForWidth());
        cam1->setSizePolicy(sizePolicy3);
        cam1->setValue(1);

        horizontalLayout_3->addWidget(cam1);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_3);

        pushButtonStereo = new QPushButton(verticalWidget);
        pushButtonStereo->setObjectName(QStringLiteral("pushButtonStereo"));
        pushButtonStereo->setMinimumSize(QSize(75, 0));
        pushButtonStereo->setMaximumSize(QSize(75, 16777215));

        formLayout->setWidget(1, QFormLayout::FieldRole, pushButtonStereo);


        verticalLayout->addLayout(formLayout);

        CameraWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(CameraWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        CameraWindow->setStatusBar(statusbar);

        retranslateUi(CameraWindow);

        QMetaObject::connectSlotsByName(CameraWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CameraWindow)
    {
        CameraWindow->setWindowTitle(QApplication::translate("CameraWindow", "Select camera", 0));
        runStereoLabel->setText(QApplication::translate("CameraWindow", "Run stereo cameras", 0));
        label_2->setText(QApplication::translate("CameraWindow", "Camera #1", 0));
        label_3->setText(QApplication::translate("CameraWindow", "Camera #2", 0));
        pushButtonStereo->setText(QApplication::translate("CameraWindow", "Run stereo", 0));
    } // retranslateUi

};

namespace Ui {
    class CameraWindow: public Ui_CameraWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAS_H
