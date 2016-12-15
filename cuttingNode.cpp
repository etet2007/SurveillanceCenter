#include "cuttingNode.h"

CuttingNode::CuttingNode()
{

}


CuttingNode::CuttingNode(int id, QString ip, QVector<QPointF> area)
{
    this->m_id=id;
    this->m_ip=ip;
    this->m_area=area;
}

CuttingNode::~CuttingNode()
{

}


QVector<QPointF> CuttingNode::getArea() const
{
    return m_area;
}

void CuttingNode::setArea(QVector<QPointF> area)
{
    this->m_area=area;
}

int CuttingNode::getId()const{
    return m_id;
}
void CuttingNode::setId(int id){
    this->m_id=id;
}

QString CuttingNode::getIp() const
{
    return m_ip;
}

void CuttingNode::setIp(QString ip)
{
    this->m_ip=ip;
}
QListWidgetItem *CuttingNode::getListWidgetItem() const
{
    return listWidgetItem;
}

void CuttingNode::setListWidgetItem(QListWidgetItem *value)
{
    listWidgetItem = value;
}
QGraphicsPolygonItem *CuttingNode::getPolygonItem() const
{
    return polygonItem;
}

void CuttingNode::setPolygonItem(QGraphicsPolygonItem *value)
{
    polygonItem = value;
}
QTransform CuttingNode::getTransformMat() const
{
    return m_transformMat;
}

void CuttingNode::setTransformMat(const QTransform &transformMat)
{
    m_transformMat = transformMat;
}
QList<qreal> CuttingNode::getCuttingXModelList() const
{
    return cuttingXModelList;
}

void CuttingNode::setCuttingXModelList(const QList<qreal> &value)
{
    cuttingXModelList = value;
}
QList<qreal> CuttingNode::getCuttingYModelList() const
{
    return cuttingYModelList;
}

void CuttingNode::setCuttingYModelList(const QList<qreal> &value)
{
    cuttingYModelList = value;
}
QList<int> CuttingNode::getMat() const
{
    return toploMat;
}

void CuttingNode::setMat(const QList<int> &value)
{
    toploMat = value;
}
int CuttingNode::getMatrixRow() const
{
    return matrixRow;
}

void CuttingNode::setMatrixRow(int value)
{
    matrixRow = value;
}
int CuttingNode::getMatrixCol() const
{
    return matrixCol;
}

void CuttingNode::setMatrixCol(int value)
{
    matrixCol = value;
}

QString CuttingNode::getCuttingXModelListString()
{
    QString str;
    for(QListIterator<qreal> iterator(cuttingXModelList);iterator.hasNext();)
    {

        str.append(QString::number(iterator.next()));
        if(iterator.hasNext())
        {
            str.append(" ");
        }
    }
    return str;
}

QString CuttingNode::getCuttingYModelListString()
{
    QString str;
    for(QListIterator<qreal> iterator(cuttingYModelList);iterator.hasNext();)
    {

        str.append(QString::number(iterator.next()));
        if(iterator.hasNext())
        {
            str.append(" ");
        }
    }
    return str;
}

QString CuttingNode::getMatString()
{
    QString str;
    for(QListIterator<int> iterator(toploMat);iterator.hasNext();)
    {

        str.append(QString::number(iterator.next()));
        if(iterator.hasNext())
        {
            str.append(" ");
        }
    }
    return str;
}

QString CuttingNode::getTransformMatString()
{
    QString str;

    str=QString::number(m_transformMat.m11()).append(" ").
            append(QString::number(m_transformMat.m12())).append(" ").
            append(QString::number(m_transformMat.m13())).append(" ").
            append(QString::number(m_transformMat.m21())).append(" ").
            append(QString::number(m_transformMat.m22())).append(" ").
            append(QString::number(m_transformMat.m23())).append(" ").
            append(QString::number(m_transformMat.m31())).append(" ").
            append(QString::number(m_transformMat.m32())).append(" ").
            append(QString::number(m_transformMat.m33()));
    return str;

}
