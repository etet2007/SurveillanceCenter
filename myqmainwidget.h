#ifndef MYQMAINWINDOW_H
#define MYQMAINWINDOW_H

#include <QMainWindow>


#include "cuttingNode.h"

class RecombinationNode;
#include<QNetworkRequest>
//前置声明
class QGraphicsPolygonItem;
class MyQGraphicsView;
class MyQGraphicsScene;

class QNetworkReply;
class QNetworkAccessManager;
//class QNetworkRequest;
class QListWidget;

class MyQMainWidget : public QWidget
{
    Q_OBJECT
public:
    //声明为explicit的构造函数不能在隐式转换中使用。
    explicit MyQMainWidget(QWidget *parent = 0);
    ~MyQMainWidget();

static int table[30];



signals:

public slots:
    void slot_requestData();
    void slot_itemEntered(QListWidgetItem* current);
    void slot_itemSelected(QListWidgetItem* current, QListWidgetItem *previous);
    void slot_getTopologicalStructureData();
    void slot_uploadBackgroundImage();
    void slot_replyFinished(QNetworkReply *);
    void slot_upLoadData();
    void slot_openRegisterDialog();
private:
    MyQGraphicsView *myQGraphicsView;
    MyQGraphicsScene *myQGraphicsScene;

    QVector<QPointF> vectorStandard;
    //摄像机列表
    QList<CuttingNode*> cuttingNodeList;
    QList<RecombinationNode*>recombinationNodeList;
    //显示摄像机列表的控件
    QListWidget *cameraListWidget;

    //映射Camera对象和对应的PolygonItem对象
    QHash<CuttingNode*,QGraphicsPolygonItem*> cameraToGraphicsItemMap;
    QHash<CuttingNode*,QListWidgetItem*> cameraToListWidgetItemMap;

    CuttingNode* cameraEntered;
    CuttingNode* cameraSelected;

    QNetworkAccessManager * m_pManager;

    QNetworkRequest networkPutRequest;
    QNetworkReply * networkPutReply;

    QNetworkReply * cuttingNoteReply;
    QNetworkReply * recombinationNodeReply;
    QString m_host;

    QString recombinationNodeUrl;

    QTimer* m_pTimer; //请求超时计时器
    void addItems();
    QRectF intersectRect(QRectF rect1, QRectF rect2);
    QPointF viewToMonitor(int id,QPointF inPoint);
};


#endif // MYQMAINWINDOW_H
