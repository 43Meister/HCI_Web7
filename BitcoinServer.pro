
#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T09:56:20
#
#-------------------------------------------------
JASON = ../qjsonrpc/
HTTP_LIB = $${JASON}/tests/manual/build-httpclient-Desktop_Qt_5_6_0_GCC_64bit-Debug
HTTP_INC = $${JASON}/tests/manual/httpclient
LIB4CXX_LIB = /usr/lib
LIB4CXX_INC = /usr/include/log4cxx

include($${JASON}/qjsonrpc.pri)
include($${JASON}/tests/tests.pri)

CONFIG += c++14

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BitcoinServer
TEMPLATE = app


SOURCES += main.cpp \
    bitcoinserver.cpp \
    servermanager.cpp \
    settingswin.cpp \
    startservers.cpp \
    stopservers.cpp \
    show.cpp \
    commands.cpp \
    showresp.cpp \
    argwindowmine.cpp \
    logable.cpp \
    demo.cpp
        bitcoinserver.cpp

HEADERS  += bitcoinserver.h\
             $${HTTP_INC}/jasonHttpclient.h \
    servermanager.h \
    settingswin.h \
    startservers.h \
    stopservers.h \
    show.h \
    commands.h \
    showresp.h \
    argwindowmine.h \
    logable.h \
    demo.h

FORMS    += bitcoinserver.ui \
    settingswin.ui \
    startservers.ui \
    stopservers.ui \