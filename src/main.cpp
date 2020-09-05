#include <QApplication>
#include <QTextCodec>
#include "MainWidget.h"
#include "Snmp_lsm.h"
#include <PortFrame.h>
#include <QRegExp>
#include <TopologyFrame.h>
#include "portconfig.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/*
    char b[] = "127.0.0.1";
    char c[ ] = ".1.3.6.1.2.1.1.5.0"; //"1.3.6.1.2.1.1.1.0";
    char e[] = "1.3.6.1.6.3.16.1.5.2.1.6.5.95.97.108.108.95.1.2";
    Snmp_Lsm *s = new Snmp_Lsm();
    QString strSnmpInfo;
    s->SnmpGet(strSnmpInfo, b, c);
    QMap<QString, QString> map;
    s->SnmpWalk(map, b, e);
    s->StartTrap(162);
*/
    //PortConfig p(0, "啊", "上端口", "下端口");
    //p.show();
    MainWidget w;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  // QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    w.show();

    return a.exec();
}
