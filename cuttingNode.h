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
    //Id
    int getId()const;
    void setId(int m_id);
    //Ip
    QString getIp(void)const;
    void setIp(QString m_ip);
    //ListWidgetItem
    QListWidgetItem *getListWidgetItem() const;
    void setListWidgetItem(QListWidgetItem *value);
    //QGraphicsPolygonItem
    QGraphicsPolygonItem *getPolygonItem() const;
    void setPolygonItem(QGraphicsPolygonItem *value);
    //QTransform
    QTransform getTransformMat() const;
    void setTransformMat(const QTransform &transformMat);
    QString getTransformMatString(void);
    //CuttingX x轴方向切分列表
    QList<qreal> getCuttingXModelList() const;
    void setCuttingXModelList(const QList<qreal> &value);
    QString getCuttingXModelListString(void);
    //CuttingY y轴方向切分列表
    QList<qreal> getCuttingYModelList() const;
    void setCuttingYModelList(const QList<qreal> &value);
    QString getCuttingYModelListString(void);

    QList<int> getMat() const;
    void setMat(const QList<int> &value);
    QString getMatString(void);

    int getMatrixRow() const;
    void setMatrixRow(int value);

    int getMatrixCol() const;
    void setMatrixCol(int value);


private:
    //id编号
    int m_id;
    //相机节点ip地址
    QString m_ip;
    //列表对象指针
    QListWidgetItem *listWidgetItem;
    //图形对象的指针
    QGraphicsPolygonItem *polygonItem;
    //摄像机的显示区域点列表
    QVector<QPointF> m_area;
    //切分坐标列表
    QList<qreal> cuttingXModelList;
    QList<qreal> cuttingYModelList;
    //拓扑矩阵
    QList<int> toploMat;
    int matrixRow;
    int matrixCol;

    QTransform m_transformMat;






};

#endif // CAMERA_H
