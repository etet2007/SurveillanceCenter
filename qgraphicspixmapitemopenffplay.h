#ifndef QGRAPHICSPIXMAPITEMOPENFFPLAY_H
#define QGRAPHICSPIXMAPITEMOPENFFPLAY_H
#include <QGraphicsPixmapItem>

class QGraphicsPixmapItemOpenFFplay : public QGraphicsPixmapItem
{
public:
    QGraphicsPixmapItemOpenFFplay();
    QGraphicsPixmapItemOpenFFplay(const QPixmap & pixmap, QString cameraIp);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
private:
    QString cameraIp;
};

#endif // QGRAPHICSPIXMAPITEMOPENFFPLAY_H
