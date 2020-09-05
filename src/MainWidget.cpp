#include "MainWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QPixmap>
#include <QLabel>

Snmp_Lsm *gptSnmp_Lsm = NULL;

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    m_ptDevInfoFrame = new DevInfoFrame(&m_tSnmp);
    m_ptPortFrame = new PortFrame(&m_tSnmp);
    gptSnmp_Lsm = &m_tSnmp;
    m_ptTopologyFrame = new TopologyFrame();
    connect(gptSnmp_Lsm, SIGNAL(signal_SendPortWarn()), m_ptTopologyFrame, SLOT(slot_PortShowWarn()));
    connect(gptSnmp_Lsm, SIGNAL(signal_SendWarnText(QString, QString)), &m_WarnFrame, SLOT(slot_ReciveWarnText(QString, QString)));
    connect(&m_WarnFrame, SIGNAL(signal_SendPortWarn()), m_ptTopologyFrame, SLOT(slot_PortShowWarn()));
    m_mainlayout = new QHBoxLayout(this);
    m_rightlayout = new QVBoxLayout();
    m_leftlayout = new QVBoxLayout();
    m_mainlayout->addLayout(m_leftlayout);
    m_mainlayout->addLayout(m_rightlayout);
    slot_Topology();
    DrawRight();

    setFixedSize(1000, 650);
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap("./pic/background.jpg").scaled( // 缩放背景图.
                                                                    this->size(),
                                                                    Qt::IgnoreAspectRatio))); // 使用平滑的缩放方式
    this->setPalette(palette); // 至此, 已给widget加上了背景图.
}
#define is_win 1

void MainWidget::DrawRight()
{
    QFont ft;
    ft.setPointSize(10);
#if is_win
    m_BtnTopology.setText(QString("图形连接拓扑"));

#else
    m_BtnTopology.setText(QString::fromLocal8Bit("图形连接拓扑"));
#endif

    m_BtnTopology.setFont(ft);
    connect(&m_BtnTopology, SIGNAL(clicked()), this, SLOT(slot_Topology()));
    m_BtnTopology.setMinimumSize(150, 40);
    m_rightlayout->addWidget(&m_BtnTopology);
#if is_win
    m_BtnPort.setText(QString("系统端口框架"));
#else
    m_BtnPort.setText(QString::fromLocal8Bit("系统端口框架"));
#endif
    m_BtnPort.setFont(ft);
    connect(&m_BtnPort, SIGNAL(clicked()), this, SLOT(slot_Port()));
    m_BtnPort.setMinimumSize(150, 40);
    m_rightlayout->addWidget(&m_BtnPort);
#if is_win
    m_BtnDevInfo.setText(QString("设备信息"));
#else
    m_BtnDevInfo.setText(QString::fromLocal8Bit("设备信息"));
#endif
    m_BtnDevInfo.setFont(ft);
    m_BtnDevInfo.setMaximumWidth(100);
    connect(&m_BtnDevInfo, SIGNAL(clicked()), this, SLOT(slot_DevInfo()));
    m_BtnDevInfo.setMinimumSize(150, 40);
    m_rightlayout->addWidget(&m_BtnDevInfo);
}

void MainWidget::slot_DevInfo()
{
    m_leftlayout->removeItem(m_leftlayout->takeAt(0));
    m_leftlayout->removeItem(m_leftlayout->takeAt(0));
    m_ptPortFrame->hide();
    m_ptTopologyFrame->hide();
    m_ptDevInfoFrame->show();
    m_leftlayout->addWidget(m_ptDevInfoFrame);
    m_leftlayout->addWidget(&m_WarnFrame);
    m_leftlayout->setStretchFactor(m_ptDevInfoFrame, 3);
    m_leftlayout->setStretchFactor(&m_WarnFrame, 2);
}

void MainWidget::slot_Topology()
{
    m_leftlayout->removeItem(m_leftlayout->takeAt(0));
    m_leftlayout->removeItem(m_leftlayout->takeAt(0));
    m_ptPortFrame->hide();
    m_ptDevInfoFrame->hide();
    m_ptTopologyFrame->show();
    m_leftlayout->addWidget(m_ptTopologyFrame);
    m_leftlayout->addWidget(&m_WarnFrame);
    m_leftlayout->setStretchFactor(m_ptTopologyFrame, 3);
    m_leftlayout->setStretchFactor(&m_WarnFrame, 2);
}


void MainWidget::slot_Port()
{
    m_leftlayout->removeItem(m_leftlayout->takeAt(0));
    m_leftlayout->removeItem(m_leftlayout->takeAt(0));
    m_ptTopologyFrame->hide();
    m_ptDevInfoFrame->hide();
    m_ptPortFrame->show();
    m_leftlayout->addWidget(m_ptPortFrame);
    m_leftlayout->addWidget(&m_WarnFrame);
    m_leftlayout->setStretchFactor(m_ptPortFrame, 3);
    m_leftlayout->setStretchFactor(&m_WarnFrame, 2);
}

