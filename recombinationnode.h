#ifndef RECOMBINATIONNODE_H
#define RECOMBINATIONNODE_H

#include<QString>
#include<QList>
#include<QPointF>
class RecombinationNode
{
public:
    RecombinationNode();
    ~RecombinationNode();

    QList<QString> receivedList()const;
    void setReceivedList(const QList<QString> &receivedList);

    int id() const;
    void setId(int id);

    void appendReceivedList(QString &);
    QString ip() const;
    void setIp(const QString &ip);

    QString position() const;
    void setPosition(const QString &position);
    void appendPosition(QPointF &point);

    QString getReceivedListString();

    void clearReceivedListAndPosition();
    QString getFourPointsList() const;

private:
    int m_id;  //id编号
    QString m_ip;//重组节点ip地址

    QList<QString> m_receivedList; //存储切分节点ip  顺序不是固定的

    QString m_position;//不用
    QString fourPointsList;

};

#endif // RECOMBINATIONNODE_H
