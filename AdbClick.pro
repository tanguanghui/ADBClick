#-------------------------------------------------
#
# Project created by QtCreator 2018-01-31T16:46:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdbClick
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpserver.cpp \
    tcpsession.cpp \
    tcpthread.cpp \
    cmdcommand.cpp

HEADERS  += mainwindow.h \
    tcpserver.h \
    tcpsession.h \
    tcpthread.h \
    cmdcommand.h

FORMS    += mainwindow.ui
