#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T14:04:09
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SurveillanceCenterDemo
TEMPLATE = app

INCLUDEPATH +=    E:/Workspace/VisualStudioWorkspace/ClientDLL/ClientDLL \
G:/curl-7.40.0-devel-mingw64/include \
E:\LIBRARY\CH-HCNetSDK(Windows64)V5.2.3.3_build20160623\header



#SOURCE FILES
SOURCES += main.cpp \
    myqgraphicsscene.cpp \
    myqgraphicsview.cpp \
    myqgraphicspolygonitem.cpp \
    cuttingNode.cpp \
    recombinationNode.cpp \
    centerqmainwidget.cpp \
    NET_SDK_Encode.cpp \
    qgraphicspixmapitemopenffplay.cpp \
    logindialog.cpp


#HEADER FILES
HEADERS  +=myqgraphicsscene.h \
 myqgraphicsview.h \
    myqgraphicspolygonitem.h \
    cuttingNode.h \
    ClientDLL.h \
    recombinationNode.h \
    centerqmainwidget.h \
    NET_SDK_Encode.h \
    qgraphicspixmapitemopenffplay.h \
    logindialog.h

#LIB

LIBS += $$PWD/lib/libssh2.lib \
$$PWD/lib/libcurl.lib \
$$PWD/lib/ClientDLL.lib \
ws2_32.lib \
wldap32.lib \
$$PWD/lib/HCNetSDK.lib \
$$PWD/lib/PlayCtrl.lib

FORMS += \
    logindialog.ui

