#ifndef WARNFRAME
#define WARNFRAME

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QString>
#include <QMap>
#include <QRegExp>
#include "basicinfo.h"
#include "Widget.h"

#define E1 "E1"
#define GE "GE"
#define CODEOID "1.3.6.1.4.1.3902.3.302.1.1.2"
#define STATUSOID "1.3.6.1.4.1.3902.3.302.1.1.6"
#define TIMEOID "1.3.6.1.4.1.3902.3.302.1.1.7"
#define DESCOID "1.3.6.1.4.1.3902.3.302.1.1.13"


typedef struct WarnConfig
{
    QString m_strWarnCode;
    QString m_strWarnLevel;
    QString m_strWarnName;
    QString m_strWarnDesc;
    bool m_bGetPort;

    void SetVal(QString strWarnText)
    {
        QStringList strList = strWarnText.split("|");
        if (strList.size() != 5)
        {
            return;
        }
        this->m_strWarnCode = strList[0];
        this->m_strWarnLevel = strList[1];
        this->m_strWarnName = strList[2];
        this->m_strWarnDesc = strList[3];
        this->m_bGetPort = (strList[4] == "1");

    }
}T_WarnConfig;

typedef struct WarnText
{
    QString m_strCode;
    QString m_strLevel;
    QString m_strName;
    QString m_strDesc;
    QString m_strTime;
    QString m_strPort;

}T_WarnText;

class WarnFrame: public Widget
{
    Q_OBJECT
public:
    WarnFrame(Widget *parent = 0);

signals:
    void signal_SendPortWarn();

public slots:
    void slot_ReciveWarnText(QString strOid, QString strMsg);

private:
    bool MatchPortFromMsg(QString strMsg, QString strType, QString &strPort);
    bool CanStartParase(QString strOid, QString strMsg, QString &strCode, QString &strStatus, QString &strTime, QString &strDesc);
    void UpdateWarnText(T_WarnText &tWarnText, QString strStatus);

    QTableWidget m_TableWiget;
    int m_dwSeq;
    QMap<QString, T_WarnConfig> m_mapCode2WarnConfig;
    QRegExp m_RegExp;
    QMap<QString, QString> m_mapOid2Val;
    QVector<T_WarnText> m_vecWarnText;
};

#endif //WARNFRAME
