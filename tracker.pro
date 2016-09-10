QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 1ACpro
CONFIG -= console
CONFIG += app_bundle
CONFIG -= static


QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_LFLAGS_RELEASE -= -O1


ICON += favicon.ico

win32 {
    INCLUDEPATH += C:\opencv\build\include
    LIBS += -LC:\opencv\build\x32\bin \
        libopencv_core310 \
        libopencv_highgui310 \
        libopencv_imgproc310 \
        libopencv_video310 \
        libopencv_videoio310 \
        libopencv_imgcodecs310 \
        libopencv_features2d310 \
        libopencv_flann310 \
        libopencv_calib3d310
} else {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_video \
        -lopencv_videoio
}

INCLUDEPATH += trackerCore\ library_gui\

SOURCES += \
    main.cpp \
    trackerCore/fhog.cpp \
    trackerCore/kcftracker.cpp \
    trackerCore/rdetector.cpp \
    selector.cpp \
    framereader.cpp \
    fpscounter.cpp \
    videogenerator.cpp \
    trackerCore/trackerAPI.cpp \
    trackerCore/detectorAPI.cpp \
    mainwindow.cpp \
    trackerthread.cpp \
    posmatcher.cpp \
    kalmanxy.cpp \
    kalmandepth.cpp \
    easychange.cpp \
    cameracalibration.cpp \
    mainwindow_gui.cpp \
    library_gui/CActiveZone.cpp \
    library_gui/CActiveZoneItem.cpp \
    library_gui/CSetLibraryTree.cpp

HEADERS += \
    trackerCore/ffttools.hpp \
    trackerCore/fhog.hpp \
    trackerCore/kcftracker.hpp \
    trackerCore/labdata.hpp \
    trackerCore/recttools.hpp \
    trackerCore/tracker.h \
    trackerCore/rdetector.h \
    trackerCore/trackerAPI.h \
    trackerCore/detectorAPI.h \
    selector.h \
    framereader.h \
    timer.h \
    fpscounter.h \
    videogenerator.h \
    mainwindow.h \
    trackerthread.h \
    posmatcher.h \
    kalmanxy.h \
    kalmandepth.h \
    easychange.h \
    cameracalibration.h \
    library_gui/CActiveZone.h \
    library_gui/CActiveZoneItem.h \
    library_gui/CSetLibraryTree.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    style.qrc

win32 {
RC_FILE += \
    app.rc
}

macx: QMAKE_INFO_PLIST = MyInfo.plist

