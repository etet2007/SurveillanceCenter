#include "qgraphicspixmapitemopenffplay.h"
#include<QDebug>
#include<QProcess>
QGraphicsPixmapItemOpenFFplay::QGraphicsPixmapItemOpenFFplay(const QPixmap &pixmap,QString cameraIp):QGraphicsPixmapItem(pixmap)
{
    this->cameraIp=cameraIp;
}

void QGraphicsPixmapItemOpenFFplay::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //测试使用ffplay打开视频
    QProcess *myProcess = new QProcess();
    QString program = "./ffmpeg/ffplay.exe";
    QString head="rtsp://admin:admin@12345@";
    QString tail=":554/h264/ch1/main/av_stream";
    QStringList arguments;
    arguments<<head+cameraIp+tail;
    qDebug()<<arguments;
    myProcess->start(program,arguments);
}

