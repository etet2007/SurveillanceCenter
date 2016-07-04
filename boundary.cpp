#include "boundary.h"

Boundary::Boundary()
{

}

Boundary::Boundary(QWidget *parent):QLabel(parent)
{

    setText("first line\nsecond line");
}

void Boundary::paintEvent(QPaintEvent *event)
{
QLabel::paintEvent(event);
QPainter painter;
painter.drawLine(0,0,100,200);
}

