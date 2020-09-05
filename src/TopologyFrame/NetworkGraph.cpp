#include <QPixmap>
#include <QVBoxLayout>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QEvent>
#include "NetworkGraph.h"
#include "basicinfo.h"


NetworkGraph::NetworkGraph(DevPixmap *ptDevs, Line *ptLines, Widget *parent) : Widget(parent)
{
    m_ptDevs = ptDevs;
    m_ptLines = ptLines;
}

void NetworkGraph::SetDevAndLines(DevPixmap *ptDevs, Line *ptLines)
{
    m_ptDevs = ptDevs;
    m_ptLines = ptLines;
    update();
}

void NetworkGraph::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int dwIdx = 0; m_ptDevs[dwIdx].pcPixName != NULL; dwIdx++)
    {
        const char *pcPixName = NULL;
        QString str = m_ptDevs[dwIdx].pcName;
        if (m_ptDevs[dwIdx].dwStat == UNMONTOR)
        {
            pcPixName = m_ptDevs[dwIdx].pcPixNameGray;
        }
        else
        {
            pcPixName = m_ptDevs[dwIdx].pcPixName;
        }
        m_ptDevs[dwIdx].ptPixmap->setStyleSheet(QString("border-image:url(%1);border:none").arg(pcPixName));
        QPixmap p(pcPixName);
        m_ptDevs[dwIdx].ptPixmap->setGeometry(width() * m_ptDevs[dwIdx].fXShift, height() * m_ptDevs[dwIdx].fYShift, p.width() / SCALEDSIZE, p.height() / SCALEDSIZE);
        m_ptDevs[dwIdx].ptLable->setGeometry(width() * m_ptDevs[dwIdx].fLableXShift, height() * m_ptDevs[dwIdx].fLableYShift, 50, 20);
    }

    for (int dwIdx = 0; m_ptLines[dwIdx].pcName != NULL; dwIdx++)
    {
        Line line = m_ptLines[dwIdx];
        QPen pen;
        if (!line.bWarn)
        {
            pen.setColor(*line.ptDefautColor);
        }
        else
        {
            pen.setColor(Qt::red);
        }
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawLine(line.adwLine[0] * width(), line.adwLine[1] * height(), \
                         line.adwLine[2] * width(), line.adwLine[3] * height());
    }
}

DevPixmap *NetworkGraph::GetDevPixmap()
{
    return m_ptDevs;
}

Line *NetworkGraph::GetLines()
{
    return m_ptLines;
}
