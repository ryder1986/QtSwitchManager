#include <QVBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QSettings>
#include "PortFrame.h"
#include "WarnFrame.h"
#include "Widget.h"

WarnFrame::WarnFrame(Widget *parent) : Widget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_dwSeq = 0;
    QStringList strlistHeaders;
    strlistHeaders << "序号" << "告警级别" << "告警代码" << "告警名称" << "告警描述" << "发生时间";
    m_TableWiget.setRowCount(m_dwSeq);
    m_TableWiget.setColumnCount(strlistHeaders.size());
    m_TableWiget.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_TableWiget.horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_TableWiget.horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_TableWiget.horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_TableWiget.horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_TableWiget.verticalHeader()->setVisible(false);
    m_TableWiget.setHorizontalHeaderLabels(strlistHeaders);
    layout->addWidget(&m_TableWiget);

    QSettings setting(QString::fromStdString("./config.ini"), QSettings::IniFormat);
    setting.beginGroup("WarnText");
    setting.setIniCodec("UTF-8");
    int dwWarnTextCnt = setting.value("Cnt").toInt();
    for (int dwIdx = 0; dwIdx < dwWarnTextCnt; dwIdx++)
    {
        QString str = QString("Warn_%1").arg(dwIdx);
        QString strVal = setting.value(str).toString();
        if (strVal == "")
        {
            QStringList strList = setting.value(str).toStringList();
            strVal = strList.join(",");
        }

        T_WarnConfig tWarnConfig;
        tWarnConfig.SetVal(strVal);
        m_mapCode2WarnConfig[tWarnConfig.m_strWarnCode] = tWarnConfig;
        //slot_ReciveWarnText(tWarnText.m_strWarnOid);
    }
    setting.endGroup();

    m_RegExp.setPattern(QString("^1.3.6.1.4.1.3902.3.302.1.1.(\d+)$"));
}

void WarnFrame::slot_ReciveWarnText(QString strOid, QString strMsg)
{
    static T_WarnText tWarnText;
    static QString strStaus;

    if (!CanStartParase(strOid, strMsg, tWarnText.m_strCode, strStaus, tWarnText.m_strTime, tWarnText.m_strDesc))
    {
        return;
    }

    //根据告警码得到配置信息
    QMap<QString, T_WarnConfig>::iterator itrCode2WarnConfig = m_mapCode2WarnConfig.find(tWarnText.m_strCode);
    if (itrCode2WarnConfig == m_mapCode2WarnConfig.end())
    {
        return;
    }
    T_WarnConfig &tWarnConfig = itrCode2WarnConfig.value();

    //得到告警级别、告警名称、告警时间
    tWarnText.m_strLevel = tWarnConfig.m_strWarnLevel;
    tWarnText.m_strName  = tWarnConfig.m_strWarnName;
    QDateTime tTime = QDateTime::fromTime_t(tWarnText.m_strTime.toInt());
    tWarnText.m_strTime = tTime.toString("yyyy-MM-dd hh:mm:ss");

    //从描述中得到具体端口
    if (tWarnConfig.m_bGetPort && (MatchPortFromMsg(tWarnText.m_strDesc, E1, tWarnText.m_strPort) || MatchPortFromMsg(strMsg, GE, tWarnText.m_strPort)))
    {
        tWarnText.m_strDesc = tWarnConfig.m_strWarnDesc + "(" + tWarnText.m_strPort + ")";
    }
    else
    {
        tWarnText.m_strDesc = tWarnConfig.m_strWarnDesc;
    }

    //得到端口状态，up or down
    strStaus = (strStaus == "0" ? UP : DOWN);

    //根据端口进行端口告警
    if (tWarnText.m_strPort != "")
    {
        PortFrame::SetInfoByPort(tWarnText.m_strPort, strStaus);
        emit signal_SendPortWarn();
    }

    //更新文本区
    UpdateWarnText(tWarnText, strStaus);

}

void WarnFrame::UpdateWarnText(T_WarnText &tWarnText, QString strStatus)
{
    bool bHandle = false;
    int dwSize = m_vecWarnText.size();
    for (int dwIdx = 0; (dwIdx < dwSize) && (!bHandle); dwIdx++)
    {
        if ((tWarnText.m_strCode == m_vecWarnText[dwIdx].m_strCode) &&
            (tWarnText.m_strPort == m_vecWarnText[dwIdx].m_strPort))
        {
            if (strStatus == DOWN)
            {
                m_vecWarnText[dwIdx].m_strTime = tWarnText.m_strTime;
            }
            else
            {
                m_vecWarnText.removeAt(dwIdx);
                --dwSize;
            }
            bHandle = true;
        }
    }

    if (!bHandle && (strStatus == DOWN))
    {
        m_vecWarnText.push_back(tWarnText);
        ++dwSize;
    }

    //根据告警级别获取背景色
    Qt::GlobalColor dwColor;
    if (tWarnText.m_strLevel == "紧急告警") {
        dwColor = Qt::darkYellow;
    }
    else if (tWarnText.m_strLevel == "主要告警") {
        dwColor = Qt::red;
    }
    else if (tWarnText.m_strLevel == "次要告警") {
        dwColor = Qt::darkYellow;
    }
    else if (tWarnText.m_strLevel == "提示告警") {
        dwColor = Qt::yellow;
    }
    else {
        return;
    }

    //显示文本告警
    m_TableWiget.setRowCount(dwSize);
    for (int dwRow = 0; dwRow < dwSize; dwRow++)
    {
        QStringList strlistVals;
        strlistVals << QString::number(dwRow + 1) << tWarnText.m_strLevel << "276-120-00-" + tWarnText.m_strCode << tWarnText.m_strName
                    << tWarnText.m_strDesc << tWarnText.m_strTime;
        for (int dwIdx = 0; dwIdx < strlistVals.size(); dwIdx++)
        {
            QTableWidgetItem *ptItem = new QTableWidgetItem(strlistVals[dwIdx]);
            ptItem->setTextAlignment(Qt::AlignCenter);
            ptItem->setFlags(Qt::NoItemFlags);
            ptItem->setBackground(QBrush(dwColor));
            if (dwIdx == 3 || dwIdx == 4)
            {
                ptItem->setToolTip(strlistVals[dwIdx]);
            }
            m_TableWiget.setItem(dwRow, dwIdx, ptItem);
        }
    }
}

//根据消息msg匹配端口
bool WarnFrame::MatchPortFromMsg(QString strMsg, QString strType, QString &strPort)
{
    QRegExp rx;
    int dwMajorIdx;
    int dwMinIdx;
    if (strType == E1)
    {
        rx.setPattern("ce1-([1-6])/([1-9]|1[0-6])");
        int dwPos = strMsg.indexOf(rx);
        if(dwPos < 0)
        {
            return false;
        }
        dwMajorIdx = rx.cap(1).toInt();
        dwMinIdx = rx.cap(2).toInt();
        if (dwMinIdx < 1 || dwMinIdx > 16)
        {
            return false;
        }
        strPort = "E1_" + QString::number((dwMajorIdx - 1) * 16 + dwMinIdx);
        return true;
    }
    else if (strType == GE)
    {
        rx.setPattern("gei-([7-8])/([1-8])");
        int dwPos = strMsg.indexOf(rx);
        if(dwPos < 0)
        {
            return false;
        }
        dwMajorIdx = rx.cap(1).toInt();
        dwMinIdx = rx.cap(2).toInt();
        if (dwMinIdx < 1 || dwMinIdx > 16)
        {
            return false;
        }
        strPort = "GE" + QString::number((dwMajorIdx - 7) * 8 + dwMinIdx);
        return true;
    }
    return false;
}

bool WarnFrame::CanStartParase(QString strOid, QString strMsg, QString &strCode, QString &strStatus, QString &strTime, QString &strDesc)
{
    if  (strOid.indexOf(m_RegExp) > 0)
    {
        m_mapOid2Val[strOid] = strMsg;
        return false;
    }
    else
    {
        strCode = m_mapOid2Val[CODEOID];
        strStatus = m_mapOid2Val[STATUSOID];
        strTime = m_mapOid2Val[TIMEOID];
        strDesc = m_mapOid2Val[DESCOID];
        m_mapOid2Val.clear();
        return true;
    }
}

