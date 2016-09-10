#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <mainwindow.h>
#include <iostream>


int main(int argc, char** argv)
{
//    QApplication::setAttribute(Qt::AA_MacPluginApplication);
    QApplication app(argc, argv);
 //   app.setAttribute(Qt::AA_DontUseNativeMenuBar);

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
      //QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    MainWindow* window = new MainWindow();
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    window->show();
    return app.exec();
}
