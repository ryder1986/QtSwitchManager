#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSettings>
#include "DevInfoFrame.h"

DevInfoFrame::DevInfoFrame(Snmp_Lsm *ptSnmp, Widget *parent) : Widget(parent)
{
    SnmpGetInfo(ptSnmp);
    ShowLayout();
}

void DevInfoFrame::SnmpGetInfo(Snmp_Lsm *ptSnmp)
{
    QSettings setting(QString::fromStdString("./config.ini"), QSettings::IniFormat);
    setting.beginGroup("Oid");
    QString strIp = setting.value("IP").toString();
    setting.endGroup();

    setting.beginGroup("DevInfo");
    setting.setIniCodec("UTF-8");
    QString strDevName;
    ptSnmp->SnmpGet(strDevName, strIp.toLatin1().data(), setting.value("Name").toByteArray().data());
    m_labelDevName.setText("设备名称：" + strDevName);
    QString strSerial;
    ptSnmp->SnmpGet(strDevName, strIp.toLatin1().data(), setting.value("Serial").toByteArray().data());
    m_labelDevSerial.setText("设备编号：" + strSerial);
    QString strModel;
    ptSnmp->SnmpGet(strDevName, strIp.toLatin1().data(), setting.value("Model").toByteArray().data());
    m_labelDevModel.setText("设备型号：" + strModel);
    QString strDevVersion;
    ptSnmp->SnmpGet(strDevName, strIp.toLatin1().data(), setting.value("Version").toByteArray().data());
    m_labelDevVersion.setText("设备版本：" + strDevVersion);
    QString strDevIP;
    ptSnmp->SnmpGet(strDevName, strIp.toLatin1().data(), setting.value("IP").toByteArray().data());
    m_labelDevIp.setText("设备IP:" + strDevIP);
    setting.endGroup();
}

void DevInfoFrame::ShowLayout()
{
    //显示布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *layout = new QVBoxLayout();
    mainLayout->addStretch();
    mainLayout->addLayout(layout);
    mainLayout->addStretch();

    QTableWidget *tableWidget = new QTableWidget();
    tableWidget->setRowCount(5);
    tableWidget->setColumnCount(1);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->setCellWidget(0, 0, &m_labelDevName);
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);
    tableWidget->setItem(0, 0, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);
    tableWidget->setCellWidget(1, 0, &m_labelDevSerial);
    tableWidget->setItem(1, 0, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);
    tableWidget->setCellWidget(2, 0, &m_labelDevModel);
    tableWidget->setItem(2, 0, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);
    tableWidget->setCellWidget(3, 0, &m_labelDevVersion);
    tableWidget->setItem(3, 0, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::NoItemFlags);
    tableWidget->setCellWidget(4, 0, &m_labelDevIp);
    tableWidget->setItem(4, 0, item);
    layout->addStretch();
    layout->addWidget(tableWidget);
    layout->addStretch();
}

void DevInfoFrame::SetDevInfo(QString strName, QString strSerial, QString strModel, QString strVersion, QString strIp)
{
    m_labelDevName.setText(strName);
    m_labelDevSerial.setText(strSerial);
    m_labelDevModel.setText(strModel);
    m_labelDevVersion.setText(strVersion);
    m_labelDevIp.setText(strIp);
}
