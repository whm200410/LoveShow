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
    QPoint                      dragPosition;

    //this is the background image(heart)
    QGraphicsPixmapItem         *m_pixItem;

    //this is use to show all the images(external images folder)
    vector<QGraphicsPixmapItem*> m_vImgs;

    //this is used for animation
    int                          m_nIdxImg;
    int                          m_nTrans;
    int                          m_nDirection;



    //logo
    QGraphicsSimpleTextItem    *m_txtLogo;


    //for snow animation
    struct snowInfo_t
    {
        QGraphicsPixmapItem *m_pix;
        int                 m_initX;
        int                 m_initY;
        int                 m_speedX;
        int                 m_speedY;
    };
    vector<snowInfo_t> m_pixSnow;

    //this is used for words(external txt folder)
    QGraphicsSimpleTextItem    *m_txtLove;
    vector<QString>             m_vStrings;
    int                         m_nIdxTxt;
    int                         m_nTranTxt;
    int                         m_nDirectionTxt;
    QTimer *m_timerSnow;
    QTimer *m_timerAnim;
    QSize m_sizePix;

};

#endif // WIDGET_H
