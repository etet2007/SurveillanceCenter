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
    //电视墙排列
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

    void slot_cuttingNodeReplyReadyRead();
    void slot_recombinationNodeReplyReadyRead();


private:
    //核心：QGraphicsView类
    MyQGraphicsView *myQGraphicsView;
    MyQGraphicsScene *myQGraphicsScene;
    //1920*1080数组
    QVector<QPointF> vectorStandard;
    //切分节点/摄像机列表
    QList<CuttingNode*> cuttingNodeList;
    //重组节点列表
    QList<RecombinationNode*>recombinationNodeList;
    //显示切分节点/摄像机列表的控件
    QListWidget *cameraListWidget;
    CuttingNode* cuttingNodeEntered;
    CuttingNode* cuttingNodeSelected;

    //Camera对象和对应的PolygonItem对象的映射
    QHash<CuttingNode*,QGraphicsPolygonItem*> cameraToGraphicsItemMap;
    QHash<CuttingNode*,QListWidgetItem*> cameraToListWidgetItemMap;

    //网络管理器
    QNetworkAccessManager * m_pManager;

    QNetworkRequest networkPutRequest;
    QNetworkReply * networkPutReply;
    QNetworkReply * cuttingNodeReply;
    QNetworkReply * recombinationNodeReply;

    //主机IP地址
    QString m_host="http://192.168.153.147:8001/";

    QString recombinationNodeUrl;

    QTimer* m_pTimer; //请求超时计时器

    //计算矩形相交区域
    QRectF intersectRect(QRectF rect1, QRectF rect2);
    //电视墙坐标系转换为单元屏坐标系
    QPointF tvToMonitor(QPointF inPoint, int id);

    QVector<QPointF> translateById(QVector<QPointF>,int id);

    void addItems();

};


#endif // MYQMAINWINDOW_H
