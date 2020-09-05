#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "TopologyFrame.h"
#include "PortFrame.h"
#include "DevInfoFrame.h"
#include "WarnFrame.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);


protected slots:
    void slot_Topology();
    void slot_Port();
    void slot_DevInfo();

private:
    void DrawRight();

    Snmp_Lsm m_tSnmp;

    TopologyFrame *m_ptTopologyFrame;
    PortFrame *m_ptPortFrame;
    DevInfoFrame *m_ptDevInfoFrame;
    WarnFrame m_WarnFrame;
    QPushButton m_BtnPort;
    QPushButton m_BtnIp;
    QPushButton m_BtnTopology;
    QPushButton m_BtnDevInfo;
    QHBoxLayout *m_mainlayout;
    QVBoxLayout *m_rightlayout;
    QVBoxLayout *m_leftlayout;
};

#endif // WIDGET_H
