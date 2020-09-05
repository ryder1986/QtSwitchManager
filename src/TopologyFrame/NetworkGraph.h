#ifndef NetworkPic
#define NetworkPic
#include <QPushButton>
#include <QMap>
#include <QVector>
#include "Widget.h"
#include "basicinfo.h"
#include "portconfig.h"

#define SCALEDSIZE 8

#define MONITOR 0
#define UNMONTOR 1
#define NOTUSE 3

typedef struct DevPixmap
{
    int dwIdx;
    const char *pcPixName;
    const char *pcPixNameGray;
    QPushButton *ptPixmap;
    float fXShift;
    float fYShift;
    const char *pcName;
    float fLableXShift;
    float fLableYShift;
    QLabel *ptLable;
    QString strGE;
    QString strE1;
    PortConfig *ptPortConfig;
    int dwStat;
}DevPixmap;

typedef struct Line
{
    const char *pcName;
    float adwLine[4];
    QColor *ptDefautColor;
    bool bWarn;
}Line;

class NetworkGraph : public Widget
{
    Q_OBJECT
public:
    NetworkGraph(DevPixmap *ptDevs, Line *ptLines, Widget *parent = 0);
    void SetDevAndLines(DevPixmap *ptDevs, Line *ptLines);
    DevPixmap *GetDevPixmap();
    Line *GetLines();
    MAPDEV2LINEPORT m_mapDevs2GE;  //记录设备id和GE口的映射关系
    MAPDEV2LINEPORT m_mapDevs2E1;  //记录设备id和E1口的映射关系
signals:
    void singal_clicked();

protected:
    void paintEvent(QPaintEvent *);

private:
    DevPixmap *m_ptDevs;
    Line *m_ptLines;
};

#endif // NetworkPic
