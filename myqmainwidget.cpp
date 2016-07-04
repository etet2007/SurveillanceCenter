 #include "myqmainwidget.h"
#include "myqgraphicsview.h"
#include "myqgraphicsscene.h"
#include <QGraphicsPolygonItem>
#include <QSlider>
#include <QDial>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include <QListWidget>
#include <QGraphicsColorizeEffect>
#include <QDebug>
#include <QtNetwork>
#include <QProcess>
#include <QProgressDialog>
#include "boundary.h"
#include "recombinationnode.h"

#include "ClientDLL.h"

#include <QMediaPlayer>
#include <QGraphicsVideoItem>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


int MyQMainWidget::table[]={0,0,0,0,0,0,
                           0,1,2,3,4,0,
                           0,5,6,7,8,0,
                           0,9,10,11,12,0,
                           0,0, 0, 0, 0,0};

MyQMainWidget::MyQMainWidget(QWidget *parent) : QWidget(parent)
{
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Surveillance Center");
    //创建myQGraphicsScene对象， 父控件释放后会自动释放
    myQGraphicsScene=new MyQGraphicsScene();

    //初始化myQGraphicsView
    myQGraphicsView=new MyQGraphicsView(myQGraphicsScene);
    myQGraphicsView->setParent(this);//可以自动内存释放
    myQGraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

//    Boundary boundary=new Boundary(myQGraphicsView);

    QPointF p1Standard(0,0);
    QPointF p2Standard(1920,0);
    QPointF p3Standard(1920,1080);
    QPointF p4Standard(0,1080);
    vectorStandard<<p1Standard<<p2Standard<<p3Standard<<p4Standard;

    //右边控制栏 mainGroupBox 包含所有东西
    QGroupBox *mainGroupBox = new QGroupBox(this);
    mainGroupBox->setFixedWidth(250);

    mainGroupBox->setTitle("工具");
    //旋转控件GroupBox
    QGroupBox *rotateGroupBox = new QGroupBox (mainGroupBox);
    rotateGroupBox->setTitle("旋转");
    QDial *rotateDial = new QDial(rotateGroupBox);
    rotateDial->setRange(-180,180);
    rotateDial->setValue(0);
    rotateDial->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(rotateDial,SIGNAL(valueChanged(int)),myQGraphicsView,SLOT(changeRotation(int)));
    //缩放控件GroupBox
    QGroupBox *scaleGroupBox = new QGroupBox(mainGroupBox);
    scaleGroupBox->setTitle(tr("缩放"));
    QSlider *scaleSlider = new QSlider(Qt::Horizontal, scaleGroupBox);
    scaleSlider->setSingleStep(20);
    scaleSlider->setRange(1,100);
    scaleSlider->setValue(50);
    scaleSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(scaleSlider,SIGNAL(valueChanged(int)),myQGraphicsView,SLOT(changeScale(int)));
    //重置按钮
    QPushButton *resetButton = new QPushButton(mainGroupBox);
    resetButton->setText(tr("重置变换"));

    connect(resetButton,SIGNAL(clicked(bool)),myQGraphicsView,SLOT(resetTransfrom()));
    connect(myQGraphicsView, SIGNAL(rotationChanged(int)), rotateDial, SLOT(setValue(int)));
    connect(myQGraphicsView, SIGNAL(scaleChanged(int)), scaleSlider, SLOT(setValue(int)));
    //查询按钮
    QPushButton *queryButton = new QPushButton(mainGroupBox);
    queryButton->setText(tr("获取节点数据"));

    connect(queryButton, SIGNAL(clicked(bool)), this, SLOT(slot_requestData()));

    //摄像机参数列表~~~~~~~~~~~~~
    cameraListWidget = new QListWidget(mainGroupBox);
    cameraListWidget->setMouseTracking(true);

    //计算按钮
    QPushButton *calcButton = new QPushButton(mainGroupBox);
    calcButton->setText(tr("生成摄像机参数"));
    connect(calcButton, SIGNAL(clicked(bool)), this, SLOT(slot_getTopologicalStructureData()));

    //更新按钮
    QPushButton *uploadDataButton = new QPushButton(mainGroupBox);

    uploadDataButton->setText(tr("上传切分节点\n重组节点参数"));
    connect(uploadDataButton, SIGNAL(clicked(bool)), this, SLOT(slot_upLoadData()));

    //上传底图按钮
    QPushButton *uploadBackgroundImageButton = new QPushButton(mainGroupBox);
    uploadBackgroundImageButton->setText("上传底图");
    connect(uploadBackgroundImageButton, SIGNAL(clicked(bool)), this, SLOT(slot_uploadBackgroundImage()));

    //上传底图按钮
    QPushButton *openRegisterDialog = new QPushButton(mainGroupBox);
    openRegisterDialog->setText("图像配准");
    connect(openRegisterDialog, SIGNAL(clicked(bool)), this, SLOT(slot_openRegisterDialog()));


//!设置控件的Layout
    //水平Layout 加入view和mainGroupBox
    QHBoxLayout *viewLayout = new QHBoxLayout(this);
    viewLayout->addWidget(myQGraphicsView);
    viewLayout->addWidget(mainGroupBox);

    QVBoxLayout *rotateGroupLayout = new QVBoxLayout(rotateGroupBox);
    rotateGroupLayout->addWidget(rotateDial);

    QVBoxLayout *scaleGroupLayout = new QVBoxLayout(scaleGroupBox);
    scaleGroupLayout->addWidget(scaleSlider);

    QVBoxLayout *mainGroupLayout = new QVBoxLayout(mainGroupBox);
    mainGroupLayout->addWidget(rotateGroupBox);
    mainGroupLayout->addWidget(scaleGroupBox);
    mainGroupLayout->addWidget(openRegisterDialog);

    mainGroupLayout->addWidget(queryButton);
    mainGroupLayout->addWidget(cameraListWidget);
    mainGroupLayout->addWidget(uploadBackgroundImageButton);
    mainGroupLayout->addWidget(calcButton);
    mainGroupLayout->addWidget(uploadDataButton);


    mainGroupLayout->addStretch(1);
    mainGroupLayout->addWidget(resetButton);

    connect(cameraListWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(slot_itemSelected(QListWidgetItem*, QListWidgetItem*)));
    connect(cameraListWidget,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(slot_itemEntered(QListWidgetItem*)));

    m_pManager=new QNetworkAccessManager(this); //QNetworkAccessManager
    connect(m_pManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_replyFinished(QNetworkReply*)));

//    m_pTimer = new QTimer;

    //fake
//    for(int i=1;i<=12;i++){
//        RecombinationNode *recombinationNode=new RecombinationNode();
//        recombinationNode->setId(i);
//        recombinationNodeList.append(recombinationNode);
//    }
    m_host="http://192.168.153.209:8001/";

}

MyQMainWidget::~MyQMainWidget()
{
    delete myQGraphicsScene;
}

//发送全景显示范围，获取数据库中摄像头数据
void MyQMainWidget::slot_requestData()
{

    //每次都清空存储的列表, 如果要清空，list map scene listwidget都要清空
    cuttingNodeList.clear();//不知道会不会释放里面camera对象的内存
    recombinationNodeList.clear();

    cameraToGraphicsItemMap.clear();
    cameraToListWidgetItemMap.clear();
    myQGraphicsScene->clear();
    cameraListWidget->clear();


    //读取全景显示范围      QList<QPointF>存储
    QString areaString=myQGraphicsView->getViewArea();

    //http://192.168.153.209:8000/snippets/?format=json

    QString cuttingNodeUrl=m_host+"snippets/search/"+areaString+"/?format=json";
    QString recombinationNodeUrl=m_host+"mat/?format=json";

//    qDebug()<<cuttingNodeUrl;
//    qDebug()<<recombinationNodeUrl;

    QNetworkRequest & cuttingNodeRequest=QNetworkRequest(QUrl(cuttingNodeUrl));
    QNetworkRequest & recombinationNodeRequest=QNetworkRequest(QUrl(recombinationNodeUrl));
    //发送给服务器，等待回应。主线程会异步调用
    cuttingNoteReply=m_pManager->get(cuttingNodeRequest);
    recombinationNodeReply=m_pManager->get(recombinationNodeRequest);

    //测试收到如下坐标的四边形，转到Camera对象中
//    QVector<QPointF> vectorTest(0);
//    QPointF p1(400,0);
//    QPointF p2(500,0);
//    QPointF p3(500,200);
//    QPointF p4(200,200);
//    vectorTest<<p1<<p2<<p3<<p4;

//    CuttingNode *camera1=new CuttingNode(1,"192",vectorTest);

//    QVector<QPointF> vectorTest2(0);
//    QPointF p5(0,0);
//    QPointF p6(300,0);
//    QPointF p7(100,200);
//    QPointF p8(0,200);
//    vectorTest2<<p5<<p6<<p7<<p8;
//    CuttingNode *camera2=new CuttingNode(2,"129",vectorTest2);

//    //测试收到如下坐标的四边形，转到Camera对象中
//    QVector<QPointF> vectorTest(0);
//    QPointF p1(350,100);
//    QPointF p2(500,0);
//    QPointF p3(500,200);
//    QPointF p4(200,200);
//    vectorTest<<p1<<p2<<p3<<p4;

//    CuttingNode *camera1=new CuttingNode(1,"192",vectorTest);

//    QVector<QPointF> vectorTest2(0);
//    QPointF p5(0,0);
//    QPointF p6(300,0);
//    QPointF p7(350,99);
//    QPointF p8(0,200);
//    vectorTest2<<p5<<p6<<p7<<p8;
//    CuttingNode *camera2=new CuttingNode(2,"129",vectorTest2);

//    cuttingNodeList<<camera1<<camera2;

//    addItems();

//    qDebug()<<camera1->getPolygonItem()->collidesWithItem(camera2->getPolygonItem(),Qt::IntersectsItemShape);
//    qDebug()<<camera1->getPolygonItem()->collidesWithItem(camera2->getPolygonItem(),Qt::IntersectsItemBoundingRect);

//    QRectF resultRect=intersectRect(camera1->getPolygonItem()->boundingRect(),camera2->getPolygonItem()->boundingRect());
//    qDebug()<<resultRect;
}


//slot 函数 鼠标移动到item上方时，加亮四边形，移到另一个item时，取消加亮上一次加亮的四边形，使用setPen
void MyQMainWidget::slot_itemEntered(QListWidgetItem *current)
{
    //上一时刻是否为选中的，如果是，不进行处理
    if(cameraEntered!=NULL&&cameraEntered!=cameraSelected){
        if(cameraToGraphicsItemMap.contains(cameraEntered)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cameraEntered);
        polygonItemTemp->setPen(QPen());
        }
    }
    //跟新为这一时刻的，如果是选中的，也不需要进行处理
    cameraEntered=cameraToListWidgetItemMap.key(current);
    if(cameraEntered!=NULL&&cameraEntered!=cameraSelected){
        if(cameraToGraphicsItemMap.contains(cameraEntered)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cameraEntered);
        polygonItemTemp->setPen(QPen(Qt::red,1));
        }
    }
}


//鼠标选中item时，加亮四边形，选中另一个item时，取消加亮上一次加亮的四边形，加亮此次选中的四边形，使用GraphicsEffect
void MyQMainWidget::slot_itemSelected(QListWidgetItem *current, QListWidgetItem *previous){
    CuttingNode* cuttingNodeTemp;
    cuttingNodeTemp=cameraToListWidgetItemMap.key(previous);

    if(cuttingNodeTemp!=NULL){//健壮性检查
        if(cameraToGraphicsItemMap.contains(cuttingNodeTemp)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cuttingNodeTemp);
        polygonItemTemp->setPen(QPen());
//      polygonItemTemp->setGraphicsEffect(0);
        }
    }

    cameraSelected=cameraToListWidgetItemMap.key(current);

    if(cameraSelected!=NULL){//健壮性检查
        if(cameraToGraphicsItemMap.contains(cameraSelected)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cameraSelected);
          polygonItemTemp->setPen(QPen(Qt::red,3));
//        polygonItemTemp->setGraphicsEffect(new QGraphicsColorizeEffect());
        }
    }
}

//!计算拓扑信息
void MyQMainWidget::slot_getTopologicalStructureData()
{
    //需要先清空重组节点append的数据。
 for(QListIterator<RecombinationNode*> iterator(recombinationNodeList);iterator.hasNext();){
    RecombinationNode* recombinationNodeTemp=iterator.next();
    recombinationNodeTemp->clearReceivedListAndPosition();
    }

    //对于每个相机、和切分节点都运算一次。
    for(QListIterator<CuttingNode*> iterator(cuttingNodeList);iterator.hasNext();){

        CuttingNode* cuttingNodeTemp=iterator.next();

        //!算法输入数据：摄像机的世界坐标系标定坐标  QVector<QPointF> areaWorld
        QVector<QPointF> areaWorld=cuttingNodeTemp->getArea();

        //!转换为观察坐标系 QVector<QPointF> areaObserve(0);
        //摄像机的标定坐标 观察坐标系 areaObserve
        QVector<QPointF> areaObserve(0);
        for (int i = 0; i < areaWorld.size(); ++i) {
            QPointF pTemp=myQGraphicsView->mapFromScene(areaWorld.at(i));
            areaObserve<<pTemp;
        }

        //!转换成电视墙大坐标系 QVector<QPointF> areaTV(0)
        //摄像机的标定坐标 电视墙坐标系
        QVector<QPointF> areaTV(0);
        for (int i = 0; i < areaObserve.size(); ++i) {
            QPointF pTemp=areaObserve.at(i);
            pTemp.setX(pTemp.x()*myQGraphicsView->ratio);
            pTemp.setY(pTemp.y()*myQGraphicsView->ratio);
            areaTV<<pTemp;
        }

        //! 计算投影变换矩阵
        //求出标准摄像机图片到观察坐标系下四点坐标的投影变换矩阵result
        QTransform transformMat;
        QPolygonF polygonareaTV(areaTV);
        QPolygonF polygonStandard(vectorStandard);
        QTransform::quadToQuad(polygonStandard,polygonareaTV,transformMat);
        cuttingNodeTemp->setTransformMat(transformMat);
        /*//调试代码
        qDebug()<<QString::number(transformMat.m11()).toLatin1()<<QString::number(transformMat.m12()).toLatin1()<<
                  QString::number(transformMat.m13()).toLatin1()<<"\n"<<QString::number(transformMat.m21()).toLatin1()<<
                  QString::number(transformMat.m22()).toLatin1()<<QString::number(transformMat.m23()).toLatin1()<<"\n"<<
                  QString::number(transformMat.m31()).toLatin1()<<QString::number(transformMat.m32()).toLatin1()<<
                  QString::number(transformMat.m33()).toLatin1();
                  */

        //!添加图片
//        QTransform transformMat2;
//       // transformMat2.setMatrix(3.1894209701702181e-001,-3.6483635059433439e-001,5.5476766212957773e+002,-6.0977370239185014e-001,-8.5445514431068315e-001,3.0466756293049680e+003,-1.9137095752773869e-004,-4.3017298410226402e-004,1);
//        QTransform::quadToQuad(polygonStandard,cuttingNodeTemp->getPolygonItem()->polygon(),transformMat2);

//        QPixmap pixmap;
//        pixmap.load("C:\\Users\\Administrator\\Web\\CaptureFiles\\2016-06-04\\192.168.2.31_01_201606041119004.jpg");
//        QGraphicsPixmapItem *pixmapItem=myQGraphicsScene->addPixmap(pixmap);
//        pixmapItem->setTransform(transformMat2);


        //!计算切分节点的切分坐标 segmentXModelList segmentYModelList基于模型坐标，可能为空
        //显示范围的外围矩形
        QRectF polygonAreaRect=polygonareaTV.boundingRect();
        qreal polygonAreaRectTopLeftX=polygonAreaRect.x();//四边形的外围矩形左上角点的x坐标
        qreal polygonAreaRectTopLeftY=polygonAreaRect.y();//四边形的外围矩形左上角点的y坐标
        qreal polygonAreaRectBottonRightX=polygonAreaRect.x()+polygonAreaRect.width();//四边形的外围矩形右下角点的x坐标
        qreal polygonAreaRectBottonRightY=polygonAreaRect.y()+polygonAreaRect.height();//四边形的外围矩形右下角点的y坐标

        //切分节点的切分坐标 segmentXModelList segmentYModelList 分别为x y坐标方向的
        QList<qreal> segmentXModelList,cuttingYModelList;
        //记录被哪几个边界切分，01234/0123
        QList<int> segmentXTvList,cuttingYTvList;

        //垂直边界 x坐标
        for(int i=0;i<myQGraphicsView->verticalLineList.size();i++){
            int boundary= myQGraphicsView->verticalLineList.at(i);
            if(polygonAreaRectTopLeftX<boundary &&boundary<polygonAreaRectBottonRightX){ //polygonAreaRectTopLeftX<boundary<polygonAreaRectBottonRightX
                segmentXModelList.append(boundary-polygonAreaRectTopLeftX);
                segmentXTvList.append(i);
            }
        }
        //水平边界 y坐标
        for(int i=0;i<myQGraphicsView->horizontalLineList.size();i++){
            int boundary= myQGraphicsView->horizontalLineList.at(i);
            if(polygonAreaRectTopLeftY<boundary &&boundary <polygonAreaRectBottonRightY){
                cuttingYModelList.append(boundary-polygonAreaRectTopLeftY);
                cuttingYTvList.append(i);
            }
        }
        cuttingNodeTemp->setCuttingXModelList(segmentXModelList);
        cuttingNodeTemp->setCuttingYModelList(cuttingYModelList);
        /*//测试代码
//        for(QListIterator<qreal> ListIterator(segmentXModelList);ListIterator.hasNext();){
//             qDebug()<<QString::number(ListIterator.next()).toLatin1();
//        }
//        for(QListIterator<qreal> ListIterator(segmentYModelList);ListIterator.hasNext();){
//             qDebug()<<QString::number(ListIterator.next()).toLatin1();
//        }

//        for(QListIterator<int> segmentXListIterator(segmentXTvList);segmentXListIterator.hasNext();){
//             qDebug()<<QString::number(segmentXListIterator.next()).toLatin1();
//        }
//        for(QListIterator<int> segmentYListIterator(segmentYTvList);segmentYListIterator.hasNext();){
//            qDebug()<<QString::number(segmentYListIterator.next()).toLatin1();
//        }*/


        //!计算矩形重叠区域信息
        QList<QGraphicsItem *> graphicsItemCollidingList;
        graphicsItemCollidingList=cuttingNodeTemp->getPolygonItem()->collidingItems();//IntersectsItemBoundingRect
        for(int i=0;i<graphicsItemCollidingList.size();i++){
            qDebug()<<graphicsItemCollidingList.at(i);
        }



        //!计算切分节点拓扑信息 输入:插入x(01234) 插入y(0123)  row column
        QList<int> mat;
        // segmentYTvList.size()+1 * segmentXTvList.size()+1 的mat
        int matrixRow=cuttingYTvList.size()+1;
        int matrixCol=segmentXTvList.size()+1;
//        qDebug()<<QString::number(matrixRow).toLatin1()<<"*"<<QString::number(matrixCol).toLatin1();

        if(matrixRow==1&&matrixCol==1){
            //不需要切分的情况
            int targetId=0;
            bool isOutOfTheViewX=true;
            bool isOutOfTheViewY=true;
            for(int i=0;i<myQGraphicsView->verticalLineList.size()-1;i++){
                if(myQGraphicsView->verticalLineList.at(i) <= polygonAreaRectTopLeftX  &&
                        polygonAreaRectBottonRightX <= myQGraphicsView->verticalLineList.at(i+1)){
                    targetId=i+1;
                    isOutOfTheViewX=false;
                    break;
                }
            }
            int j=0;
            for(int i=0;i<myQGraphicsView->horizontalLineList.size()-1;i++){
                if(myQGraphicsView->horizontalLineList.at(i) <= polygonAreaRectTopLeftY &&
                        polygonAreaRectBottonRightY <= myQGraphicsView->horizontalLineList.at(i+1)){
                    targetId+=j;
                    isOutOfTheViewY=false;
                    break;
                    }
                j+=4;
                }
            if(isOutOfTheViewX||isOutOfTheViewY){
                mat<<0;
            }else{
                mat<<targetId;
            }

        }else{
            int xStart=0,yStart=0;

            if(segmentXTvList.length()!=0){
                xStart=segmentXTvList.at(0);
            }else{//垂直方向没有被切分，则要判断区间
                for(int i=0;i<myQGraphicsView->verticalLineList.size()-1;i++){
                    if(polygonAreaRectTopLeftX  >=  myQGraphicsView->verticalLineList.at(i) &&
                            polygonAreaRectBottonRightX <= myQGraphicsView->verticalLineList.at(i+1)){
                        xStart=i+1;
                        break;
                    }
                }
            }
            if(cuttingYTvList.length()!=0){
                yStart=cuttingYTvList.at(0);
            }else{//水平方向没有被切分，则要判断区间
                for(int i=0;i<myQGraphicsView->horizontalLineList.size()-1;i++){
                    if(polygonAreaRectTopLeftY  >=  myQGraphicsView->horizontalLineList.at(i) &&
                            polygonAreaRectBottonRightY <= myQGraphicsView->horizontalLineList.at(i+1)){
                        yStart=i+1;
                        break;
                        }
                    }
            }
            int xPlusY_Start=xStart+yStart*6;
            for(int i=0;i<matrixRow;i++){   //行
                for(int j=0;j<matrixCol;j++){   //列
                    mat.append(table[xPlusY_Start+i*6+j]);
                }
            }
        }
        cuttingNodeTemp->setMat(mat);
        cuttingNodeTemp->setMatrixCol(matrixCol);
        cuttingNodeTemp->setMatrixRow(matrixRow);

        /*//测试代码
        QString out="";
        for(QListIterator<int> matIterator(mat);matIterator.hasNext();){
            out.append(QString::number(matIterator.next()).toLatin1());

            if(matIterator.hasNext())
                out.append(" "); //添加空格
        }
        qDebug()<<out;*/

        //!转换成重组节点拓扑信息
        //遍历切分节点拓扑信息mat
        for(QListIterator<int> matIterator(mat);matIterator.hasNext();){
            int next=matIterator.next();
            if(next==0){
                continue;
            }
            RecombinationNode* recombinationNodeTemp;
            recombinationNodeTemp=recombinationNodeList.at(next-1);//
            recombinationNodeTemp->appendReceivedList(cuttingNodeTemp->getIp());
        }

        //!计算位置信息：对于每一个重组节点，需要计算每个切分块在对应单块电视墙当中的坐标位置

        QPointF positionPoint;
        for(int i=0;i<matrixRow;i++){
            for(int j=0;j<matrixCol;j++){
                int inWhichRecNode=mat.at(i*matrixCol+j); //mat为切分节点拓扑信息
                if(inWhichRecNode==0){
                    continue;
                }
                RecombinationNode *recombinationNodeTemp=recombinationNodeList.at(inWhichRecNode-1);
                //id to col row
                int row=(inWhichRecNode-1)/4+1;
                int col=inWhichRecNode%4;

                if(i==0&&j==0){//矩阵的第一行第一列
                    positionPoint.setX(polygonAreaRectTopLeftX);
                    positionPoint.setY(polygonAreaRectTopLeftY);
                }else if(i==0){//第一行其余
                    positionPoint.setX(0);
                    positionPoint.setY(polygonAreaRectTopLeftY);
                }else if(j==0){//第一列其余
                    positionPoint.setX(polygonAreaRectTopLeftX);
                    positionPoint.setY(0);
                }else{
                    positionPoint.setX(0);
                    positionPoint.setY(0);
                }

                qDebug()<<inWhichRecNode;
//                qDebug()<<positionPoint;
                recombinationNodeTemp->appendPosition(viewToMonitor(inWhichRecNode,positionPoint));
            }

        }

    }//遍历切分节点循环结束。

}

void MyQMainWidget::slot_uploadBackgroundImage()
{
    //保存12张图片，主线程中执行
    myQGraphicsView->copyBackgroundImage();

    QProgressDialog progress("Uploading images...", "Abort uploading", 0, 12, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.show();


    //发送到服务器上，主线程中执行
    for(QListIterator<RecombinationNode*> iterator(recombinationNodeList);iterator.hasNext();){
        RecombinationNode* recombinationNodeTemp=iterator.next();
        int id=recombinationNodeTemp->id();

        progress.setValue(id);

        QString ip=recombinationNodeTemp->ip();
        Myhttpclient myhttpclient;
//        qDebug()<<id<<" "<<ip;
        QString imageName;
        imageName=QString::number(id).append(".jpg");
        myhttpclient.deleteid(id);
        myhttpclient.postimage(imageName.toLatin1().data(),QString::number(id).toLatin1().data(),ip.toLatin1().data());
    }

}
//接受到回复
void MyQMainWidget::slot_replyFinished(QNetworkReply *reply)
{
//    qDebug("replyFinished");
    if(reply->error()!=QNetworkReply::NoError){
        qDebug(QString::number(reply->error()).toLatin1());
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("网络连接异常"));
        msgBox.exec();

        return;
    }
    //--------------------------------------------------
//Put数据的回调。
    if(reply==networkPutReply){
        QMessageBox *msgBox=new QMessageBox();
        msgBox->setText(tr("更新成功"));
        msgBox->show();

       QTimer::singleShot(4000, msgBox, SLOT(close()));

        reply->deleteLater();
        return;
    }else if(reply==cuttingNoteReply){
        QString all=reply->readAll();
//        qDebug()<<all;
        //转为Array
        QJsonParseError json_error;
        QJsonArray qJsonArray=QJsonDocument::fromJson(all.toLatin1(),&json_error).array();

        if(json_error.error!=QJsonParseError::NoError){
            QMessageBox msgBox;
            msgBox.setText(tr("Json解析错误"));
            msgBox.exec();
            return;
        }

        for(int i=0;i<qJsonArray.size();i++){
            QJsonValue value =qJsonArray.at(i);
            QJsonObject qJsonObject=value.toObject();//转换成JsonObject

            CuttingNode *cuttingNodeTemp=new CuttingNode();
            QString ip,xa,ya,xb,yb,xc,yc,xd,yd;
            int id;
            id= qJsonObject["id"].toInt();
            ip= qJsonObject["ip"].toString();
            xa=qJsonObject["xa"].toString();
            ya=qJsonObject["ya"].toString();
            xb=qJsonObject["xb"].toString();
            yb=qJsonObject["yb"].toString();
            xc=qJsonObject["xc"].toString();
            yc=qJsonObject["yc"].toString();
            xd=qJsonObject["xd"].toString();
            yd=qJsonObject["yd"].toString();

            QPointF p1(xa.toFloat(),yb.toFloat());
            QPointF p2(xb.toFloat(),ya.toFloat());
            QPointF p3(xc.toFloat(),yc.toFloat());
            QPointF p4(xd.toFloat(),yd.toFloat());
            QVector<QPointF> vectorTemp(0);

            vectorTemp<<p1<<p2<<p3<<p4;
            cuttingNodeTemp->setIp(ip);
            cuttingNodeTemp->setArea(vectorTemp);
            cuttingNodeTemp->setId(id);

            cuttingNodeList<<cuttingNodeTemp;
        }

        addItems();


        QMessageBox *msgBox=new QMessageBox();
        QString str="接收到";
        str.append(QString::number(cuttingNodeList.length())).append("个切分节点");
        msgBox->setText(str);
        msgBox->show();
        msgBox->setIcon(QMessageBox::Information);
        QTimer::singleShot(4000, msgBox, SLOT(close()));

        reply->deleteLater();

        //-----------------------------------------------------------------------------
    }else if(reply==recombinationNodeReply){

        QString all=reply->readAll();

        //转为Array
        QJsonParseError json_error;
        QJsonArray qJsonArray=QJsonDocument::fromJson(all.toLatin1(),&json_error).array();

        if(json_error.error!=QJsonParseError::NoError){
            QMessageBox msgBox;
            msgBox.setText(tr("Json解析错误"));
            msgBox.exec();
            return;
        }

        for(int i=0;i<qJsonArray.size();i++){
            QJsonValue value =qJsonArray.at(i);
            QJsonObject qJsonObject=value.toObject();//转换成JsonObject

            RecombinationNode *recombinationNodeTemp=new RecombinationNode();

            QString ip;
            int id;

            id= qJsonObject["id"].toInt();
            ip= qJsonObject["ip"].toString();

            recombinationNodeTemp->setIp(ip);

            recombinationNodeTemp->setId(id);
            qDebug()<<id<<" "<<ip;
            recombinationNodeList<<recombinationNodeTemp;
        }

        QMessageBox *msgBox=new QMessageBox();
        QString str="接收到";
        str.append(QString::number(recombinationNodeList.length())).append("个重组节点");
        msgBox->setText(str);
        msgBox->setIcon(QMessageBox::Information);
        msgBox->show();

        QTimer::singleShot(4000, msgBox, SLOT(close()));
        reply->deleteLater();
    }

}

void MyQMainWidget::slot_upLoadData()
{

        for(QListIterator<CuttingNode*> iterator(cuttingNodeList);iterator.hasNext();){
            CuttingNode* cuttingNodeTemp=iterator.next();
            int id=cuttingNodeTemp->getId();

            QString CuttingXModelListString,CuttingYModelListString,matString,transformMatString;
            CuttingXModelListString=cuttingNodeTemp->getCuttingXModelListString();
            CuttingYModelListString=cuttingNodeTemp->getCuttingYModelListString();

            matString=cuttingNodeTemp->getMatString();
            cuttingNodeTemp->getMatrixCol();
            cuttingNodeTemp->getMatrixRow();

            transformMatString=cuttingNodeTemp->getTransformMatString();

            QVariantMap variantMap;
//            variantMap.insert("id",id);
            variantMap.insert("cuttingXList",CuttingXModelListString);
            variantMap.insert("cuttingYList",CuttingYModelListString);
            variantMap.insert("toploMat",matString);
            variantMap.insert("transformMat",transformMatString);

            QJsonDocument jsonDocument = QJsonDocument::fromVariant(variantMap);

            qDebug()<<jsonDocument.toJson();

        //!Put 进行更新的代码
            QString UrlPut=m_host+"snippets/";
            UrlPut.append(QString::number(id));
            qDebug()<<UrlPut;

            networkPutRequest.setUrl(QUrl(UrlPut));
            networkPutRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
            networkPutReply=m_pManager->put(networkPutRequest,jsonDocument.toJson());
        }

        for(QListIterator<RecombinationNode*> iterator(recombinationNodeList);iterator.hasNext();){
            RecombinationNode* recombinationNodeTemp=iterator.next();
            int id=recombinationNodeTemp->id();

            QString receivedListString;
            QString position;

            receivedListString=recombinationNodeTemp->getReceivedListString();
            position=recombinationNodeTemp->position();

            QVariantMap variantMap;
//            variantMap.insert("id",id);
            variantMap.insert("toploMat",receivedListString);
            variantMap.insert("position",position);

            QJsonDocument jsonDocument = QJsonDocument::fromVariant(variantMap);

            qDebug()<<jsonDocument.toJson();

        //!Put 进行更新的代码
            QString UrlPut=m_host+"mat/";
            UrlPut.append(QString::number(id));


            networkPutRequest.setUrl(QUrl(UrlPut));
            networkPutRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
            networkPutReply=m_pManager->put(networkPutRequest,jsonDocument.toJson());
        }

}

void MyQMainWidget::slot_openRegisterDialog()
{
        QProcess *myProcess = new QProcess(parentWidget());
        myProcess->start("E:\\Workspace\\VisualStudioWorkspace\\互信息校准测试\\x64\\Debug\\互信息校准测试.exe");
}

void MyQMainWidget::addItems()
{
    //获取到cameraList后，每一个camera对象生成一个键值对，对应相应的QGraphicsPolygonItem和QListWidgetItem对象
    for(QListIterator<CuttingNode*> iterator(cuttingNodeList);iterator.hasNext();){
        CuttingNode* cameraTemp=iterator.next();

        QPolygonF polygonTemp=QPolygonF(cameraTemp->getArea());
        QGraphicsPolygonItem *polygonItemTemp= myQGraphicsScene->addPolygon(polygonTemp,QPen(Qt::black,1));
        polygonItemTemp->setCursor(QCursor(Qt::PointingHandCursor));
        cameraTemp->setPolygonItem(polygonItemTemp);
        cameraToGraphicsItemMap.insert(cameraTemp,polygonItemTemp);


        QListWidgetItem *listWidgetItemTemp=new QListWidgetItem(cameraTemp->getIp(),cameraListWidget);
        cameraToListWidgetItemMap.insert(cameraTemp,listWidgetItemTemp);
        cameraTemp->setListWidgetItem(listWidgetItemTemp);
    }
}

QRectF MyQMainWidget::intersectRect(QRectF rect1,QRectF rect2)
{
    float topLeftX=max(rect1.x(), rect2.x());
    float width =  min(rect1.x()+rect1.width(),rect2.x()+rect2.width()) - max(rect1.x(), rect2.x());
    float topLeftY=max(rect1.y(),rect2.y());
    float height =  min(rect1.y()+rect1.height(),rect2.y()+rect2.height()) - max(rect1.y(),rect2.y());

    return QRectF(topLeftX,topLeftY,width,height);
}

QPointF MyQMainWidget::viewToMonitor(int id, QPointF inPoint)
{
    qreal x=inPoint.x();
    qreal y=inPoint.y();
    QPointF ans;
    //id to col row
    int row=(id-1)/4+1;
    int col=id%4;

    if(x!=0)
    ans.setX(x - myQGraphicsView->verticalLineList.at(col-1));
    if(y!=0)
    ans.setY(y- myQGraphicsView->horizontalLineList.at(row-1));
    return ans;
}

