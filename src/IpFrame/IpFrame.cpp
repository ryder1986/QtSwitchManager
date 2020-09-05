#include <QVBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>
#include <QHeaderView>
#include <QtNetwork/QNetworkInterface>
#include "IpFrame.h"
#include "Widget.h"

const char* acpStr[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "<—"};

IpFrame::IpFrame(Widget *parent) : Widget(parent)
{
    QVBoxLayout *mainlyout = new QVBoxLayout(this);
    QHBoxLayout *layout = new QHBoxLayout();
    m_LabelIp.setText("本机IP");
    layout->addStretch();
    layout->addWidget(&m_LabelIp);
    layout->addWidget(&m_LineEditIp);
    m_BtnConfig.setText("配置");
    connect(&m_BtnConfig, SIGNAL(clicked()), this, SLOT(slot_BtnConfig()));
    layout->addWidget(&m_BtnConfig);
    layout->addStretch();
    mainlyout->addStretch();
    mainlyout->addLayout(layout);
    QTableWidget* tablewidget = GenCalc();
    mainlyout->addWidget(tablewidget);
    mainlyout->setAlignment(tablewidget, Qt::AlignHCenter);
    mainlyout->addStretch();
}

QTableWidget* IpFrame::GenCalc()
{
    m_dwCalcCol = 3;
    m_dwCalcRow = 4;
    m_Calc.setRowCount(m_dwCalcRow);
    m_Calc.setColumnCount(m_dwCalcCol);
    m_Calc.verticalHeader()->setVisible(false);
    m_Calc.horizontalHeader()->setVisible(false);
    m_Calc.resizeColumnsToContents();
    m_Calc.resizeRowsToContents();
    m_Calc.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_Calc.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int dwRowIdx = 0; dwRowIdx < m_dwCalcRow; dwRowIdx++)
    {
        for (int dwColIdx = 0; dwColIdx < m_dwCalcCol; dwColIdx++)
        {
            QPushButton *Btn = new QPushButton(acpStr[dwRowIdx * m_dwCalcCol + dwColIdx]);
            connect(Btn, SIGNAL(clicked()), this, SLOT(slot_CalcClick()));
            m_Calc.setCellWidget(dwRowIdx, dwColIdx, Btn);
        }
    }
    return &m_Calc;
}

void IpFrame::slot_CalcClick()
{
    QPushButton *Btn = qobject_cast<QPushButton*>(sender());
    QString strBtn   = Btn->text();
    QString strIp    = m_LineEditIp.text();

    if (strBtn == "<—")
    {
        if (strIp.isEmpty())
        {
            return;
        }
        else
        {
            m_LineEditIp.setText(strIp.mid(0, strIp.length() - 1));
        }
    }
    else
    {
        m_LineEditIp.setText(strIp + strBtn);
    }
}

void IpFrame::slot_BtnConfig()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list)
    {
        QString strInterface = interface.name();
        if (strInterface != "lo")
        {
            QProcess * po = new QProcess();
            po->start(tr("ifconfig %1 %2").arg(strInterface).arg(m_LineEditIp.text()));
            return;
        }
    }
}


