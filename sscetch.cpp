#include "sscetch.h"

sScetch::sScetch(QWidget *parent):tScetch(parent)
{
    this->setMouseTracking(true);
}

void sScetch::paintEvent(QPaintEvent * /* event */)
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
        surface* cur=mPlane->BotList()->first();
        int base=mPlane->BotList()->count()+2+mPlane->botPr();
        int index=mPlane->BotSortedList()->indexOf(cur)+base;
        QPainterPath path;
        path.moveTo(trXt(0),trY(index));
        for(int i=0;i<mPlane->TopList()->count();i++)
        {
            cur=mPlane->TopList()->at(i);
            index=mPlane->TopSortedList()->indexOf(cur)+base+cur->Inc();
            path.lineTo(trXt(i),trY(index));
            path.lineTo(trXt(i+1),trY(index));
        }
        if(mPlane->hasBot())
        {
            base=2;
            for(int i=mPlane->BotList()->count()-1;i>-1;i--)
            {
                cur=mPlane->BotList()->at(i);
                index=mPlane->BotSortedList()->indexOf(cur)+base+cur->Inc();
                if(mPlane->BotList()->at(i)==mPlane->getBotSurface())
                    index=0;
                path.lineTo(trXb(i+1),trY(index));
                path.lineTo(trXb(i),trY(index));
            }
            path.closeSubpath();
        }
        else
        {
            path.lineTo(trXt(mPlane->TopList()->count()),trY(0));
            path.lineTo(trXt(0),trY(0));
            path.closeSubpath();

        }

        painter.setPen(QPen(Qt::black,2));
        painter.drawPath(path);

        //surfaces

        base=mPlane->BotList()->count()+2;
        QColor cl;
        int nm;
        QString name;
        bool act=this->isActive();
        for(int i=0;i<mPlane->TopList()->count();i++)
        {
            cur=mPlane->TopList()->at(i);
            index=mPlane->TopSortedList()->indexOf(cur)+base+mPlane->botPr()+cur->Inc();
            if(act&& cur->isActive()) cl=QColor(Qt::red);
            else cl=QColor(Qt::green);
            painter.fillRect(trXt(i)+2,trY(index),trXt(1)-ofs-4,mouseZone,
                             QBrush(cl,Qt::FDiagPattern));
            painter.setPen(QPen(cl,3));
            painter.drawLine(trXt(i)+2,trY(index),trXt(i+1)-4,trY(index));
            painter.setPen(QPen(Qt::black,3));
            nm=(mPlane->TopSortedList()->indexOf(cur)+1)*10;
            name=QString("%1").arg(nm);
            painter.drawText(trXt(i)-20,trY(index),name);
            for(int j=0;j<cur->nPr();j++)
            {
                painter.setPen(QPen(QColor(0,0,255,127),3,Qt::DashDotLine));
                QRect rc(trXt(i),trY(index+j+1),trXt(1)-ofs,scaleY);
                painter.drawRect(rc);
                if(j%2==0) painter.fillRect(rc,QBrush(QColor(0,0,255,127),Qt::BDiagPattern));
                else painter.fillRect(rc,QBrush(QColor(0,0,255,127),Qt::FDiagPattern));

                painter.setPen(QPen(Qt::black,3));
                nm=(mPlane->TopSortedList()->indexOf(cur)+1)*10+j+1;
                name=QString("%1").arg(nm);
                painter.drawText(trXt(i)-20,trY(index+j+1),name);

            }
        }
        if(mPlane->hasBot())
        {
            base=2;
            for(int i=0;i<mPlane->BotList()->count();i++)
            {
                cur=mPlane->BotList()->at(i);
                index=mPlane->BotSortedList()->indexOf(cur)+base+cur->Inc();
                if(act && cur->isActive()) cl=QColor(Qt::red);
                else cl=QColor(Qt::green);
                if(mPlane->BotList()->at(i)!=mPlane->getBotSurface())
                {
                    painter.fillRect(trXb(i)+2,trY(index),trXb(1)-ofs-4,-mouseZone,
                                 QBrush(cl,Qt::FDiagPattern));
                    painter.setPen(QPen(cl,3));
                    painter.drawLine(trXb(i)+2,trY(index),trXb(i+1)-4,trY(index));

                    painter.setPen(QPen(Qt::black,3));
                    nm=(1+mPlane->BotSortedList()->indexOf(cur)+mPlane->TopList()->count())*10;
                    name=QString("%1").arg(nm);
                    painter.drawText(trXb(i)-20,trY(index),name);

                    for(int j=0;j<cur->nPr();j++)
                    {
                        painter.setPen(QPen(QColor(0,0,255,127),3,Qt::DashDotLine));
                        QRect rc(trXb(i),trY(index-j),trXb(1)-ofs,scaleY);
                        painter.drawRect(rc);
                        if(j%2==0) painter.fillRect(rc,QBrush(QColor(0,0,255,127),Qt::BDiagPattern));
                        else painter.fillRect(rc,QBrush(QColor(0,0,255,127),Qt::FDiagPattern));

                        painter.setPen(QPen(Qt::black,3));
                        nm=(1+mPlane->BotSortedList()->indexOf(cur)+mPlane->TopList()->count())*10+j+1;
                        name=QString("%1").arg(nm);
                        painter.drawText(trXb(i)-20,trY(index-j-1),name);


                    }
                }
            }
        }

    }

}


void sScetch::mousePressEvent(QMouseEvent * event )
{

    surface* cur;
    int base=mPlane->BotList()->count()+2+mPlane->botPr();
    int index;
    int sx1,sx2,sy;
    for(int i=0;i<mPlane->TopList()->count();i++)
    {
        cur=mPlane->TopList()->at(i);
        index=mPlane->TopSortedList()->indexOf(cur)+base+cur->Inc();
        sx1=trXt(i)+2;
        sx2=trXt(i+1)-4;
        sy=trY(index);
        if(event->x()>=sx1 && event->x()<=sx2)
            if(event->y()>=sy && event->y()<=sy+mouseZone)
                emit sideChoosed(1,i);
    }
    if(mPlane->hasBot())
    {
        base=2;
        for(int i=0;i<mPlane->BotList()->count();i++)
        {
            cur=mPlane->BotList()->at(i);
            index=mPlane->BotSortedList()->indexOf(cur)+base+cur->Inc();
            if(mPlane->BotList()->at(i)!=mPlane->getBotSurface())
            {
                sx1=trXb(i)+2;
                sx2=trXb(i+1)-4;
                sy=trY(index);
                if(event->x()>=sx1 && event->x()<=sx2)
                    if(event->y()<=sy && event->y()>=sy-mouseZone)
                        emit sideChoosed(0,i);
            }
        }
    }

}

void sScetch::mouseMoveEvent(QMouseEvent *event)
{
    if(active)
    {
    surface* cur;
    int base=mPlane->BotList()->count()+2+mPlane->botPr();
    int index;
    int sx1,sx2,sy;
    for(int i=0;i<mPlane->TopList()->count();i++)
    {
        cur=mPlane->TopList()->at(i);
        index=mPlane->TopSortedList()->indexOf(cur)+base+cur->Inc();
        sx1=trXt(i)+2;
        sx2=trXt(i+1)-4;
        sy=trY(index);
        if(event->x()>=sx1 && event->x()<=sx2 &&
            event->y()>=sy && event->y()<=sy+mouseZone) cur->setActive(true);
        else cur->setActive(false);

    }
    if(mPlane->hasBot())
    {
        base=2;
        for(int i=0;i<mPlane->BotList()->count();i++)
        {
            cur=mPlane->BotList()->at(i);
            index=mPlane->BotSortedList()->indexOf(cur)+base+cur->Inc();
            if(mPlane->BotList()->at(i)!=mPlane->getBotSurface())
            {
                sx1=trXb(i)+2;
                sx2=trXb(i+1)-4;
                sy=trY(index);
                if(event->x()>=sx1 && event->x()<=sx2 &&
                    event->y()<=sy && event->y()>=sy-mouseZone) cur->setActive(true);
                else cur->setActive(false);
            }
        }
    }

    update();
    }

}

void sScetch::setTrs()
{
    int n=mPlane->totalPr();
    int cnt=mPlane->TopList()->count();
    scaleXt=(width()-2*ofs)/(double)cnt;
    int cnt2=0;
    if(mPlane->hasBot())
    {
        cnt2=mPlane->BotList()->count();
        scaleXb=(width()-2*ofs)/(double)cnt2;
    }
    scaleX=0;
    scaleY=(height()-2*ofs)/(double)(cnt+cnt2+2+n);
    mouseZone=scaleY/4;
}
