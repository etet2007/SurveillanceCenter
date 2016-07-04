#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T14:04:09
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT          += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SurveillanceCenterDemo
TEMPLATE = app


INCLUDEPATH +=    E:\Workspace\VisualStudioWorkspace\ClientDLL\ClientDLL \
                    G:\curl-7.40.0-devel-mingw64\include

SOURCES += main.cpp\
    myqgraphicsscene.cpp \
    myqgraphicsview.cpp \
    myqmainwidget.cpp \
    myqgraphicspolygonitem.cpp \
    cuttingNode.cpp \
    recombinationnode.cpp \
    boundary.cpp

HEADERS  += \
    myqgraphicsscene.h \
    myqgraphicsview.h \
    myqmainwidget.h \
    myqgraphicspolygonitem.h \
    cuttingNode.h \
    recombinationnode.h\
    ClientDLL.h \
    boundary.h

#需要用到lib
LIBS += G:\curl-7.40.0-devel-mingw64\lib64\libssh2.lib \
G:\curl-7.40.0-devel-mingw64\lib64\libcurl.lib \
E:\Workspace\VisualStudioWorkspace\ClientDLL\x64\Debug\ClientDLL.lib \
ws2_32.lib \
wldap32.lib


