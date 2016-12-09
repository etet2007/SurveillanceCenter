#include "centerqmainwidget.h"
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
#include "recombinationNode.h"
#include "ClientDLL.h"
#include <QGraphicsVideoItem>
#include <QMediaPlayer>

#include"NET_SDK_Encode.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


int CenterQMainWidget::table2d[5][6]={0,0,0,0,0,0,
                                      0,1,2,3,4,0,
                                      0,5,6,7,8,0,
                                      0,9,10,11,12,0,
                                      0,0, 0, 0, 0,0};

CenterQMainWidget::CenterQMainWidget(QWidget *parent) : QWidget(parent)
{
    //设置窗口大小、标题
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Surveillance Center");

    //初始化myQGraphicsScene，父控件释放后会自动释放
    myQGraphicsScene=new MyQGraphicsScene();

    //初始化myQGraphicsView
    myQGraphicsView=new MyQGraphicsView(myQGraphicsScene);
    myQGraphicsView->setParent(this);//可以自动内存释放
    myQGraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
    m_host=settings.value("serverHostUrl","http://192.168.153.174:8001/").toString();

    //初始化Url
    cuttingnodePutUrl=m_host+"cuttingnode/";
    recombinationnodePutUrl=m_host+"recombinationnode/";

    QPoint p1Standard(0,0);
    QPoint p2Standard(1920,0);
    QPoint p3Standard(1920,1080);
    QPoint p4Standard(0,1080);
    vectorStandard<<p1Standard<<p2Standard<<p3Standard<<p4Standard;
    standardPolygon=QPolygon(vectorStandard);
    //右边控制栏 mainGroupBox 包含所有东西
    QGroupBox *mainGroupBox = new QGroupBox(this);
    mainGroupBox->setFixedWidth(250);

    mainGroupBox->setTitle("工具栏");
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

    //摄像机参数列表
    cameraListWidget = new QListWidget(mainGroupBox);
    cameraListWidget->setMouseTracking(true);
    connect(cameraListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),this, SLOT(slot_getCameraFrame(QListWidgetItem*)));

    //计算按钮
    QPushButton *calcButton = new QPushButton(mainGroupBox);
    calcButton->setText(tr("生成节点参数"));
    connect(calcButton, SIGNAL(clicked(bool)), this, SLOT(slot_calcTopologicalData()));

    //更新按钮
    QPushButton *uploadDataButton = new QPushButton(mainGroupBox);

    uploadDataButton->setText(tr("上传节点参数"));
    connect(uploadDataButton, SIGNAL(clicked(bool)), this, SLOT(slot_upLoadData()));

    //上传底图按钮
    QPushButton *uploadBackgroundImageButton = new QPushButton(mainGroupBox);
    uploadBackgroundImageButton->setText("上传底图");
    connect(uploadBackgroundImageButton, SIGNAL(clicked(bool)), this, SLOT(slot_uploadBackgroundImage()));

    //图像配准按钮
//    QPushButton *openRegisterDialogButton = new QPushButton(mainGroupBox);
//    openRegisterDialogButton->setText("图像配准");
//    connect(openRegisterDialogButton, SIGNAL(clicked(bool)), this, SLOT(slot_openRegisterDialog()));


//!设置控件的Layout
    //水平Layout 加入view和mainGroupBox
    QHBoxLayout *viewLayout = new QHBoxLayout(this);
    viewLayout->addWidget(myQGraphicsView);
    viewLayout->addWidget(mainGroupBox);

//    QSizePolicy  tempSizePolicy=sizePolicy();
//    tempSizePolicy.setHeightForWidth(true);
//    setSizePolicy(tempSizePolicy);

    QVBoxLayout *rotateGroupLayout = new QVBoxLayout(rotateGroupBox);
    rotateGroupLayout->addWidget(rotateDial);

    QVBoxLayout *scaleGroupLayout = new QVBoxLayout(scaleGroupBox);
    scaleGroupLayout->addWidget(scaleSlider);

    QVBoxLayout *mainGroupLayout = new QVBoxLayout(mainGroupBox);
    mainGroupLayout->addWidget(rotateGroupBox);
    mainGroupLayout->addWidget(scaleGroupBox);
    mainGroupLayout->addWidget(resetButton);
    mainGroupLayout->addStretch(1);
//    mainGroupLayout->addWidget(openRegisterDialogButton);//应该转移到menu栏中。

    mainGroupLayout->addWidget(queryButton);
    mainGroupLayout->addWidget(cameraListWidget);

    mainGroupLayout->addStretch(1);
    mainGroupLayout->addWidget(uploadBackgroundImageButton);
    mainGroupLayout->addWidget(calcButton);
    mainGroupLayout->addWidget(uploadDataButton);


    connect(cameraListWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(slot_itemSelected(QListWidgetItem*, QListWidgetItem*)));
    connect(cameraListWidget,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(slot_itemEntered(QListWidgetItem*)));

    //初始化网络连接管理器
    m_pManager=new QNetworkAccessManager(this); //QNetworkAccessManager
    connect(m_pManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_replyFinished(QNetworkReply*)));

//测试 http post 上传底图
//    QUrl url =QUrl(m_host+"image/");
//    QNetworkRequest & uploadImageRequest=QNetworkRequest(url);
//    uploadImageRequest.setHeader(QNetworkRequest::ContentTypeHeader,"image/jpeg");
//    m_pManager->post();

    //测试用重组节点
//    for(int i=1;i<=12;i++){
//        RecombinationNode *recombinationNode=new RecombinationNode();
//        recombinationNode->setId(i);
//        recombinationNodeList.append(recombinationNode);
//    }

}

CenterQMainWidget::~CenterQMainWidget()
{
    delete myQGraphicsScene;
}

//发送全景显示范围，获取数据库中摄像头数据
void CenterQMainWidget::slot_requestData()
{
    //每次都清空存储的列表, 如果要清空，list map scene listwidget都要清空
    cuttingNodeList.clear();//应手动释放内部存储对象的内存
    recombinationNodeList.clear();
    cameraToGraphicsItemMap.clear();
    cameraToListWidgetItemMap.clear();
    myQGraphicsScene->clear();
    cameraListWidget->clear();

    //读取全景显示范围      QList<QPointF>存储
    QString areaString=myQGraphicsView->getViewArea();

    QString cuttingNodeUrl=m_host+"cuttingnode/search/"+areaString+"/?format=json";
    QString recombinationNodeUrl=m_host+"recombinationnode/?format=json";

//    qDebug()<<cuttingNodeUrl;
//    qDebug()<<recombinationNodeUrl;

    //根据URL生产request对象
    QNetworkRequest & cuttingNodeRequest=QNetworkRequest(QUrl(cuttingNodeUrl));
    QNetworkRequest & recombinationNodeRequest=QNetworkRequest(QUrl(recombinationNodeUrl));
    //发送给服务器，等待回应。主线程会异步调用
    cuttingNodeReply=m_pManager->get(cuttingNodeRequest);
    recombinationNodeReply=m_pManager->get(recombinationNodeRequest);
    connect(cuttingNodeReply,SIGNAL(readyRead()),this,SLOT(slot_cuttingNodeReplyReadyRead()));
    connect(recombinationNodeReply,SIGNAL(readyRead()),this,SLOT(slot_recombinationNodeReplyReadyRead()));

//    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
//    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));

    //测试代码-----
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

//    cuttingNodeList<<camera1<<camera2;

//    qDebug()<<camera1->getPolygonItem()->collidesWithItem(camera2->getPolygonItem(),Qt::IntersectsItemShape);
//    qDebug()<<camera1->getPolygonItem()->collidesWithItem(camera2->getPolygonItem(),Qt::IntersectsItemBoundingRect);

//    QRectF resultRect=intersectRect(camera1->getPolygonItem()->boundingRect(),camera2->getPolygonItem()->boundingRect());
//    qDebug()<<resultRect;

//    addItems();
}

//鼠标移动到item上方时，加亮四边形，移到另一个item时，取消加亮上一次加亮的四边形，使用setPen
void CenterQMainWidget::slot_itemEntered(QListWidgetItem *current)
{
    //鼠标上一时刻移动到的Entered如果不是Selected，取消加粗。
    if(cuttingNodeEntered!=NULL && cuttingNodeEntered!=cuttingNodeSelected){
        if(cameraToGraphicsItemMap.contains(cuttingNodeEntered)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cuttingNodeEntered);
        polygonItemTemp->setPen(QPen());
        }
    }

    //根据QListWidgetItem找到cuttingNode对象，并更新cuttingNodeEntered。
    cuttingNodeEntered=cameraToListWidgetItemMap.key(current);

    if(cuttingNodeEntered!=NULL&&cuttingNodeEntered!=cuttingNodeSelected){
        if(cameraToGraphicsItemMap.contains(cuttingNodeEntered)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cuttingNodeEntered);
        polygonItemTemp->setPen(QPen(Qt::red,1));
        }
    }
}


//鼠标选中item时，加亮四边形，选中另一个item时，取消加亮上一次加亮的四边形，加亮此次选中的四边形，使用GraphicsEffect
void CenterQMainWidget::slot_itemSelected(QListWidgetItem *current, QListWidgetItem *previous){
    CuttingNode* cuttingNodeTemp;
    cuttingNodeTemp=cameraToListWidgetItemMap.key(previous);

    if(cuttingNodeTemp!=NULL){//健壮性检查
        if(cameraToGraphicsItemMap.contains(cuttingNodeTemp)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cuttingNodeTemp);
        polygonItemTemp->setPen(QPen());
//      polygonItemTemp->setGraphicsEffect(0);
        }
    }

    cuttingNodeSelected=cameraToListWidgetItemMap.key(current);

    if(cuttingNodeSelected!=NULL){//健壮性检查
        if(cameraToGraphicsItemMap.contains(cuttingNodeSelected)){
        QGraphicsPolygonItem *polygonItemTemp=cameraToGraphicsItemMap.value(cuttingNodeSelected);
          polygonItemTemp->setPen(QPen(Qt::red,3));
//        polygonItemTemp->setGraphicsEffect(new QGraphicsColorizeEffect());
        }
    }
}

//!计算拓扑信息
void CenterQMainWidget::slot_calcTopologicalData(){
    //需要先清空重组节点append的数据。
    for(QListIterator<RecombinationNode*> iterator(recombinationNodeList);iterator.hasNext();){
        RecombinationNode* recombinationNodeTemp=iterator.next();
        recombinationNodeTemp->clearReceivedListAndPosition();
    }

    //对于每个切分节点都运算一次。
    for(QListIterator<CuttingNode*> iterator(cuttingNodeList);iterator.hasNext();){
        CuttingNode* cuttingNodeTemp=iterator.next();

        //!算法输入数据：摄像机的世界坐标系配置坐标                   世界坐标系 areaWorld
        QVector<QPointF> areaWorld=cuttingNodeTemp->getArea();

        //!转换为观察坐标系、视口坐标系，宽高为graphicsView的宽高。    视口坐标系 areaViewport
        //摄像机的标定坐标 观察坐标系 areaObserve
        QVector<QPointF> areaViewport(0);
        for (int i = 0; i < areaWorld.size(); ++i) {
            QPointF pTemp=myQGraphicsView->mapFromScene(areaWorld.at(i));
            areaViewport<<pTemp;
        }

        //!转换成拼接屏坐标系 QVector<QPointF> areaTV(0)          拼接屏坐标系 7680*3240
        //摄像机的标定坐标 电视墙坐标系 即为id=1的拼接屏坐标
        QVector<QPointF> areaTV(0);
        for (int i = 0; i < areaViewport.size(); ++i) {
            QPointF pTemp=areaViewport.at(i);
            pTemp.setX(pTemp.x()*myQGraphicsView->ratio);
            pTemp.setY(pTemp.y()*myQGraphicsView->ratio);
            qDebug()<<"old pTemp"<<pTemp;
            areaTV<<pTemp;
        }
//        //!转换成拼接屏坐标系 QVector<QPointF> areaTV(0)          拼接屏坐标系 7680*3240  使用归一化？
//        //摄像机的标定坐标 电视墙坐标系 即为id=1的拼接屏坐标
////        QVector<QPointF> areaTV(0);
//        for (int i = 0; i < areaViewport.size(); ++i) {
//            QPointF pTemp=areaViewport.at(i);
//            pTemp.setX(pTemp.x()/myQGraphicsView->width()*myQGraphicsView->tvWidth);
//            pTemp.setY(pTemp.y()/myQGraphicsView->height()*myQGraphicsView->tvHeight);
//            qDebug()<<"new pTemp"<<pTemp;
//            areaTV<<pTemp;
//        }

        //!计算切分节点的切分坐标 segmentXModelList segmentYModelList基于模型坐标，可能为空
        QPolygonF polygonAreaTV(areaTV);
        //显示范围的外围矩形
        QRectF polygonAreaRect=polygonAreaTV.boundingRect();
        qreal polygonAreaRectTopLeftX=polygonAreaRect.x();//四边形的外围矩形左上角点的x坐标
        qreal polygonAreaRectTopLeftY=polygonAreaRect.y();//四边形的外围矩形左上角点的y坐标
        qreal polygonAreaRectBottonRightX=polygonAreaRect.x()+polygonAreaRect.width();//四边形的外围矩形右下角点的x坐标
        qreal polygonAreaRectBottonRightY=polygonAreaRect.y()+polygonAreaRect.height();//四边形的外围矩形右下角点的y坐标

        //切分节点的切分坐标 segmentXModelList segmentYModelList 分别为x y坐标方向的
        QList<qreal> segmentXModelList,cuttingYModelList;
        //记录切分边界的Id，01234/0123
        QList<int> cuttingXTvList,cuttingYTvList;

        //垂直边界 x坐标
        for(int i=0;i<myQGraphicsView->verticalLineList.size();i++){
            int boundary= myQGraphicsView->verticalLineList.at(i);//verticalLineList : 0 1920 ...
            if(polygonAreaRectTopLeftX<boundary && boundary<polygonAreaRectBottonRightX){ //polygonAreaRectTopLeftX<boundary<polygonAreaRectBottonRightX
                segmentXModelList.append(boundary-polygonAreaRectTopLeftX);
                cuttingXTvList.append(i);
            }
        }
        //水平边界 y坐标
        for(int i=0;i<myQGraphicsView->horizontalLineList.size();i++){
            int boundary= myQGraphicsView->horizontalLineList.at(i);
            if(polygonAreaRectTopLeftY<boundary && boundary <polygonAreaRectBottonRightY){
                cuttingYModelList.append(boundary-polygonAreaRectTopLeftY);
                cuttingYTvList.append(i);
            }
        }
        cuttingNodeTemp->setCuttingXModelList(segmentXModelList);
        cuttingNodeTemp->setCuttingYModelList(cuttingYModelList);

        //切分节点需要发到哪些节点上。
        QList<int> mat;
        //左上角点
        int topLeftXcolId=calXcloId(polygonAreaRectTopLeftX);
        int TopLeftYrowId=calYrowId(polygonAreaRectTopLeftY);
        //右下角点
        int BottonRightXcldId=calXcloId(polygonAreaRectBottonRightX);
        int BottonRightYrowId=calYrowId(polygonAreaRectBottonRightY);

        int matrixCol=BottonRightXcldId-topLeftXcolId+1;
        int matrixRow=BottonRightYrowId-TopLeftYrowId+1;
//        qDebug()<<QString::number(matrixRow).toLatin1()<<"*"<<QString::number(matrixCol).toLatin1();

        for(int i=TopLeftYrowId;i<=BottonRightYrowId;i++){
            for(int j=topLeftXcolId;j<=BottonRightXcldId;j++){
               mat<<table2d[i][j];
            }
        }
        cuttingNodeTemp->setMat(mat);
        cuttingNodeTemp->setMatrixCol(matrixCol);
        cuttingNodeTemp->setMatrixRow(matrixRow);
//        qDebug()<<mat;

        //! 有覆盖的重组节点处加入信息
        //! 遍历当前切分节点的拓扑矩阵
        for(int i=0;i<mat.size();i++){
            int idFromMat= mat[i];//id :1~12还有0
            if(idFromMat==0){
                continue;
            }
            RecombinationNode* recombinationNodeTemp=recombinationNodeList[idFromMat-1]; //逐个取出重组节点
            recombinationNodeTemp->appendReceivedList(cuttingNodeTemp->getIp());//加入摄像机IP

            QVector<QPointF> fourPoints=translateById(areaTV,idFromMat);
            recombinationNodeTemp->appendFourPointsList(fourPoints);
//            qDebug()<<recombinationNodeTemp->getFourPointsList();
        }

    }//遍历切分节点循环结束。

}

QVector<QPointF> CenterQMainWidget::translateById(QVector<QPointF> areaTv,int id){
    QVector<QPointF> translatePoints(0);
    for(int i=0;i<areaTv.size();i++){
        QPointF pointF=areaTv[i];
        QPointF ans=tvToMonitor(pointF,id);
        translatePoints<<ans;
    }
    return translatePoints;
}


//计算x值的区域列编号
int CenterQMainWidget::calXcloId(qreal x)
{
    int colId=myQGraphicsView->verticalLineList.size();//列id 初始值为下方循环判断的缺失情况。 012345
    for(int i=0;i<myQGraphicsView->verticalLineList.size();i++){
        if(x <= myQGraphicsView->verticalLineList.at(i)){
         colId=i;
         break;
        }
    }
    return colId;
}
//计算y值的区域行编号
int CenterQMainWidget::calYrowId(qreal y)
{
    int rowId=myQGraphicsView->horizontalLineList.size();//行id  01234
    for(int i=0;i<myQGraphicsView->horizontalLineList.size();i++){
        if(y <= myQGraphicsView->horizontalLineList.at(i)){
         rowId=i;
         break;
        }
    }
    return rowId;
}

int CenterQMainWidget::calPointRegionId(QPointF point)
{
    qreal x=point.x();
    int colId = calXcloId(x);

    qreal y=point.y();
    int rowId = calYrowId(y);

    int id=table2d[rowId][colId];//查表获得

    return id;
}

void CenterQMainWidget::slot_uploadBackgroundImage(){
    //保存12张图片，主线程中执行
    myQGraphicsView->copyBackgroundImage();//复制背景图片到文件夹下

    QProgressDialog progress("Uploading images...", "Abort uploading", 0, 12, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.show();

    //发送到服务器上，主线程中执行
    for(QListIterator<RecombinationNode*> iterator(recombinationNodeList);iterator.hasNext();){
        RecombinationNode* recombinationNodeTemp=iterator.next();
        int id=recombinationNodeTemp->id();
        //QProgressDialog的数值
        progress.setValue(id);

        QString ip=recombinationNodeTemp->ip();
        Myhttpclient myhttpclient;

        QString imageName;
        imageName=QString::number(id).append(".jpg");
        myhttpclient.deleteid(id);
        myhttpclient.postimage(imageName.toLatin1().data(),QString::number(id).toLatin1().data(),ip.toLatin1().data());
    }

}

//接受回复
void CenterQMainWidget::slot_replyFinished(QNetworkReply *reply)
{
    if(reply->error()!=QNetworkReply::NoError){
        qDebug(QString::number(reply->error()).toLatin1());
        QMessageBox *msgBox=new QMessageBox();
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(tr("网络连接异常"));
        msgBox->show();//exec的话不能关掉
        //msgBox->exec();//Shows the dialog as a modal dialog, blocking until the user closes it. The function returns a DialogCode result.

        QTimer::singleShot(4000, msgBox, SLOT(close()));
        return;
    }

//Put数据的回调。
    if(reply==networkPutReply){
        QMessageBox *msgBox=new QMessageBox();
        msgBox->setText(tr("更新成功"));
        msgBox->show();

        QTimer::singleShot(4000, msgBox, SLOT(close()));

        reply->deleteLater();
        return;
    }

}

void CenterQMainWidget::slot_upLoadData(){
    //上传切分节点数据。
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

//            qDebug()<<jsonDocument.toJson();

        //!Put 进行更新的代码
            networkPutRequest.setUrl(QUrl(cuttingnodePutUrl+QString::number(id)));
            networkPutRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
            networkPutReply=m_pManager->put(networkPutRequest,jsonDocument.toJson());
        }

        //上传重组节点数据。
        for(QListIterator<RecombinationNode*> iterator(recombinationNodeList);iterator.hasNext();){
            RecombinationNode* recombinationNodeTemp=iterator.next();
            int id=recombinationNodeTemp->id();

            QString receivedListString;
            QString position;
            QString fourPoints;

            receivedListString=recombinationNodeTemp->getReceivedListString();
            position=recombinationNodeTemp->position();
            fourPoints=recombinationNodeTemp->getFourPointsList();

            QVariantMap variantMap;
//            variantMap.insert("id",id);
            variantMap.insert("toploMat",receivedListString);
            variantMap.insert("position",position);
            variantMap.insert("fourpoints",fourPoints);
            variantMap.insert("ispointchange",1);

            QJsonDocument jsonDocument = QJsonDocument::fromVariant(variantMap);

        //!Put 进行更新的代码
            networkPutRequest.setUrl(QUrl(recombinationnodePutUrl+(QString::number(id))));
            networkPutRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
            networkPutReply=m_pManager->put(networkPutRequest,jsonDocument.toJson());
        }

}

void CenterQMainWidget::slot_openRegisterDialog()
{
        QProcess *myProcess = new QProcess(parentWidget());
        myProcess->start("E:\\Workspace\\VisualStudioWorkspace\\互信息校准测试\\x64\\Debug\\互信息校准测试.exe");

}


void CenterQMainWidget::slot_cuttingNodeReplyReadyRead()
{
    QString all=cuttingNodeReply->readAll();

    //转为Array
    QJsonParseError json_error;
    QJsonArray qJsonArray=QJsonDocument::fromJson(all.toLatin1(),&json_error).array();

    if(json_error.error!=QJsonParseError::NoError){
        QMessageBox msgBox;
        msgBox.setText(tr("cuttingNode Json解析错误"));
           qDebug()<<all;
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

        QPointF p1(xa.toFloat(),ya.toFloat());
        QPointF p2(xb.toFloat(),yb.toFloat());
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

    cuttingNodeReply->deleteLater();
}

void CenterQMainWidget::slot_recombinationNodeReplyReadyRead(){
    QString all=recombinationNodeReply->readAll();

//"[{\"id\":1,\"ip\":\"192.168.0.1\",\"toploMat\":\"\",\"position\":\"\",\"fourpoints\":\"\",\"ispointchange\":\"1\"},{\"id\":2,\"ip\":\"192.168.0.2\",\"toploMat\":\"192.168.153.201\",\"position\":\"\",\"fourpoints\":\"700,2486,1756,67,3499,1584,2870,2908\",\"ispointchange\":\"1\"},{\"id\":3,\"ip\":\"192.168.0.3\",\"toploMat\":\"192.168.153.201\",\"position\":\"\",\"fourpoints\":\"-1219,2486,-163,67,1579,1584,950,2908\",\"ispointchange\":\"1\"},{\"id\":4,\"ip\":\"192.168.0.4\",\"toploMat\":\"\",\"position\":\"\",\"fourpoints\":\"\",\"ispointchange\":\"1\"},{\"id\":5,\"ip\":\"192.168.0.5\",\"toploMat\":\"\",\"position\":\"\",\"fourpoints\":\"\",\"ispointchange\":\"1\"},{\"id\":6,\"ip\":\"192.168.0.6\",\"toploMat\":\"192.168.153.201\",\"position\":\"\",\"fourpoints\":\"700,1406,1756,-1012,3499,504,2870,1828\",\"ispointchange\":\"1\"},{\"id\":7,\"ip\":\"192.168.0.7\",\"toploMat\":\"192.168.153.201\",\"position\":\"\",\"fourpoints\":\"-1219,1406,-163,-1012,1579,504,950,1828\",\"ispointchange\":\"1\"},{\"id\":8,\"ip\":\"192.168.0.8\",\"toploMat\":\"\",\"position\":\"\",\"fourpoints\":\"\",\"ispointchange\":\"1\"},{\"id\":9,\"ip\":\"192.168.0.9\",\"toploMat\":\"\",\"position\":\"\",\"fourpoints\":\"\",\"ispointchange\":\"1\"},{\"id\":10,\"ip\":\"192.168.0.10\",\"toploMat\":\"192.168.153.201\",\"position\":\"\",\"fourpoints\":\"700,326,1756,-2092,3499,-575,2870,748\",\"ispointchange\":\"1\"},{\"id\":11,\"ip\":\"192.168.0.11\",\"toploMat\":\"192.168.153.201\",\"position\":\"\",\"fourpoints\":\"-1219,326,-163,-2092,1579,-575,950,748\",\"ispointchange\":\"1\"},{\"id\":12,\"ip\":\"192.168.0.12\",\"toploMat\":\"\",\"position\":\"\",\"fourpoints\":\"\",\"ispointchange\":\"1\"}]"
    //转为Array
    QJsonParseError json_error;
    QJsonArray qJsonArray=QJsonDocument::fromJson(all.toLatin1(),&json_error).array();

    if(json_error.error!=QJsonParseError::NoError){
        QMessageBox msgBox;
        msgBox.setText(tr("recombinationNode Json解析错误"));
        qDebug()<<all.toLatin1();
        qDebug()<<json_error.error;
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
//            qDebug()<<id<<" "<<ip;
        recombinationNodeList<<recombinationNodeTemp;
    }

    QMessageBox *msgBox=new QMessageBox();
    QString str="接收到";
    str.append(QString::number(recombinationNodeList.length())).append("个重组节点");
    msgBox->setText(str);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->show();

    QTimer::singleShot(4000, msgBox, SLOT(close()));
    recombinationNodeReply->deleteLater();
}

void CenterQMainWidget::slot_getCameraFrame(QListWidgetItem* listWidgetItem)
{
    //获取listWidgetItem的text字符串
    QString cameraIp=listWidgetItem->text();
    //获取摄像机某帧的函数
    int isSucceed=readCamera(cameraIp);
    qDebug()<<"isSucceed "<<isSucceed;
    if(isSucceed>=0){
        qDebug()<<"isSucceed";
        QPixmap framePixmap(cameraIp+QString(".jpg"));
        if(!framePixmap.isNull()){
         QGraphicsPixmapItemOpenFFplay* pixmapItem=new QGraphicsPixmapItemOpenFFplay(framePixmap,cameraIp);
         CuttingNode* cuttingNodeTemp=cameraToListWidgetItemMap.key(listWidgetItem);
         if(cuttingNodeTemp!=NULL){

             QTransform t;
             QTransform::quadToQuad(standardPolygon,cuttingNodeTemp->getPolygonItem()->polygon(),t);

             pixmapItem->setTransform(t);//暂时这个数值不对
             myQGraphicsScene->addItem(pixmapItem);
         }
        }
    }



}

void CenterQMainWidget::addItems(){
    //获取到cameraList后，每一个CuttingNode对象生成一个键值对，对应相应的QGraphicsPolygonItem和QListWidgetItem对象
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

QRectF CenterQMainWidget::intersectRect(QRectF rect1,QRectF rect2)
{
    float topLeftX=max(rect1.x(), rect2.x());
    float width =  min(rect1.x()+rect1.width(),rect2.x()+rect2.width()) - max(rect1.x(), rect2.x());
    float topLeftY=max(rect1.y(),rect2.y());
    float height = min(rect1.y()+rect1.height(),rect2.y()+rect2.height()) - max(rect1.y(),rect2.y());

    return QRectF(topLeftX,topLeftY,width,height);
}

QPointF CenterQMainWidget::tvToMonitor(QPointF inPoint,int id)
{
    qreal x=inPoint.x();
    qreal y=inPoint.y();
    QPointF ans;
    //id to col row
    int row=(id-1)/4+1;
    int col=(id-1)%4+1;

    if(x!=0)//0是交界
    ans.setX(x - myQGraphicsView->verticalLineList.at(col-1));
    if(y!=0)
    ans.setY(y- myQGraphicsView->horizontalLineList.at(row-1));
    return ans;
}

