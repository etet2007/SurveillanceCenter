#include "myqgraphicsview.h"
#include "myqgraphicsscene.h"

#include <QGraphicsView>
#include <QGridLayout>
#include <QSlider>
#include <QDial>
#include<QMessageBox>
#include<QGraphicsLineItem>
#include<QDebug>
#include<QProgressDialog>
#include<QBitmap>

MyQGraphicsView::MyQGraphicsView(MyQGraphicsScene *scene)
{
    m_rotation=0.0;
    m_scale=50;
    //控件宽高,根据需求改变：http://blog.csdn.net/chlk118/article/details/24882641
    setFixedWidth(viewWidth);
    setFixedHeight(viewHeight);

    setDragMode(QGraphicsView::ScrollHandDrag);//在View中可以拖动
    initBoundary();
    readBackgroundPic();

    m_backgroundPicWidth= backgroundPic.width();
    m_backgroundPicHeight = backgroundPic.height();

    scene->setSceneRect(0,0,m_backgroundPicWidth,m_backgroundPicHeight);//设置场景边框
    setScene(scene);
//    setCacheMode(CacheBackground); //可以提高性能，但是会保存部分像素不进行改变

    //设置背景为瓷砖
    QPixmap m_tile = QPixmap(128, 128);
    m_tile.fill(Qt::white);
    QPainter pt(&m_tile);
    QColor color(230, 230, 230);
    pt.fillRect(0, 0, 64, 64, color);
    pt.fillRect(64, 64, 64, 64, color);
    pt.end();
    QPalette pal = palette();
    pal.setBrush(backgroundRole(), m_tile);
    setPalette(pal);

    //ViewportAnchor这个属性控制这当转换时候view应该如何摆放场景的位置
//    setTransformationAnchor(QGraphicsView::NoAnchor);//就是因为这个值，才会以中心点为坐标。
//    setTransform(QTransform(1,0,0 ,0,1,0 ,-3709,-1689,1));
//    qDebug()<<mapToScene(0,0);
}

//!读取图片
void MyQGraphicsView::readBackgroundPic()
{
    if(!backgroundImage.load( "G:\\课程\\视屏监控全景融合与场景再现系统\\ExperimentData\\sandboxie.jpg" )){
        QMessageBox msgBox;
        msgBox.setText(tr("读取图片失败"));
        msgBox.exec();
        return;
    }

    backgroundPic= QPixmap::fromImage( backgroundImage);//这里可以控制QImage转换成显示Pixmap的尺度
    if(backgroundPic.isNull()){
    qDebug("empty");
    }
}

QString MyQGraphicsView::getViewArea(){
    QPoint zeroPoint(0,0);
    QPointF zeroPointScene = mapToScene(zeroPoint);

    QRect viewportRect=viewport()->rect();
    qreal viewportRectWidth=viewportRect.width();
    qreal viewportRectHeight=viewportRect.height();

    QPoint topRight(viewportRectWidth,0);
    QPointF topRightScene = mapToScene(topRight);

    QPoint bottomLeft(0,viewportRectHeight);
    QPointF bottomLeftScene = mapToScene(bottomLeft);

    QPoint bottomRight(viewportRectWidth,viewportRectHeight);
    QPointF bottomRightScene = mapToScene(bottomRight);

    QString str=QString::number(zeroPointScene.x())+","+QString::number(zeroPointScene.y())+"/"+
            QString::number(topRightScene.x())+","+QString::number(topRightScene.y())+"/"+
            QString::number(bottomRightScene.x())+","+QString::number(bottomRightScene.y())+"/"+
            QString::number(bottomLeftScene.x())+","+QString::number(bottomLeftScene.y());
    return str;

//    qDebug(str.toLatin1());
//    QList<QPointF> areaList;
//    areaList<<zeroPointScene<< topRightScene <<bottomRightScene<<bottomLeftScene;
    //    return areaList;
}

void MyQGraphicsView::copyBackgroundImage()
{
    //生成每个重组节点的背景图片并上传到服务器。
    //1，取得小屏幕显示区域位置（先形成观察坐标系再转世界坐标系，使用Rect就不用考虑缩放）
    //2，裁剪似乎暂时只有 QImage::copy取出某图像的某个矩形区域，无法取出旋转了的区域

    //会卡，旋转时失效，因为现在只通过世界坐标系两点。

    QProgressDialog progress("Copying images...", "Abort Copy", 0, 12, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.show();

    //测试旋转
    QImage imageTransformed=backgroundImage.transformed(roateMat,Qt::SmoothTransformation);
//    qDebug()<<imageTransformed.width()<<imageTransformed.height();

    QMatrix tureMatrix=QImage::trueMatrix(roateMat,backgroundImage.width(),backgroundImage.height());//必须使用tureMatrix才不会出错
    //验证对tureMatrix理解的正确性，使用roateMat映射图片的四点坐标后，求外包矩形左上角。
    QRect rectTransformed =roateMat.mapRect(QRect(0,0,backgroundImage.width(),backgroundImage.height()));
    qDebug()<<"topleft"<<rectTransformed.topLeft();
    qDebug()<<"tureMatrix "<<tureMatrix;

    //保存旋转后的图片
//    imageTransformed.save("imageTransformed.jpg");

    bool isSucceed;
    int count=1;
    for(int j=0;j<horizontalLineList.size()-1;j++){
        for(int i=0;i<verticalLineList.size()-1;i++){
            //电视墙边界
            QPoint topLeftPoint(verticalLineList.at(i)/ratio,horizontalLineList.at(j)/ratio);
            QPoint bottomRightPoint(verticalLineList.at(i+1)/ratio,horizontalLineList.at(j+1)/ratio);
            //转换到世界坐标系
            QPointF topLeftPointScene = mapToScene(topLeftPoint);
            QPointF bottomRightPointScene = mapToScene(bottomRightPoint);
//            qDebug()<<"topLeftPointScene,bottomRightPointScene"<<topLeftPointScene<<bottomRightPointScene;
//            qDebug()<<mapToScene();

            //old code
            QRect rect;
            rect.setTopLeft(topLeftPointScene.toPoint());
            rect.setBottomRight(bottomRightPointScene.toPoint());
            QImage patch;
            patch=backgroundImage.copy(rect);//使用copy进行处理的是背景图
            QString nameStr="old"+QString::number(count).append(".jpg");
            patch=patch.scaled(1920,1080);
            isSucceed= patch.save(nameStr,"JPG",100);


            QRect rectTransformed;
            rectTransformed.setTopLeft(tureMatrix.map(topLeftPointScene.toPoint()));
            rectTransformed.setBottomRight(tureMatrix.map(bottomRightPointScene.toPoint()));
//            qDebug()<<matrix();
//            qDebug()<<rectTransformed;

            QImage patchTranformed=imageTransformed.copy(rectTransformed);
            QString nameStrTranformed=QString::number(count).append(".jpg");
//            qDebug()<<patchTranformed.width()<<patchTranformed.height();

            progress.setValue(count);
            count++;

            patchTranformed=patchTranformed.scaled(1920,1080,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            isSucceed= patchTranformed.save(nameStrTranformed,"JPG",100);

            if(!isSucceed){
                QMessageBox msgBox;
                msgBox.setText(tr("保存失败"));
                msgBox.exec();
            }
        }
    }

}



void MyQGraphicsView::changeRotation(int rotation)
{
    rotate(rotation-m_rotation);
    roateMat.rotate(rotation-m_rotation);
    m_rotation=rotation;
}

void MyQGraphicsView::changeScale(int value)
{
    //value的输入值为1~100
    scaleFactor;
    qreal factor=1.05;

    if(value>m_scale){
        scaleFactor=pow(factor,(value-m_scale));
    }
    else{
        scaleFactor=pow(1/factor,(m_scale-value));
    }
    scale(scaleFactor,scaleFactor);

    m_scale=value;

}

void MyQGraphicsView::resetTransfrom()
{
//    setTransform(QTransform( 0.707107,0.707107,0 , -0.707107 ,0.707107 ,0 , -955.3931275 ,-4283.7977985 ,1));
//    setTransform(QTransform( 1.5         ,                0         ,          -5963.5 ,
//                             0                       1.5                  -2702.25
//                             0                         0                         1));

    emit rotationChanged(0);
    emit scaleChanged(50);
}

//Draws the background of the scene using painter, before any items and the foreground are drawn.
//Reimplement this function to provide a custom background for the scene.
//All painting is done in scene coordinates. The rect parameter is the exposed rectangle.
void MyQGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    //sceneRect This property holds the area of the scene visualized by this view.
    painter->drawPixmap(int(sceneRect().left()),int(sceneRect().top()), backgroundPic);//画出背景图

//    Returns the current transformation matrix for the view.
//    qDebug()<<"matrix:"<<matrix().m11()<<matrix().m12()<<0<<";"<<matrix().m21()<<matrix().m22()<<0<<";"<<matrix().dx()<<matrix().dy()<<1;
//    Returns a matrix that maps viewport coordinates to scene coordinates.
    qDebug()<<"viewportTransform:"<<viewportTransform().m11()<<viewportTransform().m12()<<viewportTransform().m13()<<";"<<viewportTransform().m21()<<viewportTransform().m22()
           <<viewportTransform().m23()<<";"<<viewportTransform().m31()<<viewportTransform().m32()<<viewportTransform().m33();
    qDebug()<<"viewportTransform,inverted:"<<viewportTransform().inverted().m11()<<viewportTransform().inverted().m12()<<viewportTransform().inverted().m13()<<";"<<viewportTransform().inverted().m21()<<viewportTransform().inverted().m22()
           <<viewportTransform().inverted().m23()<<";"<<viewportTransform().inverted().m31()<<viewportTransform().inverted().m32()<<viewportTransform().inverted().m33();
    qDebug()<<"roateMat:"<<roateMat;
    qDebug()<<"the point in the middle:"<<mapToScene(width()/2,height()/2);
    qDebug()<<"translate:"<<mapToScene(0,0);
    qDebug()<<"Scene coord:(100,100)    viewport:"<<mapFromScene(100,100);
    qDebug()<<" ";
}

void MyQGraphicsView::initBoundary(){
    verticalLine1topView=QPoint(qreal(1920)/tvWidth*viewWidth,0);
    verticalLine1ButtonView=QPoint(qreal(1920)/tvWidth*viewWidth,qreal(3240)/tvHeight*viewHeight);

    verticalLine2topView=QPoint(qreal(3840)/tvWidth*viewWidth,0);
    verticalLine2ButtonView=QPoint(qreal(3840)/tvWidth*viewWidth,qreal(3240)/tvHeight*viewHeight);

    verticalLine3topView=QPoint(qreal(5760)/tvWidth*viewWidth,0);
    verticalLine3ButtonView=QPoint(qreal(5760)/tvWidth*viewWidth,qreal(3240)/tvHeight*viewHeight);

    horizontalLine1LeftView=QPoint(0,qreal(1080)/tvHeight*viewHeight);
    horizontalLine1RightView=QPoint(qreal(7680)/tvWidth*viewWidth,qreal(1080)/tvHeight*viewHeight);

    horizontalLine2LeftView=QPoint(0,qreal(2160)/tvHeight*viewHeight);
    horizontalLine2RightView=QPoint(qreal(7680)/tvWidth*viewWidth,qreal(2160)/tvHeight*viewHeight);


    verticalLineList.append(verticalLine1);
    verticalLineList.append(verticalLine2);
    verticalLineList.append(verticalLine3);
    verticalLineList.append(verticalLine4);
    verticalLineList.append(verticalLine5);

    horizontalLineList.append(horizontalLine1);
    horizontalLineList.append(horizontalLine2);
    horizontalLineList.append(horizontalLine3);
    horizontalLineList.append(horizontalLine4);
}

void MyQGraphicsView::paintEvent(QPaintEvent *event){
    QGraphicsView::paintEvent(event);

    //画边界的新函数
    QPainter painter(this->viewport());
    painter.setPen(QPen(Qt::red,4));
    painter.drawLine(verticalLine1topView,verticalLine1ButtonView);
    painter.drawLine(verticalLine2topView,verticalLine2ButtonView);
    painter.drawLine(verticalLine3topView,verticalLine3ButtonView);
    painter.drawLine(horizontalLine1LeftView,horizontalLine1RightView);
    painter.drawLine(horizontalLine2LeftView,horizontalLine2RightView);
}


