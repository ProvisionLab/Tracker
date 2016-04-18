#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void init(int* camera, int* camera2, std::string* videoFile,
              bool* useCamera, bool* useFile, bool *useStereo);
    ~MainWindow();

private slots:
    void on_pushButton_video_clicked();
    void on_pushButtonCamera_clicked();

    void on_pushButtonStereo_clicked();

private:
    Ui::MainWindow *ui;
    int* camera_;
    int* camera2_;
    std::string* videoFile_;
    bool* useCamera_;
    bool* useFile_;
    bool* useStereo_;
};

#endif // MAINWINDOW_H
