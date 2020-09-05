#ifndef SNMPLSM
#define SNMPLSM
#include <QObject>
#include <QString>
#include <QMap>
#include <QThread>

#include <libsnmp.h>
#include "snmp_pp/snmp_pp.h"
#include "snmp_pp/collect.h"
#include "snmp_pp/notifyqueue.h"
using namespace Snmp_pp;
#define BULK_MAX 10


class Snmp_Lsm : public QObject
{
  //  Q_OBJECT

public:
    Snmp_Lsm(int dwRetries = 1, int dwTimeout = 3, u_short wAgentPort=161, const char *pcCommunity = "public", u_short wManagerPort=162);
    int SnmpGet(QString &strRet, char *ip, char *oid_array);
    int SnmpWalk(QMap<QString, QString> &map_SnmpWalkRet, char *ip, char *oid);
    int StartTrap(int wManagerPort);
    void EmitPortWarnSignal();
    void EmitWarnTextSignal(QString strOid, QString strMsg);
    ~Snmp_Lsm();
signals:
   // void signal_SendPortWarn();
   // void signal_SendWarnText(QString strOid, QString strMsg);
private slots:
private:
    u_short m_wAgentPort;
    QString m_strCommunity;
    CTarget m_ctarget;
    Snmp *m_TrapSnmp;
};

#endif //SNMPLSM
