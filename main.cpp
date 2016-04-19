#include <QApplication>
#include <QApplication>
#include <mainwindow.h>

int main(int argc, char** argv)
{
#ifdef WIN32
    // for fix bug with demo runing on windows
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    QCoreApplication::setLibraryPaths(paths);
#endif
    // contains selected source
    bool useCamera = false, useFile = false, useStereo = false;
    int camera = 0, camera2 = 0;
    std::string videoPath;

    // run dialog for select video source
    QApplication app(argc, argv);

    Ui_MainWindow* uiMainWindow;
    MainWindow* window = new MainWindow();
    window->init(&camera, &camera2, &videoPath,
                 &useCamera, &useFile, &useStereo, &uiMainWindow);
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    window->show();
    app.exec();


}
