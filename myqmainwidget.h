#ifndef MYQMAINWINDOW_H
#define MYQMAINWINDOW_H

#include <QMainWindow>
#include<QNetworkRequest>
#include "cuttingNode.h"

//前置声明
class RecombinationNode;
class QGraphicsPolygonItem;
class MyQGraphicsView;
class MyQGraphicsScene;
class QNetworkReply;
class QNetworkAccessManager;
class QListWidget;

//主控件类
class MyQMainWidget : public QWidget
{
    Q_OBJECT
public:
    //声明为explicit的构造函数不能在隐式转换中使用。
    explicit MyQMainWidget(QWidget *parent = 0);
    ~MyQMainWidget();

static int table[30];

signals:
//槽函数
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
    //核心：QGraphicsView类
    MyQGraphicsView *myQGraphicsView;
    MyQGraphicsScene *myQGraphicsScene;

    QVector<QPointF> vectorStandard;
    //切分结点/摄像机列表
    QList<CuttingNode*> cuttingNodeList;
    //重组结点列表
    QList<RecombinationNode*>recombinationNodeList;
    //显示摄像机列表的控件
    QListWidget *cameraListWidget;
    //Camera对象和对应的PolygonItem对象的映射
    QHash<CuttingNode*,QGraphicsPolygonItem*> cameraToGraphicsItemMap;
    QHash<CuttingNode*,QListWidgetItem*> cameraToListWidgetItemMap;

    CuttingNode* cameraEntered;
    CuttingNode* cameraSelected;
    //网络管理器
    QNetworkAccessManager * m_pManager;

    QNetworkRequest networkPutRequest;
    QNetworkReply * networkPutReply;

    QNetworkReply * cuttingNoteReply;
    QNetworkReply * recombinationNodeReply;
    //主机IP地址
    QString m_host="http://192.168.153.116:8001/";

    QString recombinationNodeUrl;

    QTimer* m_pTimer; //请求超时计时器

    QRectF intersectRect(QRectF rect1, QRectF rect2);
    QPointF tvToMonitor(QPointF inPoint, int id);
    QVector<QPointF> translateById(QVector<QPointF>,int id);

    void addItems();

};


#endif // MYQMAINWINDOW_H
