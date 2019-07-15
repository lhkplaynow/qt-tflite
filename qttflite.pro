#-------------------------------------------------
#
# Project created by QtCreator 2019-07-14T23:44:16
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = qttflite
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# pre-requisites: tensorflow and flatbuffers
LIBS += -L/vol3/tensorflow/bazel-bin/tensorflow/lite
LIBS += -ltensorflowlite

INCLUDEPATH += /vol3/tensorflow
INCLUDEPATH += /home/lhk/Public/flatbuffers/include

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    imagewidget.cpp \
    deepseg.cpp

HEADERS += \
    mainwindow.h \
    imagewidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
