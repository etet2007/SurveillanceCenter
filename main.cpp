#include "mainwindow.h"
#include <QApplication>
#include "centerqmainwidget.h"
#include"recombinationNode.h"

#include<QMenuBar>

#include"NET_SDK_Encode.h"

int main(int argc, char *argv[])
{
    //QApplication对象
    //The QApplication class manages the GUI application's control flow and main settings.
    QApplication qApplication(argc, argv);

    //设定字体
    QFont font("ARPL KaitiM GB",18);
    font.setBold(true);
    qApplication.setFont(font);

//    readCamera(NULL);

    CenterQMainWidget *myQMainWidget=new CenterQMainWidget();
    myQMainWidget->show();

    //后续可使用QMainWindow加入工具栏
//    QMainWindow myQMainWindow;
//    myQMainWindow.setCentralWidget(myQMainWidget);
//    QMenu * fileMenu=myQMainWindow.createPopupMenu();
//    fileMenu = myQMainWindow.menuBar()->addMenu("&File");
//    myQMainWindow.show();

    //Enters the main event loop and waits until exit() is called, then returns the value that was set to exit() (which is 0 if exit() is called via quit()).
    return qApplication.exec();
}
