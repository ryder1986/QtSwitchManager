#ifndef PORTFRAME
#define PORTFRAME

#include <QVector>
#include <QMap>
#include <QLabel>
#include <QTableWidget>
#include <QStringList>
#include <QMutex>
#include <QString>
#include "Widget.h"
#include "BasicGraph.h"
#include "basicinfo.h"
#include "Snmp_lsm.h"

#define UP "1"
#define DOWN "2"

class SingleRow : public QTableWidget
{
    Q_OBJECT
public:
    SingleRow(int dwShape, QStringList &strWordsList, QWidget *parent = 0);
    QVector<BasicGraph*> m_vecSingleRow;
};

class PortFrame : public Widget
{
    Q_OBJECT
public:
    PortFrame(Snmp_Lsm *ptSnmp, Widget *parent = 0);

    static void SetWarn(T_Controler &tControler);
    static bool GetInfo(QString strPort);
    static bool SetInfoByOid(QString strOid, QString strMsg);
    static bool SetInfoByPort(QString strPort, QString StrMsg);
private:
    void FindControler(QString &strControler, int &dwIdx, QString &strName);
    void InitConfig();
    void SnmpGet();
    QString m_strIp;
    Snmp_Lsm *m_ptSnmp;
    static QMutex mutex;
    static QMap<QString, T_Controler> Oid2port;
    static QMap<QString, BasicGraph*> BasicGraphs;
};

#endif //PORTFRAME
