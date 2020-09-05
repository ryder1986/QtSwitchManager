#include <QSettings>
#include <QSet>
#include "PortFrame.h"
#include "TopologyFrame.h"

DevPixmap gatDevPixmapForSpecial[] = {
    {0, "./pic/dev.png", "./pic/devgray.png", NULL, 0.25, 0.4, "设备2", 0.175, 0.42, NULL, "1:上端口|5:右端口", "3:下端口", NULL, UNMONTOR},
    {1, "./pic/station.png", "./pic/station.png", NULL, 0.24, 0.7, "站点1", 0.175, 0.8, NULL,  "", "3:上端口", NULL, NOTUSE},
    {2, "./pic/station.png", "./pic/station.png", NULL, 0.68, 0.7, "站点2", 0.785, 0.8, NULL,"", "4:上端口", NULL, NOTUSE},
    {3, "./pic/dev.png", "./pic/devgray.png", NULL, 0.68, 0.4, "设备3", 0.785, 0.42, NULL, "2:上端口|5:左端口", "4:下端口", NULL, UNMONTOR},
    {4, "./pic/dev.png", "./pic/devgray.png", NULL, 0.453, 0.09, "设备1", 0.468, 0.2, NULL, "1:左端口|2:右端口", "", NULL, UNMONTOR},
    {-1, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, "", "", NULL, NOTUSE}
};

Line gatLineForSpecial[] = {
    {"1", {0.295, 0.395, 0.295, 0.15}, new QColor(Qt::yellow), false},   //左上
    {"1", {0.295, 0.15, 0.451, 0.15}, new QColor(Qt::yellow), false},
    {"2", {0.548, 0.15, 0.725, 0.15}, new QColor(Qt::yellow), false},     //右上
    {"2", {0.725, 0.15, 0.725, 0.395}, new QColor(Qt::yellow), false},
    {"3", {0.295, 0.50, 0.295, 0.71}, new QColor(Qt::blue), true}, //左下
    {"4", {0.725, 0.50, 0.725, 0.72}, new QColor(Qt::blue), true},    //右下
    {"5", {0.346, 0.45, 0.679, 0.45}, new QColor(Qt::yellow), false},//中
    {NULL, 0, 0, 0}
};

DevPixmap gatDevPixmapForRing[] = {
    {5, "./pic/dev.png", "./pic/devgray.png", NULL, 0.25, 0.4, "设备3", 0.175, 0.42, NULL, "7:上端口|12:右端口", "10:下端口", NULL, UNMONTOR},
    {6, "./pic/station.png", "./pic/station.png", NULL, 0.24, 0.7, "站点1", 0.175, 0.8, NULL, "", "10:上端口",NULL, NOTUSE},
    {7, "./pic/station.png", "./pic/station.png", NULL, 0.68, 0.7, "站点2", 0.785, 0.8, NULL, "", "11:上端口",NULL, NOTUSE},
    {8, "./pic/dev.png", "./pic/devgray.png", NULL, 0.68, 0.4, "设备4", 0.785, 0.42, NULL, "9:上端口|12:左端口", "11:下端口", NULL, UNMONTOR},
    {9, "./pic/dev.png", "./pic/devgray.png", NULL, 0.25, 0.09, "设备1", 0.175, 0.126, NULL, "7:下端口|8:右端口", "", NULL, UNMONTOR},
    {10, "./pic/dev.png", "./pic/devgray.png", NULL, 0.68, 0.09, "设备2", 0.785, 0.126, NULL,  "9:下端口|8:左端口", "",NULL, UNMONTOR},
    {-1, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, "", "", NULL, NOTUSE}
};

Line gatLineForRing[] = {
    {"7", {0.295, 0.395, 0.295, 0.192}, new QColor(Qt::yellow), false}, //左上
    {"8", {0.346, 0.15, 0.679, 0.15}, new QColor(Qt::yellow), false}, //中上
    {"9", {0.725, 0.192, 0.725, 0.395}, new QColor(Qt::yellow), false}, //右上
    {"10", {0.295, 0.50, 0.295, 0.71}, new QColor(Qt::blue), true}, //左下
    {"11", {0.725, 0.50, 0.725, 0.72}, new QColor(Qt::blue), true},//右下
    {"12", {0.346, 0.45, 0.679, 0.45}, new QColor(Qt::yellow), false},//中下
    {NULL, 0, 0, 0}
};

TopologyFrame::TopologyFrame(Widget *parent) : Widget(parent)
{
    m_mapNetworkGraph[SPECIALMODE] = new NetworkGraph(gatDevPixmapForSpecial, gatLineForSpecial);
    m_mapNetworkGraph[PROTECTIONMODE] = new NetworkGraph(gatDevPixmapForRing, gatLineForRing);
    QMap<QString, NetworkGraph*>::iterator itr = m_mapNetworkGraph.begin();
    for (; itr != m_mapNetworkGraph.end(); itr++)
    {
        DevPixmap* ptPixmap = itr.value()->GetDevPixmap();
        for (int dwIdx = 0; ptPixmap[dwIdx].pcPixName != NULL; dwIdx++)
        {
            ptPixmap[dwIdx].ptPixmap = new QPushButton(itr.value());
            ptPixmap[dwIdx].ptPixmap->setStyleSheet(QString("border-image:url(%1);border:none").arg(ptPixmap[dwIdx].pcPixName));
            if (!QString(ptPixmap[dwIdx].pcName).contains("站点"))
            {
                connect(ptPixmap[dwIdx].ptPixmap, SIGNAL(clicked()), this, SLOT(slot_PortConfigShow()));
            }
            ptPixmap[dwIdx].ptLable = new QLabel(ptPixmap[dwIdx].pcName);
            ptPixmap[dwIdx].ptLable->setParent(itr.value());
            ptPixmap[dwIdx].ptLable->setStyleSheet("background-color: rgba(0,0,0,0);border: 1px groove gray;border-top-left-radius:6px;border-top-right-radius:6px;border-bottom-left-radius:6px;border-bottom-right-radius:6px;padding-left:0px;padding-right:0px;");

            ptPixmap[dwIdx].ptPortConfig = new PortConfig(ptPixmap[dwIdx].dwIdx, ptPixmap[dwIdx].pcName, ptPixmap[dwIdx].strGE, ptPixmap[dwIdx].strE1, &(itr.value()->m_mapDevs2GE), &(itr.value()->m_mapDevs2E1), ptPixmap[dwIdx].ptLable, NULL);
            ptPixmap[dwIdx].ptPortConfig->hide();
            connect(ptPixmap[dwIdx].ptPortConfig, SIGNAL(signal_config(int)), this, SLOT(slot_PortShowWarn(int)));
        }
        m_Tabwidget.addTab(itr.value(), itr.key());
    }
    m_mainlayout.addWidget(&m_Tabwidget);
    setLayout(&m_mainlayout);
    slot_PortShowWarn();
}

void TopologyFrame::slot_PortConfigShow()
{
    QPushButton *Btn  = (QPushButton*)sender();
    PortConfig *ptPortConfigToShow = NULL;

    QMap<QString, NetworkGraph*>::iterator itr = m_mapNetworkGraph.begin();
    for (; itr != m_mapNetworkGraph.end(); itr++)
    {
        DevPixmap *ptDevPixmap = itr.value()->GetDevPixmap();
        for (int i = 0; ptDevPixmap[i].pcPixName != NULL; i++)
        {
            if (!ptDevPixmap[i].ptPortConfig->isHidden())
            {
                return;
            }

            if ((unsigned long long int)(ptDevPixmap[i].ptPixmap) == (unsigned long long int)Btn)
            {
                ptPortConfigToShow = ptDevPixmap[i].ptPortConfig;
            }
        }
    }

    if (ptPortConfigToShow)
    {
        ptPortConfigToShow->MyShow();
    }
}

void TopologyFrame::slot_PortShowWarn(int dwDevId)
{
    int dwIdx;
    QSet<QString> setNotWarnLiens;
    QMap<QString, NetworkGraph*>::iterator itr = m_mapNetworkGraph.begin();
    for ( ;itr != m_mapNetworkGraph.end(); itr++)
    {
        DevPixmap *ptDevPixMap = itr.value()->GetDevPixmap();
        MAPDEV2LINEPORT &mapDev2E1 = itr.value()->m_mapDevs2E1;
        MAPDEV2LINEPORT &mapDev2GE= itr.value()->m_mapDevs2GE;
        for (dwIdx = 0; ptDevPixMap[dwIdx].pcPixName; dwIdx++)
        {
            if (dwDevId == ptDevPixMap[dwIdx].dwIdx && ptDevPixMap[dwIdx].dwStat != NOTUSE)
            {
                ptDevPixMap[dwIdx].dwStat = (ptDevPixMap[dwIdx].ptPortConfig->IsSelected() ? MONITOR : UNMONTOR);
            }
            if (ptDevPixMap[dwIdx].dwStat == UNMONTOR || (dwDevId != ptDevPixMap[dwIdx].dwIdx && dwDevId != -1))
            {
                ptDevPixMap[dwIdx].ptPortConfig->Unselect();
                ptDevPixMap[dwIdx].dwStat = UNMONTOR;
            }
            WarnLines(mapDev2E1, "E1", ptDevPixMap[dwIdx].dwStat, ptDevPixMap[dwIdx].dwIdx, setNotWarnLiens);
            WarnLines(mapDev2GE, "GE", ptDevPixMap[dwIdx].dwStat, ptDevPixMap[dwIdx].dwIdx, setNotWarnLiens);

        }
    }

    for (itr = m_mapNetworkGraph.begin() ;itr != m_mapNetworkGraph.end(); itr++)
    {
        Line *ptLines = itr.value()->GetLines();
        for (int dwIdx = 0; ptLines[dwIdx].pcName; dwIdx++)
        {
            if (setNotWarnLiens.find(ptLines[dwIdx].pcName) != setNotWarnLiens.end())
            {
                SetWarn(ptLines[dwIdx].pcName, false);
            }
            else
            {
                SetWarn(ptLines[dwIdx].pcName, true);
            }
        }
    }
}

void TopologyFrame::WarnLines(MAPDEV2LINEPORT &mapDevs2Port, QString strPortType, int dwStat, int dwDevId, QSet<QString> &setNotWarnLiens)
{
    MAPDEV2LINEPORT::iterator itr = mapDevs2Port.find(dwDevId);
    if (itr == mapDevs2Port.end())
    {
        return;
    }
    QVector<T_LinePort> &vectLinePort = itr.value();
    QString strPort;
    QString strLineName;
    for (int dwIdx = 0; dwIdx < vectLinePort.size(); dwIdx++)
    {
        T_LinePort &tLinePort = vectLinePort[dwIdx];
        strLineName = tLinePort.strLineName;
        if (strPortType == "GE")
        {
            strPort = QString("GE%1").arg(tLinePort.dwGEPort);
            if (dwStat == MONITOR && !PortFrame::GetInfo(strPort))
            {
                setNotWarnLiens.insert(strLineName);
            }
        }
        else if (strPortType == "E1")
        {
            QVector<int> &vecE1Ports = tLinePort.vecE1Ports;
            if (vecE1Ports.size() == 0)
            {
                continue;
            }
            for (int i = 0; i < vecE1Ports.size(); i++)
            {
                strPort = QString("E1_%1").arg(vecE1Ports[i]);
                if (dwStat == MONITOR && !PortFrame::GetInfo(strPort))
                {
                    setNotWarnLiens.insert(strLineName);
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }
}

void TopologyFrame::SetWarn(QString strLineName, bool bWarn)
{
    QMap<QString, NetworkGraph*>::iterator itr = m_mapNetworkGraph.begin();
    for ( ;itr != m_mapNetworkGraph.end(); itr++)
    {
        Line *ptLine = itr.value()->GetLines();
        for (int dwIdx = 0; ptLine[dwIdx].pcName; dwIdx++)
        {
            if (strLineName == ptLine[dwIdx].pcName)
            {
                ptLine[dwIdx].bWarn = bWarn;
            }
        }
        itr.value()->SetDevAndLines(itr.value()->GetDevPixmap(), ptLine);
    }
}
