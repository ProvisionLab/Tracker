#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::init(int *camera, int *camera2, std::string *videoFile,
                      bool *useCamera, bool *useFile, bool* useStereo)
{
    camera_ = camera;
    camera2_ = camera2;
    videoFile_ = videoFile;
    useCamera_ = useCamera;
    useFile_ = useFile;
    useStereo_ = useStereo;
    *useCamera_ = *useFile_ = *useStereo_ = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_video_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Video"), ".",
                                                    tr("Video Files (*.avi *.mpg *.mp4 *.mov)"));
    if (!filename.isEmpty())
    {
        *useFile_ = true;
        *videoFile_ = filename.toStdString();
        close();
    }
}

void MainWindow::on_pushButtonCamera_clicked()
{
    *useCamera_ = true;
    *camera_ = ui->cameraNumber->value();
    close();
}

void MainWindow::on_pushButtonStereo_clicked()
{
    *useStereo_ = true;
    *camera_ = ui->cam1->value();
    *camera_ = ui->cam2->value();
    close();
}
