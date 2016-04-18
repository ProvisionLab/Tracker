QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = tracker
CONFIG -= console
CONFIG -= app_bundle
CONFIG += static



TEMPLATE = app

#INCLUDEPATH += C:\opencv\build\include
#INCLUDEPATH += C:\Qt\5.5\mingw492_32\include\QtANGLE
#INCLUDEPATH +=	/Users/iMini/Qt

INCLUDEPATH += trackerCore\
INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_video \
        -lopencv_videoio

#LIBS += /usr/local/lib/libopencv_highgui.dylib
#LIBS += /usr/local/lib/libopencv_core.dylib
#LIBS += /usr/local/lib/libopencv_imgproc.dylib
#LIBS += /usr/local/lib/libopencv_video.dylib
#LIBS += /usr/local/lib/libopencv_videoio.dylib
SOURCES += \
    main.cpp \
    trackerCore\fhog.cpp \
    trackerCore\kcftracker.cpp \
    trackerCore\rdetector.cpp \
    selector.cpp \
    framereader.cpp \
    mainwindow.cpp \
    fpscounter.cpp \
    videogenerator.cpp \
    trackerCore/trackerAPI.cpp

HEADERS += \
    trackerCore\ffttools.hpp \
    trackerCore\fhog.hpp \
    trackerCore\kcftracker.hpp \
    trackerCore\labdata.hpp \
    trackerCore\recttools.hpp \
    trackerCore\tracker.h \
    trackerCore\rdetector.h \
    trackerCore\trackerAPI.h \
    selector.h \
    framereader.h \
    mainwindow.h \
    timer.h \
    fpscounter.h \
    videogenerator.h

FORMS += \
    mainwindow.ui

macx: QMAKE_INFO_PLIST = MyInfo.plist
