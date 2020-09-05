#ifndef BASICINFO
#define BASICINFO

#include <QColor>
#include <QLabel>

typedef struct Controler
{
    QString m_strOid;
    QString m_strName;
    QString m_strPort;
    int m_dwIdx;
    bool m_bWarn;
}T_Controler;


typedef struct T_LineOfBasicGraph
{
    int dwShape;
    QString strWords;
}T_LineOfBasicGraph;


#endif // BASICINFO

