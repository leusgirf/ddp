#include "truescetch.h"

tScetch::tScetch(QWidget *parent) :
    QWidget(parent)
{
    this->setEnabled(true);
    ofs=30;
    mPlane=NULL;
    active=false;
}


void tScetch::paintEvent(QPaintEvent * /* event */)
{

    QPainter painter(this);
    painter.setPen(QPen(Qt::black,3));
    painter.fillRect(rect(),Qt::white);
    painter.drawRect(0,0,width()-1,height()-1);

    painter.drawLine(5,height()-ofs,width()-5,height()-ofs);
    painter.drawLine(ofs,5,ofs,height()-5);
    if(mPlane!=NULL)
    {
        setTrs();

        pPoint* cur=mPlane->getFirst();
        pPoint* next=cur->getNext();
        bool isFirst=false;
        if(mPlane->hasBot())
        {
            bool bot=false;
            QPainterPath path;
            path.moveTo(trX(cur->X()),trY(cur->Y()));
            while(!isFirst)
            {
                if(bot)
                {
                    painter.setPen(QPen(Qt::green,2));
                    painter.drawLine(trX(cur->X()),trY(cur->Y()),trX(cur->X()),trY(0));

                }
                path.lineTo(trX(next->X()),trY(next->Y()));
                cur=cur->getNext();
                next=cur->getNext();
                if(cur==mPlane->getBotPoint()) bot=true;
                if(cur==mPlane->getFirst()) isFirst=true;
            }
            painter.setPen(QPen(Qt::green,3));
            painter.drawPath(path);
            painter.fillPath(path,Qt::FDiagPattern);

        }
        else
        {
            while(!isFirst)
            {
                painter.setPen(QPen(Qt::green,2));
                painter.drawLine(trX(cur->X()),trY(cur->Y()),trX(cur->X()),trY(0));
                painter.setPen(QPen(Qt::green,3));
                painter.drawLine(trX(cur->X()),trY(cur->Y()),trX(next->X()),trY(next->Y()));
                cur=cur->getNext();
                next=cur->getNext();
                if(cur==mPlane->getFirst()) isFirst=true;
            }
        }
        QPainterPath sPath;
        sPath.moveTo(trX(mPlane->TopList()->first()->getP1()->X()),
                     trY(mPlane->TopList()->first()->getP1()->Y()));
        for(int i=0;i<mPlane->TopList()->count();i++)
        {
            sPath.lineTo(trX(mPlane->TopList()->at(i)->getP2()->X()),
                         trY(mPlane->TopList()->at(i)->getP2()->Y()));
            if(i<mPlane->TopList()->count()-1)
            {
                sPath.lineTo(trX(mPlane->TopList()->at(i+1)->getP1()->X()),
                             trY(mPlane->TopList()->at(i+1)->getP1()->Y()));
            }
        }
        if(mPlane->hasBot())
        {
            sPath.lineTo(trX(mPlane->BotList()->last()->getP2()->X()),
                         trY(mPlane->BotList()->last()->getP2()->Y()));
            for(int i=mPlane->BotList()->count()-1;i!=0;i--)
            {
                sPath.lineTo(trX(mPlane->BotList()->at(i)->getP1()->X()),
                             trY(mPlane->BotList()->at(i)->getP1()->Y()));
                if(i>0)
                    sPath.lineTo(trX(mPlane->BotList()->at(i-1)->getP2()->X()),
                                 trY(mPlane->BotList()->at(i-1)->getP2()->Y()));
            }
            sPath.lineTo(trX(mPlane->BotList()->first()->getP1()->X()),
                         trY(mPlane->BotList()->first()->getP1()->Y()));
        }
        else
        {
            sPath.lineTo(trX((mPlane->TopList()->last()->getP2()->X())),
                         trY(0));
            sPath.lineTo(trX(0),trY(0));
        }
        sPath.closeSubpath();
        painter.setPen(QPen(QColor(0,0,255,127),3,Qt::DashDotLine));
        painter.drawPath(sPath);

    }


    painter.end();
}

QSize tScetch::minimumSizeHint() const
{
    return QSize(100, 100);
}
//! [1]

//! [2]
QSize tScetch::sizeHint() const
{
    return QSize(600, 300);
}

int tScetch::minX()
{
    return mPlane->minX();
}

int tScetch::maxX()
{
    return mPlane->maxX();
}

int tScetch::maxY()
{
    return mPlane->maxY();
}

void tScetch::setTrs()
{
    scaleX=(width()-2*ofs)/(double)(maxX()-minX());
    scaleY=(height()-2*ofs)/(double)maxY();
    if(scaleX>scaleY) scaleX=scaleY;
    else scaleY=scaleX;
}

