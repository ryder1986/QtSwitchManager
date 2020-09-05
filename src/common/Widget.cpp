#include "MainWidget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::gray);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}
