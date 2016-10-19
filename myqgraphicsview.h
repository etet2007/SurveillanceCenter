#ifndef BACKGROUNDVIEW_H
#define BACKGROUNDVIEW_H

#include <QGraphicsView>


class MyQGraphicsScene;

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyQGraphicsView(MyQGraphicsScene *scene);
//生成可视区域范围字符串
    QString getViewArea();
//常量
    //视口分辨率
    static const int viewHeight=675;
    static const int viewWidth=1600;
    //电视墙分辨率
    static const int tvHeight=3240;
    static const int tvWidth=7680;

    float ratio=4.8;

//已经将下放的常量放入列表当中
    QList<int>  verticalLineList;
    QList<int>  horizontalLineList;

    //电视墙坐标系下转换为观察坐标系用比例变换
    static const int verticalLine1=0;
    static const int verticalLine2=1920;
    static const int verticalLine3=3840;
    static const int verticalLine4=5760;
    static const int verticalLine5=7680;

    static const int horizontalLine1=0;
    static const int horizontalLine2=1080;
    static const int horizontalLine3=2160;
    static const int horizontalLine4=3240;

    void copyBackgroundImage();

public slots:
    //旋转
    void changeRotation(int rotation);
    //缩放
    void changeScale(int scale);
    //重置变换
    void resetTransfrom();

signals:
    void rotationChanged(int rotation);
    void scaleChanged(int scale);

protected:
    void drawBackground(QPainter * painter, const QRectF & rect);

//    void render(QPainter * painter, const QRectF & target = QRectF(), const QRect & source = QRect(), Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio);//用于截屏
private:
    void MyQGraphicsView::paintEvent(QPaintEvent *event);
    //读取背景图
    void readBackgroundPic();
    void initBoundary();

    QImage backgroundImage;
    QPixmap backgroundPic;//QPixmap
    int m_backgroundPicWidth;
    int m_backgroundPicHeight;

    qreal m_rotation;
    qreal m_scale;
    qreal scaleFactor;
    //用于在View中显示电视墙边界线。把电视墙坐标系转换成观察坐标系。
    QPoint verticalLine1topView;
    QPoint verticalLine1ButtonView;

    QPoint verticalLine2topView;
    QPoint verticalLine2ButtonView;

    QPoint verticalLine3topView;
    QPoint verticalLine3ButtonView;

    QPoint horizontalLine1LeftView;
    QPoint horizontalLine1RightView;

    QPoint horizontalLine2LeftView;
    QPoint horizontalLine2RightView;


};

#endif // BACKGROUNDVIEW_H
