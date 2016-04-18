QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = tracker
CONFIG -= console
CONFIG -= app_bundle
CONFIG += static



TEMPLATE = app

INCLUDEPATH += C:\opencv\build\include
INCLUDEPATH += C:\Qt\5.5\mingw492_32\include\QtANGLE

INCLUDEPATH += trackerCore\

LIBS += -LC:\opencv\build\x32\bin \
    libopencv_core310 \
    libopencv_highgui310 \
    libopencv_imgproc310 \
    libopencv_video310 \
    libopencv_videoio310 \

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
