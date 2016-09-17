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


#INCLUDEPATH +=    E:\Workspace\VisualStudioWorkspace\ClientDLL\ClientDLL

#源文件
SOURCES += main.cpp\
    myqgraphicsscene.cpp \
    myqgraphicsview.cpp \
    myqmainwidget.cpp \
    myqgraphicspolygonitem.cpp \
    cuttingNode.cpp \
    recombinationnode.cpp \
    boundary.cpp
#头文件
HEADERS  += \
    myqgraphicsscene.h \
    myqgraphicsview.h \
    myqmainwidget.h \
    myqgraphicspolygonitem.h \
    cuttingNode.h \
    recombinationnode.h\
    ClientDLL.h \
    boundary.h

#静态库
LIBS += ClientDLL.lib


