#ifndef RECOMBINATIONNODE_H
#define RECOMBINATIONNODE_H

#include<QString>
#include<QList>
#include<QVector>
#include<QPointF>

//重组节点类
class RecombinationNode
{
public:
    RecombinationNode();
    ~RecombinationNode();

    //toploMat
    QList<QString> receivedList()const;
    void setReceivedList(const QList<QString> &receivedList);
    void appendReceivedList(QString &);
    QString getReceivedListString();

    int id() const;
    void setId(int id);
    //当前IP
    QString ip() const;
    void setIp(const QString &ip);
    //位置信息字符串
    QString position() const;
    void setPosition(const QString &position);
    void appendPosition(QPointF &point);

    void clearReceivedListAndPosition();

    QString getFourPointsList() const;
    void appendFourPointsList(QVector<QPointF> fourPoints);
private:
    //ID
    int m_id;
    //重组节点ip地址
    QString m_ip;
    //重组节点接收列表，每个元素为切分节点IP
    QList<QString> m_receivedList;
    //位置信息
    QString m_position;
    //测试使用的四点坐标
    QString fourPointsList;
};

#endif // RECOMBINATIONNODE_H
