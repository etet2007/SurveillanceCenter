#include "mainwindow.h"
#include <QApplication>
#include "centerqmainwidget.h"
//#include"recombinationNode.h"

#include<QMenuBar>
#include <QSplashScreen>
#include "logindialog.h"

//use for test
#include"myqgraphicsscene.h"
#include"myqgraphicsview.h"

int main(int argc, char *argv[])
{
    //QApplication对象
    //The QApplication class manages the GUI application's control flow and main settings.
    QApplication qApplication(argc, argv);


    //设定字体
    QFont font("ARPL KaitiM GB",18);
    font.setBold(true);
    qApplication.setFont(font);

    QPixmap pixmap("splash.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
    qApplication.processEvents();

//    LoginDialog loginDialog;
//    if(loginDialog.exec()==QDialog::Accepted)
//    {
//        CenterQMainWidget *myQMainWidget=new CenterQMainWidget();
//        myQMainWidget->show();
//    }else{
//        splash.finish(&loginDialog);
//        return 0;
//    }

    CenterQMainWidget *myQMainWidget=new CenterQMainWidget();
    myQMainWidget->show();
    //使用QMainWindow加入工具栏
//    QMainWindow myQMainWindow;
//    myQMainWindow.setCentralWidget(myQMainWidget);
//    QMenu * settingMenu=myQMainWindow.createPopupMenu();
//    settingMenu = myQMainWindow.menuBar()->addMenu("&Setting");
//    settingMenu->addAction("Setting");

//    //    QMenu * aboutMenu=myQMainWindow.createPopupMenu();
//    //    aboutMenu = myQMainWindow.menuBar()->addMenu("&About");
//    myQMainWindow.show();

    //Enters the main event loop and waits until exit() is called, then returns the value that was set to exit() (which is 0 if exit() is called via quit()).
    return qApplication.exec();
}
