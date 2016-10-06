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


INCLUDEPATH +=    E:\Workspace\VisualStudioWorkspace\ClientDLL\ClientDLL \
                    G:\curl-7.40.0-devel-mingw64\include

SOURCES += main.cpp\
    cuttingNode.cpp \
    boundary.cpp \
myqgraphicsview.cpp \
myqgraphicsscene.cpp \
myqmainwidget.cpp \
    recombinationNode.cpp

HEADERS  += \
    cuttingNode.h \
    myqgraphicsscene.h \
    myqgraphicsview.h \
    myqmainwidget.h \
    ClientDLL.h \
    boundary.h \
    recombinationNode.h


LIBS += G:\curl-7.40.0-devel-mingw64\lib64\libssh2.lib \
G:\curl-7.40.0-devel-mingw64\lib64\libcurl.lib \
E:\Workspace\VisualStudioWorkspace\ClientDLL\x64\Debug\ClientDLL.lib \
ws2_32.lib \
wldap32.lib
