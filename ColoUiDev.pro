#-------------------------------------------------
#
# Project created by QtCreator 2016-09-11T19:31:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColoUiDev
TEMPLATE = app

MOC_DIR = MOCS
OBJECTS_DIR = OBJS

SOURCES += main.cpp\
    coloUi/colouielement.cpp \
    coloUi/colouibutton.cpp \
    coloUi/colouicontainer.cpp \
    coloUi/colouibase.cpp \
    coloUi/colouilist.cpp \
    coloUi/colouiview.cpp \
    coloUi/colouisignalmanager.cpp \
    coloUi/colouitransitionscreen.cpp \
    coloUi/colouiitem.cpp \
    coloUi/colouiconfiguration.cpp \
    colouidesigner.cpp \
    codeeditor.cpp \
    coloUi/colouicreator.cpp \
    previewwidget.cpp \
    projectcreation.cpp \
    projectbuilder.cpp \
    coloUi/colouidropdownlist.cpp \
    coloUi/colouicheckbox.cpp \
    coloUi/colouislider.cpp \
    coloUi/colouiprogressbar.cpp \
    coloUi/colouiplaceholder.cpp \
    coloUi/colouikeyboard.cpp \
    coloUi/colouimultilinetext.cpp \
    coloUi/colouilineedit.cpp \
    coloUi/colouilabel.cpp

HEADERS  += \
    coloUi/colouielement.h \
    coloUi/colouibutton.h \
    coloUi/colouicontainer.h \
    coloUi/colouidefines.h \
    coloUi/colouibase.h \
    coloUi/colouilist.h \
    coloUi/colouiview.h \
    coloUi/colouisignalmanager.h \
    coloUi/colouitransitionscreen.h \
    coloUi/colouiitem.h \
    coloUi/colouiconfiguration.h \
    colouidesigner.h \
    codeeditor.h \
    coloUi/colouicreator.h \
    previewwidget.h \
    projectcreation.h \
    projectbuilder.h \
    coloUi/colouidropdownlist.h \
    coloUi/colouicheckbox.h \
    coloUi/colouislider.h \
    coloUi/colouiprogressbar.h \
    coloUi/colouiplaceholder.h \
    coloUi/colouikeyboard.h \
    coloUi/colouimultilinetext.h \
    coloUi/colouilineedit.h \
    coloUi/colouilabel.h

FORMS    += \
    colouidesigner.ui \
    previewwidget.ui \
    projectcreation.ui \
    projectbuilder.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    assets.qrc

