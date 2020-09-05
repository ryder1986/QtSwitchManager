#include <QVBoxLayout>
#include <QTableWidget>
#include <QSettings>
#include <QPair>
#include <QHeaderView>
#include "PortFrame.h"

SingleRow::SingleRow(int dwShape, QStringList &strWordsList, QWidget *parent) : QTableWidget(parent)
{
    this->setRowCount(1);
    this->setColumnCount(strWordsList.size());
    this->setShowGrid(false);
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int dwIdx = 0; dwIdx < strWordsList.size(); dwIdx++)
    {
        BasicGraph *graph = new BasicGraph(dwShape, strWordsList[dwIdx]);
        m_vecSingleRow.push_back(graph);
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setFlags(Qt::NoItemFlags);
        item->setBackgroundColor(Qt::gray);
        this->setItem(0, dwIdx, item);
        this->setCellWidget(0, dwIdx, graph);
    }
}
  using namespace BG ;
T_LineOfBasicGraph gLineOfBasicGraph[] = {
    {SWITCH, "E1 1-8|E1 9-16|E1 17-24|E1 25-32"},
    {SWITCH, "E1 33-40|E1 41-48|E1 49-56|E1 57-64"},
    {SWITCH, "E1 65-72|E1 73-80|E1 81-88|E1 89-96"},
    {BGSHAPE::RECT, "GE1|GE2|GE3|GE4|GE5|GE6|GE7|GE8"},
    {BGSHAPE::RECT, "GE9|GE10|GE11|GE12|GE13|GE14|GE15|GE16"},
    {ECLIPSE, "主电源|备电源"}
};

QMap<QString, T_Controler> PortFrame::Oid2port;
QMutex PortFrame::mutex;
QMap<QString, BasicGraph*> PortFrame::BasicGraphs;

PortFrame::PortFrame(Snmp_Lsm *ptSnmp, Widget *parent) : Widget(parent)
{
    m_ptSnmp = ptSnmp;
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QTableWidget *TableWidget = new QTableWidget();
    int dwRows = sizeof(gLineOfBasicGraph) / sizeof(gLineOfBasicGraph[0]);
    TableWidget->setRowCount(dwRows);
    TableWidget->setColumnCount(1);
    TableWidget->verticalHeader()->setVisible(false);
    TableWidget->horizontalHeader()->setVisible(false);
    TableWidget->resizeColumnsToContents();
    TableWidget->resizeRowsToContents();
    TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int dwIdx = 0; dwIdx < dwRows; dwIdx++)
    {
        QStringList strListwords = gLineOfBasicGraph[dwIdx].strWords.split('|');
        SingleRow *singleRow = new SingleRow(gLineOfBasicGraph[dwIdx].dwShape, strListwords, TableWidget);
        for (int dwIdx = 0; dwIdx < singleRow->m_vecSingleRow.size(); dwIdx++)
        {
            PortFrame::BasicGraphs[singleRow->m_vecSingleRow[dwIdx]->GetName()] = singleRow->m_vecSingleRow[dwIdx];
        }

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setFlags(Qt::NoItemFlags);
        TableWidget->setItem(dwIdx, 0, item);
        TableWidget->setCellWidget(dwIdx, 0, singleRow);
    }
    mainLayout->addWidget(TableWidget);
    InitConfig();
    SnmpGet();
}

void PortFrame::FindControler(QString &strControler, int &dwIndex, QString &strName)
{
    for (int dwIdx = 0; dwIdx < sizeof(gLineOfBasicGraph) / sizeof(gLineOfBasicGraph[0]); dwIdx++)
    {
        QStringList strList = gLineOfBasicGraph[dwIdx].strWords.split("|");
        for (int dwJ = 0; dwJ < strList.size(); dwJ++)
        {
            if (strList[dwJ] == strControler)
            {
                dwIndex = -1;
                strName = strControler;
                return;
            }
            else if (strList[dwIdx].mid(0,2) == strControler.mid(0, 2))
            {
                int dwNum = strControler.mid(3).toInt();
                int dwMin = dwIdx * 32 + dwJ * 8 + 1;
                if (dwNum < (dwMin + 8) && dwNum >= dwMin)
                {
                    strName = strList[dwJ];
                    dwIndex = ((dwNum - 1) % 8);
                    return;
                }
            }
        }
    }
}

void PortFrame::InitConfig()
{
    QSettings setting(QString::fromStdString("./config.ini"), QSettings::IniFormat);
    setting.beginGroup("Oid");
    for (int dwIdx = 1; dwIdx <= 112; dwIdx++)
    {
        T_Controler tControler;
        if (dwIdx <= 96)
        {
            tControler.m_strPort = QString("E1_%1").arg(dwIdx);
        }
        else
        {
            tControler.m_strPort = QString("GE%1").arg(dwIdx - 96);
        }
        tControler.m_strOid = setting.value(tControler.m_strPort).toString();
        FindControler(tControler.m_strPort, tControler.m_dwIdx, tControler.m_strName);
        tControler.m_bWarn = true;
        PortFrame::Oid2port[tControler.m_strOid] = tControler;
    }
    m_strIp = setting.value("IP").toString();
    setting.endGroup();
}

void PortFrame::SnmpGet()
{
    QMap<QString, T_Controler>::iterator itr = PortFrame::Oid2port.begin();
    for (;itr != PortFrame::Oid2port.end(); itr++)
    {
        QString strRet;
        T_Controler &tControler = itr.value();
        if (0 == m_ptSnmp->SnmpGet(strRet, m_strIp.toLatin1().data(), tControler.m_strOid.toLatin1().data())
            && strRet == "1")
        {
            tControler.m_bWarn = false;
        }
        PortFrame::SetWarn(tControler);
    }
}


bool PortFrame::SetInfoByOid(QString strOid, QString strMsg)
{
    bool bWarn;
    if (strMsg== "1")
    {
        bWarn = false;
    }
    else if (strMsg== "2")
    {
        bWarn = true;
    }
    else
    {
        return false;
    }

    PortFrame::mutex.lock();
    if (PortFrame::Oid2port.find(strOid) != PortFrame::Oid2port.end())
    {
        T_Controler &tControler = PortFrame::Oid2port[strOid];
        tControler.m_bWarn = bWarn;
        PortFrame::mutex.unlock();
        PortFrame::SetWarn(tControler);
        return true;
    }
    PortFrame::mutex.unlock();
    return false;
}

bool PortFrame::SetInfoByPort(QString strPort, QString strMsg)
{
    bool bWarn;
    if (strMsg== UP)
    {
        bWarn = false;
    }
    else if (strMsg== DOWN)
    {
        bWarn = true;
    }
    else
    {
        return false;
    }

    PortFrame::mutex.lock();
    QMap<QString, T_Controler>::iterator itr = PortFrame::Oid2port.begin();
    for ( ; itr != PortFrame::Oid2port.end(); itr++)
    {
        T_Controler &tControler = itr.value();
        if (tControler.m_strPort == strPort)
        {
            tControler.m_bWarn = bWarn;
            PortFrame::SetWarn(tControler);
            break;
        }
    }
    PortFrame::mutex.unlock();
    return false;
}

void PortFrame::SetWarn(T_Controler &tControler)
{
    if (PortFrame::BasicGraphs.find(tControler.m_strName) != PortFrame::BasicGraphs.end())
    {
        PortFrame::BasicGraphs[tControler.m_strName]->SetWarn(tControler.m_bWarn, tControler.m_dwIdx);
    }
}

bool PortFrame::GetInfo(QString strPort)
{
    bool bWarn = true;
    PortFrame::mutex.lock();
    QMap<QString, T_Controler>::iterator itr = PortFrame::Oid2port.begin();
    for ( ; itr != PortFrame::Oid2port.end(); itr++)
    {
        T_Controler &tControler = itr.value();
        if (tControler.m_strPort == strPort)
        {
            bWarn = tControler.m_bWarn;
            break;
        }
    }
    PortFrame::mutex.unlock();
    return bWarn;
}
