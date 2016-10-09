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

MyQGraphicsView::MyQGraphicsView(MyQGraphicsScene *scene)

{
    m_rotation=0.0;
    m_scale=50;
    //控件宽高
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

    //尝试使用flag添加固定直线
//    QGraphicsLineItem* item = scene->addLine(QLineF(verticalLine1topView,verticalLine1ButtonView));
//    item->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);

}

//!读取图片
void MyQGraphicsView::readBackgroundPic()
{

    backgroundImage.load( "G:\\课程\\视屏监控全景融合与场景再现系统\\ExperimentData\\background.bmp" );

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
    //生成每块屏幕的图片并上传到服务器。
    //1，取得小屏幕显示区域位置（先形成观察坐标系再转世界坐标系，使用Rect就不用考虑缩放）
    //2，裁剪似乎暂时只有 QImage::copy取出某图像的某个矩形区域，无法取出旋转了的区域

    //会卡，旋转时失效，因为现在只通过两点世界坐标系。

    QProgressDialog progress("Copying images...", "Abort Copy", 0, 12, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.show();

    bool isSucceed;
    int count=1;
    for(int j=0;j<horizontalLineList.size()-1;j++){
        for(int i=0;i<verticalLineList.size()-1;i++){
            QPoint topLeftPoint(verticalLineList.at(i)/ratio,horizontalLineList.at(j)/ratio);
            QPoint bottomRightPoint(verticalLineList.at(i+1)/ratio,horizontalLineList.at(j+1)/ratio);
            QPointF topLeftPointScene = mapToScene(topLeftPoint);
            QPointF bottomRightPointScene = mapToScene(bottomRightPoint);
//            qDebug()<<topLeftPoint<<bottomRightPoint;

            QRect rect;
            rect.setTopLeft(topLeftPointScene.toPoint());
            rect.setBottomRight(bottomRightPointScene.toPoint());
            QImage patch;
            patch=backgroundImage.copy(rect);//使用copy进行处理的是背景图
            QString nameStr;
            nameStr=QString::number(count).append(".jpg");

            progress.setValue(count);

            count++;
            patch=patch.scaled(1920,1080);
            isSucceed= patch.save(nameStr,"JPG",100);
            if(!isSucceed){
                QMessageBox msgBox;
                msgBox.setText(tr("保存失败"));
                msgBox.exec();
            }
        }
    }


//    QTransform transformMat=transform();
//    patch=backgroundImage.transformed(transformMat);
//    isSucceed= patch.save("1.jpg","JPG",100);

}



void MyQGraphicsView::changeRotation(int rotation)
{
    rotate(rotation-m_rotation);
    m_rotation=rotation;
}

void MyQGraphicsView::changeScale(int value)
{
    //value的输入值为1~100
    scaleFactor;
    qreal factor=1.05;

    if(value>m_scale)
    {
        scaleFactor=pow(factor,(value-m_scale));
    }
    else
    {
        scaleFactor=pow(1/factor,(m_scale-value));
    }
    scale(scaleFactor,scaleFactor);

    m_scale=value;

}

void MyQGraphicsView::resetTransfrom()
{
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


}

void MyQGraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    //性能差 需要进行整理.
    painter->setPen(Qt::DotLine);
    painter->setPen(QPen(Qt::red));

    QPointF verticalLine1top=mapToScene(verticalLine1topView);
    QPointF verticalLine1Button=mapToScene(verticalLine1ButtonView);

    painter->drawLine(verticalLine1top,verticalLine1Button);

    QPointF verticalLine2top=mapToScene(verticalLine2topView);
    QPointF verticalLine2Button=mapToScene(verticalLine2ButtonView);

    painter->drawLine(verticalLine2top,verticalLine2Button);

    QPointF verticalLine3top=mapToScene(verticalLine3topView);
    QPointF verticalLine3Button=mapToScene(verticalLine3ButtonView);

    painter->drawLine(verticalLine3top,verticalLine3Button);

    QPointF horizontalLine1Left=mapToScene(horizontalLine1LeftView);
    QPointF horizontalLine1Right=mapToScene(horizontalLine1RightView);

    painter->drawLine(horizontalLine1Left,horizontalLine1Right);

    QPointF horizontalLine2Left=mapToScene(horizontalLine2LeftView);
    QPointF horizontalLine2Right=mapToScene(horizontalLine2RightView);

    painter->drawLine(horizontalLine2Left,horizontalLine2Right);

}

