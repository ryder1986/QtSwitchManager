#ifndef IPFRAME
#define IPFRAME

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QTableWidget>
#include "Widget.h"

class IpFrame : public Widget
{
    Q_OBJECT
public:
    IpFrame(Widget *parent = 0);

public slots:
    void slot_BtnConfig();
    void slot_CalcClick();

private:
    QTableWidget* GenCalc();
    QLabel m_LabelIp;
    QLineEdit m_LineEditIp;
    QPushButton m_BtnConfig;
    QTableWidget m_Calc;
    int m_dwCalcRow;
    int m_dwCalcCol;
};

#endif //IPFRAME
