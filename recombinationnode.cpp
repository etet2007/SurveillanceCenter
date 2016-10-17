#include "recombinationNode.h"

RecombinationNode::RecombinationNode()
{

}

RecombinationNode::~RecombinationNode()
{

}
//void function()const; //常成员函数, 它不改变对象的成员变量.
QList<QString> RecombinationNode::receivedList()const
{
    return m_receivedList;
}
//参数为引用，为了增加效率同时防止修改。void function(const Class& Var);
//禁止对引用的对象的一切修改,唯一不同的是按值传递会先建立一个类对象的副本, 然后传递过去,
//而它直接传递地址,所以这种传递比按值传递更有效.另外只有引用的const传递可以传递一个临时对象,因为临时对象都是const属性, 且是不可见的,他短时间存在一个局部域中,所以不能使用指针,只有引用的const传递能够捕捉到这个家伙.
void RecombinationNode::setReceivedList(const QList<QString> &receivedList)
{
    m_receivedList = receivedList;
}
int RecombinationNode::id() const
{
    return m_id;
}

void RecombinationNode::setId(int id)
{
    m_id = id;
}

void RecombinationNode::appendReceivedList(QString &string)
{
    m_receivedList.append(string);
}
QString RecombinationNode::ip() const
{
    return m_ip;
}

void RecombinationNode::setIp(const QString &ip)
{
    m_ip = ip;
}
QString RecombinationNode::position() const
{
    return m_position;
}

void RecombinationNode::setPosition(const QString &position)
{
    m_position = position;
}

void RecombinationNode::appendPosition(QPointF &point)
{
    if(!m_position.isEmpty()){
        m_position.append(" ");
    }
    m_position.append(QString::number(point.x())).append(",").append(QString::number(point.y()));
}

QString RecombinationNode::getReceivedListString()
{
    QString result;
    for(QListIterator<QString> iterator(m_receivedList);iterator.hasNext();){
//        QString next=iterator.next();
        result.append(iterator.next()).append(" ");
    }

    return result;
}

void RecombinationNode::clearReceivedListAndPosition()
{
    m_receivedList.clear();
    m_position.clear();
    fourPointsList.clear();
}
QString RecombinationNode::getFourPointsList() const
{
    return fourPointsList;
}

void RecombinationNode::appendFourPointsList(QVector<QPointF> fourPoints)
{

    for (int i = 0; i < fourPoints.size(); ++i) {
        QPointF pointTemp=fourPoints[i];
        if(!fourPointsList.isEmpty()){
            if(i==0)
                fourPointsList.append(";");
            else
                fourPointsList.append(",");
        }
        fourPointsList.append(QString::number(pointTemp.x()).toLatin1());
        fourPointsList.append(",");
        fourPointsList.append(QString::number(pointTemp.y()).toLatin1());

    }
}





