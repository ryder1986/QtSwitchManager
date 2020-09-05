#ifndef BASICGRAPH_H
#define BASICGRAPH_H

#include <QWidget>
#include <QString>
#include <QPaintEvent>

//#define CIRCLE 0    //圆形
//#define RECT 1      //矩形
//#define ECLIPSE 2      //椭圆形
//#define RECTANGLE 3   //圆角长方形
//#define SWITCH 4   //交换机端口形状
namespace BG {
enum BGSHAPE{
    CIRCLE,
    RECT,
    ECLIPSE,
    RECTANGLE,
    SWITCH
};

}

class BasicGraph : public QWidget
{
    Q_OBJECT
public:
    BasicGraph(int dwShape, QString strWords, QWidget *parent = 0);
    QString& GetName();
    void SetWarn(bool bWarn, int dwIdx);
protected:
    void paintEvent(QPaintEvent *event);

private:
    QString m_strWords;
    int m_dwShape;
    bool m_bWarn;
    int m_dwWarnIdx;
    bool m_baWarn[8];
};
#endif // BASICGRAPH_H
