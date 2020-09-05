#include <QPainter>
#include <QTransform>
#include <QFont>
#include <QRect>
#include "BasicGraph.h"

BasicGraph::BasicGraph(int dwShape, QString strWords, QWidget *parent) : QWidget(parent)
{
    m_dwShape  = dwShape;
    m_strWords = strWords;
    m_dwWarnIdx = 0;
    m_bWarn = true;
    update();
    for (int dwIdx = 0; dwIdx < 8; dwIdx++)
    {
        m_baWarn[dwIdx] = true;
    }
}

void BasicGraph::paintEvent(QPaintEvent *event)
{
    int dwSide = qMin(width(), height());
    QPainter cPainter(this);
    cPainter.setRenderHint(QPainter::Antialiasing, true);
    QTransform cTransform;
    cTransform.translate(width() / 2, height() / 2);
    cTransform.scale(dwSide / 300.0, dwSide / 300.0);
    cPainter.setTransform(cTransform);
    QPen pen;
    pen.setWidth(5);
    cPainter.setPen(pen);

    if (m_bWarn)
    {
        cPainter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    }
    else
    {
        cPainter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    }
    using namespace BG ;
    if (m_dwShape == CIRCLE)
    {
        cPainter.drawEllipse(-75, -120, 150, 150);
    }
    else if (m_dwShape == BG::RECT)
    {
        cPainter.drawRect(-70, -120, 140, 140);
    }
    else if (m_dwShape == ECLIPSE)
    {
        cPainter.drawRoundedRect(-90, -80, 180, 100, 40, 40);
    }
    else if (m_dwShape == RECTANGLE)
    {
        cPainter.drawRect(-100, -90, 200, 110);
    }
    else if (m_dwShape == SWITCH)
    {
        int dwStartx = -135;
        int dwStarty = -90;
        int dwHight = 100;
        int dwWidth = 35;
        m_baWarn[m_dwWarnIdx] = m_bWarn;
        for (int dwIdx = 0; dwIdx < 8; dwIdx++)
        {
            if (m_baWarn[dwIdx])
            {
                cPainter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
            }
            else
            {
                cPainter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
            }

            cPainter.drawRect(dwStartx, dwStarty, dwWidth, dwHight);
            dwStartx += dwWidth;
        }
    }
    QRect rect(-160, 55, 320, 100);
    QFont font(QString::fromLocal8Bit("宋体"), 50);
    cPainter.setFont(font);
    cPainter.drawText(rect, Qt::AlignHCenter, m_strWords);
}

void BasicGraph::SetWarn(bool bWarn, int dwIdx)
{
    m_bWarn = bWarn;
    m_dwWarnIdx = dwIdx;
    paintEvent(NULL);
}

QString& BasicGraph::GetName()
{
    return m_strWords;
}
