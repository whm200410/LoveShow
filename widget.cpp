#include "widget.h"
#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QCoreApplication>
#include <QGraphicsSimpleTextItem>
#include <QDateTime>
#include <QDir>
namespace {

    const int c_nPixY       = 200;
    const int c_nLogoY      = 640;
    const int c_nTxtY       = 143;
    const int c_nPixHeight  = 390;
}
Widget::Widget(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

    //load icons
    QPixmap pixmap;
    pixmap.load(":/icon/ht.png");
    QPixmap pixmapSnow;
    pixmapSnow.load(":/icon/snow.png");


    //set mask
    pixmapSnow = pixmapSnow.scaled(QSize(24,24));
    resize(pixmap.size());
    setMask(pixmap.mask());

    //create scene
    m_sizePix = pixmap.size();
    setScene(new QGraphicsScene(0, 0, m_sizePix.width(), m_sizePix.height(), this));


    //add heart image
    m_pixItem = new QGraphicsPixmapItem ();
    m_pixItem->setPixmap(pixmap);
    m_pixItem->setTransformationMode(Qt::SmoothTransformation);
    m_pixItem->setPos(0, 0);
    m_pixItem->setZValue(0);
    scene()->addItem(m_pixItem);

    //logo(hei hei)
    {
        m_txtLogo = new QGraphicsSimpleTextItem(tr("By Sheldon"));
        m_txtLogo->setBrush(QBrush(QColor(255, 255,255)));
        auto font = m_txtLogo->font();
        font.setPixelSize(font.pixelSize() * 2);
        font.setPointSize(font.pointSize() * 2);
        m_txtLogo->setFont(font);
        auto rt = m_txtLogo->boundingRect();
        m_txtLogo->setPos(0, 0);
        const int nLogX = m_sizePix.width() / 2 - rt.width() / 2;
        m_txtLogo->setPos(nLogX, c_nLogoY);
        m_txtLogo->setZValue(10);
        scene()->addItem(m_txtLogo);
    }



    //load txt
    {
        m_txtLove = 0;
        QString strDir = QCoreApplication::applicationDirPath();
        strDir += QString("/txt/lv.txt");
        QFile file(strDir);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);

            while(!in.atEnd()) {
                QString line = in.readLine();
                m_vStrings.push_back(line);
            }
            file.close();

            if(m_vStrings.size())
            {
                m_txtLove = new QGraphicsSimpleTextItem(m_vStrings[0]);
                m_txtLove->setBrush(QBrush(QColor(0, 100,255)));
                auto font = m_txtLove->font();
                font.setPointSize(font.pointSize() * 3);
                m_txtLove->setFont(font);
                m_txtLove->setPos(m_sizePix.width(), c_nTxtY);
                m_txtLove->setZValue(10);
                scene()->addItem(m_txtLove);
                m_nIdxTxt = 0;
                m_nTranTxt = 0;
            }
        }
    }

    //load all images
    {
        QString strDir = QCoreApplication::applicationDirPath();
        strDir += QString("/images");
        QDir            logDir(strDir, "*.*");

        QFileInfoList   fList = logDir.entryInfoList();

        for(auto it = fList.rbegin(); it != fList.rend(); ++it)
        {
            QFileInfo &info = *it;
            QPixmap pixImg;
            pixImg.load(info.absoluteFilePath());

            if(pixImg.size().width() > 0)
            {
                auto szImg = pixImg.size();
                const int nImgWidth = szImg.width() * c_nPixHeight / szImg.height();
                const int nImgX     = m_sizePix.width() / 2 - nImgWidth / 2;
                pixImg = pixImg.scaled(QSize(nImgWidth, c_nPixHeight));

                QGraphicsPixmapItem * pixItem = new QGraphicsPixmapItem ();
                pixItem->setPixmap(pixImg);
                pixItem->setTransformationMode(Qt::SmoothTransformation);
                pixItem->setPos(nImgX, c_nPixY);
                pixItem->setZValue(1);
     //           pixItem->hide();
                pixItem->setOpacity(0);
                scene()->addItem(pixItem);
                m_vImgs.push_back(pixItem);
            }
        }
        m_nIdxImg = 0;
        m_nTrans = 0;
        m_nDirection = 1;
    }

    //for snow animation
    qsrand(QDateTime::currentDateTime().time().second());
    const int nMaxSnow = 50;
    for(int i = 0; i < nMaxSnow; ++i)
    {
        snowInfo_t item;
        item.m_pix = new QGraphicsPixmapItem ();
        item.m_pix->setPixmap(pixmapSnow);
        item.m_pix->setTransformationMode(Qt::SmoothTransformation);
        item.m_initX = qrand() % m_sizePix.width();
        item.m_initY = (qrand() % 100);
        item.m_speedY = qrand() % 30;
        item.m_speedX = item.m_speedY / 5;
        if(item.m_speedX <= 1) item.m_speedX  = 2;
        if(item.m_speedY <= 1) item.m_speedY  = 2;
        item.m_pix->setPos(item.m_initX, item.m_initY);
        item.m_pix->setZValue(2);
        m_pixSnow.push_back(item);
        scene()->addItem(item.m_pix);
    }
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_timerSnow = new QTimer(this);
    connect(m_timerSnow, &QTimer::timeout, [&]()
    {
        for(auto &v : m_pixSnow)
        {
            auto pos = v.m_pix->pos();

            if(pos.y() > m_sizePix.height())
            {
                v.m_pix->setPos(v.m_initX, 0);
            }
            else
            {
                v.m_pix->setPos(pos.x() + v.m_speedX, pos.y() + v.m_speedY);
            }
        }
    });
    m_timerSnow->start(50);

    m_timerAnim = new QTimer(this);
    connect(m_timerAnim, &QTimer::timeout, [&]()
    {
        if(m_vImgs.size())
        {
            m_vImgs[m_nIdxImg]->setOpacity( ((double)m_nTrans) / 100);
            m_nTrans += 5 * m_nDirection;
            if(m_nTrans > 100)
            {
                m_nDirection = -1;
                m_nTrans = 100;
            }
            else if(m_nTrans < 0)
            {
                m_nDirection = 1;
                m_nTrans = 0;
                m_nIdxImg ++;

                if(m_nIdxImg == m_vImgs.size())
                {
                    m_nIdxImg = 0;
                }
            }
        }



        if(m_txtLove)
        {
            auto pos = m_txtLove->pos();
            pos.setX(pos.x() - 10);
            auto rt = m_txtLove->boundingRect();
            m_txtLove->setPos(pos.x() - 1, c_nTxtY);

            if(pos.x() + rt.width() <= 0)
            {
                m_nIdxTxt ++;

                if(m_nIdxTxt == m_vStrings.size())
                {
                    m_nIdxTxt = 0;
                }
                m_txtLove->setText(m_vStrings[m_nIdxTxt]);
                auto rt = m_txtLove->boundingRect();
                m_txtLove->setPos(m_sizePix.width(), c_nTxtY);

            }

        }

    });
    m_timerAnim->start(100);
}

Widget::~Widget()
{

}
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        dragPosition = event->globalPos() - frameGeometry().topLeft();
    }else if(event->button() == Qt::RightButton){
        close();
    }
    QGraphicsView::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        move(event->globalPos() - dragPosition);
    }
    else
    {
        qDebug() << event;
    }
    QGraphicsView::mouseMoveEvent(event);
}
