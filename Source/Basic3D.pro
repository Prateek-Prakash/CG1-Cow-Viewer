#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T13:53:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Basic3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += mainwindow.ui

SOILPATH = "C:/SOIL"
INCLUDEPATH += "$${SOILPATH}"
LIBS += -L"$${SOILPATH}" -lSOIL

LIBS += -lopengl32

CONFIG += static
