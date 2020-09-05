#ifndef DEVINFOFRAME
#define DEVINFOFRAME

#include <QLabel>
#include "Snmp_lsm.h"
#include "Widget.h"

class DevInfoFrame : public Widget
{
    Q_OBJECT
public:
    DevInfoFrame(Snmp_Lsm *ptSnmp, Widget *parent = 0);
    void SetDevInfo(QString strName, QString strSerial, QString strModel, QString strVersion, QString strIp);
private:
    void ShowLayout();
    void SnmpGetInfo(Snmp_Lsm *ptSnmp);

    QLabel m_labelDevName;
    QLabel m_labelDevSerial;
    QLabel m_labelDevModel;
    QLabel m_labelDevVersion;
    QLabel m_labelDevIp;
};

#endif //DEVINFOFRAME
