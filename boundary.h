#ifndef BOUNDARY_H
#define BOUNDARY_H
#include <QLabel>
#include <QPainter>
class Boundary : public QLabel
{
public:
    Boundary();
    Boundary(QWidget *parent=0);
    void paintEvent(QPaintEvent *);
};

#endif // BOUNDARY_H
