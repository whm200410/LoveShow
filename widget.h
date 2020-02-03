#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QTimer>

#include <vector>
using namespace std;
class QGraphicsProxyWidget;
class QGraphicsPixmapItem;
class QMovie;
class QLable;
class Widget : public QGraphicsView
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint dragPosition;
    QGraphicsPixmapItem *m_pixItem;
    vector<QGraphicsPixmapItem*> m_vImgs;
    int                          m_nIdxImg;
    int                          m_nTrans;
    int                          m_nDirection;
    QMovie               *m_movie;
    QLable               *m_lable;
    QGraphicsSimpleTextItem    *m_txtLove;
    QGraphicsSimpleTextItem    *m_txtLogo;
    struct snowInfo_t
    {
        QGraphicsPixmapItem *m_pix;
        int                 m_initX;
        int                 m_initY;
        int                 m_speedX;
        int                 m_speedY;
    };
    vector<snowInfo_t> m_pixSnow;
    vector<QString>    m_vStrings;
    int                m_nIdxTxt;
    int                m_nTranTxt;
    int                          m_nDirectionTxt;
    QTimer *m_timer;
    QTimer *m_timerImg;
    QTimer *m_timerTxt;
    QSize m_sizePix;

};

#endif // WIDGET_H
