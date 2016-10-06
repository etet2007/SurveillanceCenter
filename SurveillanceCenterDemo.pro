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
                    G:\curl-7.40.0-devel-mingw64\include \
                    $$PWD\lib\includePath
#源文件
SOURCES += main.cpp\
    myqgraphicsscene.cpp \
    myqgraphicsview.cpp \
    myqmainwidget.cpp \
    myqgraphicspolygonitem.cpp \
    cuttingNode.cpp \
    boundary.cpp \
    recombinationNode.cpp



#头文件
HEADERS  +=myqgraphicsscene.h \
 myqgraphicsview.h \
    myqmainwidget.h \
    myqgraphicspolygonitem.h \
    cuttingNode.h \
    recombinationnode.h\
    ClientDLL.h \
    boundary.h \
    recombinationNode.h


LIBS += G:\curl-7.40.0-devel-mingw64\lib64\libssh2.lib \
G:\curl-7.40.0-devel-mingw64\lib64\libcurl.lib \
E:\Workspace\VisualStudioWorkspace\ClientDLL\x64\Debug\ClientDLL.lib \

#静态库
LIBS += $$PWD\lib\libssh2.lib \
$$PWD\lib\libcurl.lib \
$$PWD\lib\ClientDLL.lib \
ws2_32.lib \
wldap32.lib
