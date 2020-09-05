#include <QGridLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <PortFrame.h>
#include "portconfig.h"


PortConfig::PortConfig(int dwIdx, QString strName, QString strGE, QString strE1, MAPDEV2LINEPORT *pMapDevs2GE, MAPDEV2LINEPORT *pMapDevs2E1, QLabel *ptLable, Widget *parent) : Widget(parent)
{
    m_dwIdx = dwIdx;
    m_pMapDevs2GE = pMapDevs2GE;
    m_pMapDevs2E1 = pMapDevs2E1;
    m_ptLable = ptLable;
    m_bMonited = false;

    QGridLayout *mainlayout = new QGridLayout(this);

   m_lbSelect.setText("监控本设备：");


     mainlayout->addWidget(&m_lbSelect, 0, 0, 1, 1);
    mainlayout->addWidget(&m_cbSelect, 0, 1, 1, 1);

    m_lbName.setText("本设备名称：");
    mainlayout->addWidget(&m_lbName, 1, 0, 1, 1);
    m_leName.setText(strName);
    mainlayout->addWidget(&m_leName, 1, 1, 1, 8);

    int dwE1RowStart = 3;
    if (strGE != "")
    {
        QVector<T_LinePort> vec;
        (*m_pMapDevs2GE)[m_dwIdx] = vec;
        QStringList strList = strGE.split("|");
        dwE1RowStart += strList.size();
        for (int dwIdx = 0; dwIdx < strList.size(); dwIdx++)
        {
            T_LinePort tLinePort;
            tLinePort.strLineName = strList[dwIdx].split(":")[0];
            QLabel *ptLabel = new QLabel("配置" + strList[dwIdx].split(":")[1] + ":");
            mainlayout->addWidget(ptLabel, 2 + dwIdx, 0, 1, 1);
            QComboBox *ptCombox = new QComboBox();
            for (int i = 1; i <= 16; i++)
            {
                if (IsGESelectedByOther(i))
                {
                    continue;
                }
                ptCombox->addItem(QString("GE%1").arg(i));
            }
            m_vecComboxGeName.push_back(ptCombox);
            tLinePort.dwGEPort = CurGEPortIdx(dwIdx);
            (*m_pMapDevs2GE)[m_dwIdx].push_back(tLinePort);
            mainlayout->addWidget(ptCombox, 2 + dwIdx, 1, 1, 8);
        }
    }

    if (strE1 != "")
    {
        QVector<T_LinePort> vec;
        (*m_pMapDevs2E1)[m_dwIdx] = vec;
        T_LinePort tLinePort;
        tLinePort.strLineName = strE1.split(":")[0];
        (*m_pMapDevs2E1)[m_dwIdx].push_back(tLinePort);

        m_LbE1Name.setText("配置" + strE1.split(":")[1] + ":");
        mainlayout->addWidget(&m_LbE1Name, dwE1RowStart--, 0, 1, 1);
        for (int dwIdx = 1; dwIdx <= 96; dwIdx++)
        {
            QCheckBox *ptCheckBox = new  QCheckBox(QString("E1_%1").arg(dwIdx));
            if (dwIdx % 8 == 1)
            {
                dwE1RowStart++;
            }
            m_vecE1Checkbox.push_back(ptCheckBox);
            mainlayout->addWidget(ptCheckBox, dwE1RowStart, 1 + ((dwIdx - 1) % 8), 1, 1);
        }
    }

    m_BtnOk.setText("确认");
    mainlayout->addWidget(&m_BtnOk, dwE1RowStart + 1, 7, 1, 2);
    connect(&m_BtnOk, SIGNAL(clicked()), this,  SLOT(slot_config()));
    setWindowTitle(strName);
}

void PortConfig::slot_ChangeGE(int dwGEIdx)
{
    QComboBox *ComboxSender  = (QComboBox*)sender();
    for (int dwIdx = 0; dwIdx < m_vecComboxGeName.size(); dwIdx++)
    {
        if (m_vecComboxGeName[dwIdx] == ComboxSender)
        {
            continue;
        }
        disconnect(m_vecComboxGeName[dwIdx], 0, this, 0);
        int dwCurGEPort = CurGEPortIdx(dwIdx);
        int dwCnt = m_vecComboxGeName[dwIdx]->count();
        for (int i = 0; i < dwCnt; i++)
        {
            m_vecComboxGeName[dwIdx]->removeItem(0);
        }
        for (int i = 1; i <= 16; i++)
        {
            if (ComboxSender->currentText().mid(2).toInt() == i)
            {
                continue;
            }
            m_vecComboxGeName[dwIdx]->addItem(QString("GE%1").arg(i));
        }
        m_vecComboxGeName[dwIdx]->setCurrentText("GE" + QString::number(dwCurGEPort));
        connect(m_vecComboxGeName[dwIdx], SIGNAL(currentIndexChanged(int)), this, SLOT(slot_ChangeGE(int)));
    }
}

void PortConfig::slot_config()
{
    m_ptLable->setText(m_leName.text());
    setWindowTitle(m_leName.text());

    for (int dwIdx = 0; dwIdx < m_vecComboxGeName.size(); dwIdx++)
    {
        T_LinePort &tLinePort = (*m_pMapDevs2GE)[m_dwIdx][dwIdx];
        tLinePort.dwGEPort = CurGEPortIdx(dwIdx);
        disconnect(m_vecComboxGeName[dwIdx], 0, this, 0);
    }

    if ((*m_pMapDevs2E1)[m_dwIdx].size())
    {
        (*m_pMapDevs2E1)[m_dwIdx][0].vecE1Ports.clear();
        for (int dwIdx = 0; dwIdx < m_vecE1Checkbox.size(); dwIdx++)
        {
            if (m_vecE1Checkbox[dwIdx]->isChecked())
            {
                (*m_pMapDevs2E1)[m_dwIdx][0].vecE1Ports.push_back(dwIdx + 1);
            }
        }
    }
    m_bMonited = m_cbSelect.isChecked();
    this->close();
    emit signal_config(m_dwIdx);
}

void PortConfig::MyShow()
{
    for (int dwIdx = 0; dwIdx < m_vecComboxGeName.size(); dwIdx++)
    {
        int dwGEPort = (*m_pMapDevs2GE)[m_dwIdx][dwIdx].dwGEPort;

        int dwCnt = m_vecComboxGeName[dwIdx]->count();
        for (int i = 0; i < dwCnt; i++)
        {
            m_vecComboxGeName[dwIdx]->removeItem(0);
        }
        for (int i = 1; i <= 16; i++)
        {
            if (i != dwGEPort && IsGESelectedByOther(i))
            {
                continue;
            }
            m_vecComboxGeName[dwIdx]->addItem(QString("GE%1").arg(i));
        }
        m_vecComboxGeName[dwIdx]->setCurrentIndex(m_vecComboxGeName[dwIdx]->findText((QString("GE%1").arg(dwGEPort))));
        connect(m_vecComboxGeName[dwIdx], SIGNAL(currentIndexChanged(int)), this, SLOT(slot_ChangeGE(int)));
    }

    int dwCheckSize = m_vecE1Checkbox.size();
    if (dwCheckSize)
    {
        for (int dwIdx = 0; dwIdx < dwCheckSize; dwIdx++)
        {
            m_vecE1Checkbox[dwIdx]->setEnabled(true);
            if (!(*m_pMapDevs2E1)[m_dwIdx][0].vecE1Ports.contains(dwIdx + 1))
            {
                m_vecE1Checkbox[dwIdx]->setChecked(false);
            }

        }
        MAPDEV2LINEPORT::iterator b = m_pMapDevs2E1->begin();
        for (; b != m_pMapDevs2E1->end(); b++)
        {
            if (b.key() == m_dwIdx || b.value().size() == 0)
            {
                continue;
            }
            QVector<int> &vecE1Port = b.value()[0].vecE1Ports;
            for (int dwIdx = 0; dwIdx < vecE1Port.size(); dwIdx++)
            {
                if (dwCheckSize < vecE1Port[dwIdx])
                    break;
                m_vecE1Checkbox[vecE1Port[dwIdx] - 1]->setEnabled(false);
            }
        }
    }
    m_cbSelect.setChecked(m_bMonited);
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    this->show();
}

bool PortConfig::IsGESelectedByOther(int dwGEIdx)
{
    for (int dwIdx = 0; dwIdx < m_vecComboxGeName.size(); dwIdx++)
    {
        QComboBox *ptBox = m_vecComboxGeName[dwIdx];
        if (ptBox->currentText().mid(2).toInt() == dwGEIdx)
        {
            return true;
        }
    }
    return false;
}

int PortConfig::CurGEPortIdx(int dwComboxIdx)
{
    if (dwComboxIdx >= m_vecComboxGeName.size())
    {
        return -1;
    }
    return m_vecComboxGeName[dwComboxIdx]->currentText().mid(2).toInt();
}

void PortConfig::ShowWarn(MAPDEV2LINEPORT *pMapDev2Port)
{
    MAPDEV2LINEPORT::iterator itr = pMapDev2Port->begin();

    for ( ; itr != pMapDev2Port->end(); itr++)
    {
        QVector<T_LinePort> &vecTLinePort = itr.value();
        for (int dwIdx = 0; dwIdx < vecTLinePort.size(); dwIdx++)
        {
            T_LinePort &tLinePort = vecTLinePort[dwIdx];
            bool bWarn = PortFrame::GetInfo(QString("GE%1").arg(tLinePort.dwGEPort));
        }
    }
}

void PortConfig::Unselect()
{
    m_bMonited = false;
    m_cbSelect.setChecked(false);
}

bool PortConfig::IsSelected()
{
    return m_cbSelect.isChecked();
}
