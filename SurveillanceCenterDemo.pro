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

#目标名
TARGET = SurveillanceCenterDemo
TEMPLATE = app


INCLUDEPATH +=    E:\Workspace\VisualStudioWorkspace\ClientDLL\ClientDLL \
                    G:\curl-7.40.0-devel-mingw64\include

#源文件
SOURCES += main.cpp\
    cuttingNode.cpp \
    recombinationnode.cpp \
    boundary.cpp

#头文件
HEADERS  += \
    myqgraphicsscene.h \
    myqgraphicsview.h \
    ClientDLL.h \
    boundary.h

#静态库
LIBS += G:\curl-7.40.0-devel-mingw64\lib64\libssh2.lib \
G:\curl-7.40.0-devel-mingw64\lib64\libcurl.lib \
E:\Workspace\VisualStudioWorkspace\ClientDLL\x64\Debug\ClientDLL.lib \
ws2_32.lib \
wldap32.lib
