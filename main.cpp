#include "mainwindow.h"
#include <QApplication>
//#include "myqgraphicsview.h"
//#include "myqgraphicsscene.h"
#include "myqmainwidget.h"

//#include<QTextCodec>
#include"recombinationnode.h"


int main(int argc, char *argv[])
{
    //QApplication对象
    //The QApplication class manages the GUI application's control flow and main settings.
    QApplication qApplication(argc, argv);

    QFont font("ARPL KaitiM GB",18);
    font.setBold(true);
    qApplication.setFont(font);

    //实现方法一 在MyQGraphicsView的构造函数中创建scene对象，再把其设置为view的scene
//    MyQGraphicsView myQGraphicsView;
//    myQGraphicsView.show();

    //实现方法二
//    MyQGraphicsScene *myQGraphicsScene=new MyQGraphicsScene();
//    MyQGraphicsView *myQGraphicsView=new MyQGraphicsView(myQGraphicsScene);


    MyQMainWidget *myQMainWindow=new MyQMainWidget();
    myQMainWindow->show();


    //Enters the main event loop and waits until exit() is called, then returns the value that was set to exit() (which is 0 if exit() is called via quit()).
    return qApplication.exec();
}
