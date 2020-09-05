#ifndef PORTCONFIG_H
#define PORTCONFIG_H

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>
#include "Widget.h"

typedef struct LinePort{
    QString strLineName;
    int dwGEPort;
    QVector<int> vecE1Ports;
}T_LinePort;

typedef QMap<int, QVector<T_LinePort> > MAPDEV2LINEPORT;

class PortConfig : public Widget
{
    Q_OBJECT
public:
    PortConfig(int dwIdx, QString strNname, QString strGE, QString strE1, MAPDEV2LINEPORT *pMapDevs2GE, MAPDEV2LINEPORT *pMapDevs2E1, QLabel *ptLable, Widget *parent = 0);
    void MyShow();
    int CurGEPortIdx(int dwComboxIdx);
    bool IsSelected();
    void Unselect();

signals:
    void signal_config(int dwDevId);

protected slots:
    void slot_config();
    void slot_ChangeGE(int dwGEIdx);

private:
    bool IsGESelectedByOther(int dwIdx);
    void ShowWarn(MAPDEV2LINEPORT *pMapDev2Port);
    QLabel m_lbSelect;
    QCheckBox m_cbSelect;
    QLabel m_lbName;
    QLineEdit m_leName;
    QVector<QComboBox *> m_vecComboxGeName;
    QLabel m_LbE1Name;
    QVector<QCheckBox *> m_vecE1Checkbox;
    int m_dwIdx;
    QPushButton m_BtnOk;

    QLabel *m_ptLable;
    MAPDEV2LINEPORT *m_pMapDevs2GE;  //全局记录设备id和GE口的映射关系
    MAPDEV2LINEPORT *m_pMapDevs2E1;  //全局记录设备id和E1口的映射关系
    bool m_bMonited;
};

#endif // PORTCONFIG_H
