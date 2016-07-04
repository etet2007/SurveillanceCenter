#ifndef CAMERA_H
#define CAMERA_H

#include<QVector>
#include<QPointF>
#include<QString>
#include<QGraphicsPolygonItem>
#include<QListWidgetItem>

class CuttingNode
{
public:
    CuttingNode();
    CuttingNode(int m_id,QString m_ip,QVector<QPointF> m_area);
    ~CuttingNode();

    QVector<QPointF> getArea() const;
    void setArea(QVector<QPointF> m_area);

    int getId()const;
    void setId(int m_id);

    QString getIp(void)const;
    void setIp(QString m_ip);

    QListWidgetItem *getListWidgetItem() const;
    void setListWidgetItem(QListWidgetItem *value);

    QGraphicsPolygonItem *getPolygonItem() const;
    void setPolygonItem(QGraphicsPolygonItem *value);

    QTransform getTransformMat() const;
    void setTransformMat(const QTransform &transformMat);

    QList<qreal> getCuttingXModelList() const;
    void setCuttingXModelList(const QList<qreal> &value);

    QList<qreal> getCuttingYModelList() const;
    void setCuttingYModelList(const QList<qreal> &value);

    QList<int> getMat() const;
    void setMat(const QList<int> &value);

    int getMatrixRow() const;
    void setMatrixRow(int value);

    int getMatrixCol() const;
    void setMatrixCol(int value);

    QString getCuttingXModelListString(void);
    QString getCuttingYModelListString(void);
    QString getMatString(void);
    QString getTransformMatString(void);
private:
    //input
    int m_id;  //id编号
    QString m_ip;//相机节点ip地址
    QVector<QPointF> m_area; //摄像机的显示区域

    //output
    QTransform m_transformMat;
    QList<qreal> cuttingXModelList,cuttingYModelList;
    QList<int> mat;
    int matrixRow;
    int matrixCol;

    QGraphicsPolygonItem *polygonItem; //对应图形对象的指针
    QListWidgetItem *listWidgetItem;    //对应列表对象指针

};

#endif // CAMERA_H
