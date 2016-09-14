#-------------------------------------------------
#
# Project created by QtCreator 2016-09-11T19:31:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColoUiDev
TEMPLATE = app


SOURCES += main.cpp\
        test.cpp \
    coloUi/colouielement.cpp \
    coloUi/colouisignalmanager.cpp \
    coloUi/colouibutton.cpp \
    coloUi/colouicontainer.cpp \
    coloUi/colouibase.cpp \
    coloUi/colouitext.cpp \
    coloUi/colouitextinputdialog.cpp \
    coloUi/colouilist.cpp \
    coloUi/colouiview.cpp

HEADERS  += test.h \
    coloUi/colouielement.h \
    coloUi/colouisignalmanager.h \
    coloUi/colouibutton.h \
    coloUi/colouicontainer.h \
    coloUi/colouidefines.h \
    coloUi/colouibase.h \
    coloUi/colouitext.h \
    coloUi/colouitextinputdialog.h \
    coloUi/colouilist.h \
    coloUi/colouiview.h

FORMS    += test.ui

CONFIG += mobility
MOBILITY = 

