#ifndef TOPOLOGYFRAME
#define TOPOLOGYFRAME

#include <QLabel>
#include <QTabWidget>
#include <QMap>
#include <QString>
#include "Widget.h"
#include "NetworkGraph.h"
#include "portconfig.h"
#include <QHBoxLayout>

#define SPECIALMODE "专线模式"
#define PROTECTIONMODE "环形保护模式"


class TopologyFrame : public Widget
{
    Q_OBJECT
public:
    TopologyFrame(Widget *parent = 0);
    void SetWarn(QString strLineName, bool bWarn);

protected slots:
    void slot_PortConfigShow();  //打开端口配置界面
    void slot_PortShowWarn(int dwDevId = -1);  //关闭端口配置页面和收到告警的回调

private:
    QString GetLineNameByPort(MAPDEV2LINEPORT &mapDev2LinePort, QString strPort);
    void WarnLines(MAPDEV2LINEPORT &mapDevs2Port, QString strPortType, int dwStat, int dwDevId, QSet<QString> &setNotWarnLiens);

    QTabWidget m_Tabwidget;
    QHBoxLayout m_mainlayout;
    QMap<QString, NetworkGraph*> m_mapNetworkGraph;
};

#endif //TOPOLOGYFRAME
