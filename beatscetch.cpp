#include "beatscetch.h"

beatScetch::beatScetch(QWidget* parent =0):sScetch(parent)
{
    this->setMouseTracking(true);
}

void beatScetch::paintEvent(QPaintEvent * /* event */)
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
        for(int i=0;i<mPlane->TopList()->count();i++)
        {
            cur=mPlane->TopList()->at(i);
            index=mPlane->TopSortedList()->indexOf(cur)+base+mPlane->botPr()+cur->Inc();
            if(state==1 && cur->isActive()) cl=QColor(Qt::green);
            else cl=QColor(Qt::black);
            painter.fillRect(trXt(i)+2,trY(index),trXt(1)-ofs-4,mouseZone,
                             QBrush(cl,Qt::FDiagPattern));
            painter.setPen(QPen(cl,3));
            painter.drawLine(trXt(i)+2,trY(index),trXt(i+1)-4,trY(index));
            painter.setPen(QPen(cl,2));
            painter.drawLine(trXt(i+1),trY(index),width()-20,trY(index));

            nm=(mPlane->TopSortedList()->indexOf(cur)+1)*10;
            name=QString("%1").arg(nm);
            painter.drawText(trXt(i)-20,trY(index),name);
            for(int j=0;j<cur->nPr();j++)
            {
                if(state==1 && cur->ActivePr()==j) cl=QColor(Qt::green);
                else cl=QColor(Qt::black);
                QRect rc(trXt(i),trY(index+j+1),trXt(1)-ofs,scaleY);
                painter.setPen(QPen(cl,3,Qt::DashDotLine));
                painter.drawRect(rc);
                if(j%2==0) painter.fillRect(rc,QBrush(cl,Qt::BDiagPattern));
                else painter.fillRect(rc,QBrush(cl,Qt::FDiagPattern));
                painter.setPen(QPen(cl,2));
                painter.drawLine(trXt(i+1),trY(index+j+1),width()-20,trY(index+j+1));

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
                if(state==1 && cur->isActive()) cl=QColor(Qt::green);
                else cl=QColor(Qt::black);
                if(mPlane->BotList()->at(i)!=mPlane->getBotSurface())
                {
                    painter.fillRect(trXb(i)+2,trY(index),trXb(1)-ofs-4,-mouseZone,
                                 QBrush(cl,Qt::FDiagPattern));
                    painter.setPen(QPen(cl,3));
                    painter.drawLine(trXb(i)+2,trY(index),trXb(i+1)-4,trY(index));
                    painter.setPen(QPen(cl,2));
                    painter.drawLine(trXb(i+1),trY(index),width()-20,trY(index));

                    nm=(1+mPlane->BotSortedList()->indexOf(cur)+mPlane->TopList()->count())*10;
                    name=QString("%1").arg(nm);
                    painter.drawText(trXb(i)-20,trY(index),name);
                    for(int j=0;j<cur->nPr();j++)
                    {
                        if(state==1&& cur->ActivePr()==j) cl=QColor(Qt::green);
                        else cl=QColor(Qt::black);
                        QRect rc(trXb(i),trY(index-j),trXb(1)-ofs,scaleY);
                        painter.setPen(QPen(cl,3,Qt::DashDotLine));
                        painter.drawRect(rc);
                        if(j%2==0) painter.fillRect(rc,QBrush(cl,Qt::BDiagPattern));
                        else painter.fillRect(rc,QBrush(cl,Qt::FDiagPattern));
                        painter.setPen(QPen(cl,2));
                        painter.drawLine(trXb(i+1),trY(index-j-1),width()-20,trY(index-j-1));

                        nm=(1+mPlane->BotSortedList()->indexOf(cur)+mPlane->TopList()->count())*10+j+1;
                        name=QString("%1").arg(nm);
                        painter.drawText(trXb(i)-20,trY(index-j-1),name);


                    }
                }
            }
        }
        base=mPlane->TopList()->count()+1;
        double nob;
        for(int i=0;i<mPlane->opList()->count();i++)
        {
            operation* op=mPlane->opList()->at(i);
            nob=op->count();
            if(nob==0) nob=1;
            if(state==1 && mPlane->opList()->at(i)->isActive()) cl=Qt::green;
            else cl=Qt::black;
            painter.setPen(QPen(cl,3));
            painter.drawLine(trX(base),trY(0),trX(base+nob),trY(0));
            painter.drawLine(trX(base),trY(0)-5,trX(base),trY(0)+5);
            painter.drawLine(trX(base+nob),trY(0)-5,trX(base+nob),trY(0)+5);
            QString opName(QString("%1").arg((i+1)*5));
            while(opName.size()!=3) opName.prepend("0");
            painter.drawText(trX(base+nob/2)-10,trY(0)+15,opName);
            for(double j=0;j<op->bList.count();j++)
            {
                beat* b=op->bList.at(j);
                switch (b->type)
                {
                case blank:
                    cl=QColor(125,125,125);
                    break;
                case baseSur:
                    cl=QColor(255,140,0);
                    break;
                case before:
                    cl=Qt::red;
                    break;
                case after:
                    cl=Qt::blue;
                    break;
                case tvr:
                    cl=QColor(96,0,160);
                    break;
                default:
                    cl=Qt::green;
                }
                painter.setPen(QPen(cl,3));
                double bx=base+((2*j+1)/2.);

                if(!b->twoSides)
                {
                    QPainterPath path;
                    path.moveTo(trX(base+nob/2.),trY(0));

                    if((j+1)/(nob+1)==0.5)
                        path.lineTo(trX(bx),trY(1));
                    else if((j+1)/(nob+1)<0.5)
                        path.cubicTo(trX(bx-0.20),trY(0.5),trX(bx),trY(0.5),trX(bx),trY(1));
                    else
                        path.cubicTo(trX(bx+0.20),trY(0.5),trX(bx),trY(0.5),trX(bx),trY(1));
                    int by,beatB;
                    surface* sr;
                    if(b->top1)
                    {
                        beatB=mPlane->BotList()->count()+2+mPlane->botPr();
                        sr=mPlane->TopList()->at(b->side1-1);
                        by=mPlane->TopSortedList()->indexOf(sr)+ beatB+sr->Inc()+b->pr1;
                    }
                    else
                    {
                        sr=mPlane->BotList()->at(b->side1-1);
                        by=mPlane->BotSortedList()->indexOf(sr)+2+sr->Inc()-b->pr1;
                    }
                    path.lineTo(trX(bx),trY(by));
                    painter.drawPath(path);
                    painter.drawLine(trX(bx)-5,trY(by)+7,trX(bx),trY(by));
                    painter.drawLine(trX(bx)+5,trY(by)+7,trX(bx),trY(by));
                    painter.rotate(+90);
                    painter.drawText(trY(2)+5,-trX(bx)-5,b->name1);
                    painter.rotate(-90);
                    if(b->type==baseSur)
                    {
                        painter.setPen(QPen(cl,1));
                        painter.drawEllipse(trX(bx)-5,trY(by)-8,10,10);
                        QPainterPath bsPath;
                        bsPath.moveTo(trX(bx)-20,trY(by)-20);
                        bsPath.lineTo(trX(bx)-10,trY(by)-20);
                        bsPath.lineTo(trX(bx),trY(by));
                        bsPath.lineTo(trX(bx)+10,trY(by)-20);
                        bsPath.lineTo(trX(bx)+20,trY(by)-20);
                        painter.drawPath(bsPath);
                    }
                }
                else
                {
                    int by1, by2, beatB1, beatB2;
                    surface* sr;
                    if(b->top1)
                    {
                        beatB1=mPlane->BotList()->count()+2+mPlane->botPr();
                        sr=mPlane->TopList()->at(b->side1-1);
                        by1=mPlane->TopSortedList()->indexOf(sr)+ beatB1+sr->Inc()+b->pr1;
                    }
                    else
                    {
                        sr=mPlane->BotList()->at(b->side1-1);
                        by1=mPlane->BotSortedList()->indexOf(sr)+2+sr->Inc()-b->pr1;
                    }
                    if(b->top2)
                    {
                        beatB2=mPlane->BotList()->count()+2+mPlane->botPr();
                        sr=mPlane->TopList()->at(b->side2-1);
                        by2=mPlane->TopSortedList()->indexOf(sr)+ beatB2+sr->Inc()+b->pr2;
                    }
                    else
                    {
                        sr=mPlane->BotList()->at(b->side2-1);
                        by2=mPlane->BotSortedList()->indexOf(sr)+2+sr->Inc()-b->pr2;
                    }
                    painter.drawLine(trX(bx)-2,trY(by1),trX(bx)-2,trY(by2)-4);
                    painter.drawLine(trX(bx)+2,trY(by1),trX(bx)+2,trY(by2)-4);
                    painter.drawLine(trX(bx)-4,trY(by2)-8,trX(bx),trY(by2));
                    painter.drawLine(trX(bx)+4,trY(by2)-8,trX(bx),trY(by2));
                }
            }

            base+=1+nob;
        }

    }

}


void beatScetch::mousePressEvent(QMouseEvent * event )
{
    if(state==1)
    {
        surface* cur;
        int base=mPlane->BotList()->count()+2+mPlane->botPr();
        int index;
        int sx1,sx2,sy;
        int sendSide=-1;
        int sendOp=-1;
        for(int i=0;i<mPlane->TopList()->count();i++)
        {
            cur=mPlane->TopList()->at(i);
            index=mPlane->TopSortedList()->indexOf(cur)+base+cur->Inc();
            sy=trY(index);
            if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone) sendSide=100+(i+1)*10;
            else
            {
                for(int j=0;j<cur->nPr();j++)
                {
                    sy=trY(index+j+1);
                    if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone)
                    {
                        sendSide=100+(i+1)*10+(j+1);
                    }
                }
            }

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
                    sy=trY(index);
                    if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone) sendSide=(i+1)*10;
                    else
                    {
                        for(int j=0;j<cur->nPr();j++)
                        {
                            sy=trY(index-j-1);
                            if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone)
                            {
                                sendSide=(i+1)*10+(j+1);
                            }
                        }
                    }
                }
            }
        }
        base=mPlane->TopList()->count()+1;
        int nob;
        for(int i=0;i<mPlane->opList()->count();i++)
        {
            nob=mPlane->opList()->at(i)->count();
            if(nob==0) nob++;
            sx1=trX(base);
            sx2=trX(base+1+nob);
            if(sx1<=event->x() && sx2>=event->x()) sendOp=i;
            base+=1+nob;
        }
        if(sendSide!=-1 && sendOp!=-1)
            emit sideChoosed(sendSide,sendOp);
        update();
    }
    if(state==2)
    {

        int base=mPlane->TopList()->count()+1;
        int nob;
        for(int i=0;i<mPlane->opList()->count();i++)
        {
            nob=mPlane->opList()->at(i)->count();
            if(nob==0) nob++;
            double sx1=trX(base);
            double sx2=trX(base+1+nob);
            if(sx1<=event->x() && sx2>=event->x()) mPlane->opList()->at(i)->setActive(true);
            else mPlane->opList()->at(i)->setActive(false);
            base+=1+nob;
        }
        update();

    }
    if(state==3)
    {

        int base=mPlane->TopList()->count()+1;
        int nob;
        for(int i=0;i<mPlane->opList()->count();i++)
        {
            nob=mPlane->opList()->at(i)->count();
            double sx1=trX(base);
            double sx2=trX(base+1+nob);
            if(sx1<=event->x() && sx2>=event->x()) mPlane->opList()->at(i)->setActive(true);
            else mPlane->opList()->at(i)->setActive(false);
            base+=2+nob;
        }
        update();
    }

}

void beatScetch::mouseMoveEvent(QMouseEvent *event)
{
    if(state==1)
    {
        surface* cur;
        int base=mPlane->BotList()->count()+2+mPlane->botPr();
        int index;
        int sx1,sx2,sy;
        for(int i=0;i<mPlane->TopList()->count();i++)
        {
            cur=mPlane->TopList()->at(i);
            index=mPlane->TopSortedList()->indexOf(cur)+base+cur->Inc();
            sy=trY(index);
            if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone) cur->setActive(true);
            else
            {
                bool ap=false;
                cur->setActive(false);
                for(int j=0;j<cur->nPr();j++)
                {
                    sy=trY(index+j+1);
                    if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone && !ap)
                    {
                        cur->setActivePr(j);
                        ap=!ap;
                    }
                }
                if(!ap)
                    cur->setActivePr(-1);
            }

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
                    cur->setActive(false);
                    sy=trY(index);
                    if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone) cur->setActive(true);
                    else
                    {

                        cur->setActive(false);
                        bool ap=false;
                        for(int j=0;j<cur->nPr();j++)
                        {
                            sy=trY(index-j-1);
                            if(event->y()>=sy-mouseZone && event->y()<=sy+mouseZone && !ap)
                            {
                                cur->setActivePr(j);
                                ap=!ap;
                            }
                        }
                        if(!ap)
                            cur->setActivePr(-1);
                    }
                }
            }
        }
        base=mPlane->TopList()->count()+1;
        int nob;
        for(int i=0;i<mPlane->opList()->count();i++)
        {
            nob=mPlane->opList()->at(i)->count();
            if(nob==0) nob++;
            sx1=trX(base);
            sx2=trX(base+1+nob);
            if(sx1<=event->x() && sx2>=event->x()) mPlane->opList()->at(i)->setActive(true);
            else mPlane->opList()->at(i)->setActive(false);
            base+=1+nob;
        }
        update();
    }
    if(state==2)
    {

        int base=mPlane->TopList()->count()+1;
        int nob;
        for(int i=0;i<mPlane->opList()->count();i++)
        {
            nob=mPlane->opList()->at(i)->count();
            if(nob==0) nob++;
            double sx1=trX(base);
            double sx2=trX(base+1+nob);
            if(sx1<=event->x() && sx2>=event->x()) mPlane->opList()->at(i)->setActive(true);
            else mPlane->opList()->at(i)->setActive(false);
            base+=1+nob;
        }
        update();

    }
    if(state==3)
    {

        int base=mPlane->TopList()->count()+1;
        int nob;
        for(int i=0;i<mPlane->opList()->count();i++)
        {
            nob=mPlane->opList()->at(i)->count();
            double sx1=trX(base);
            double sx2=trX(base+1+nob);
            if(sx1<=event->x() && sx2>=event->x()) mPlane->opList()->at(i)->setActive(true);
            else mPlane->opList()->at(i)->setActive(false);
            base+=2+nob;
        }
        update();
    }

}

void beatScetch::setTrs()
{
    double nOp;
    for(int i=0;i<mPlane->opList()->count();i++)
    {
        if(mPlane->opList()->at(i)->count()==0)
            nOp+=mPlane->opList()->at(i)->count()+2;
        else
            nOp+=mPlane->opList()->at(i)->count()+1;
    }
    int nPr=mPlane->totalPr();
    int cnt=mPlane->TopList()->count();
    scaleXt=(width()-2*ofs)/((double)(cnt+nOp));
    int cnt2=0;
    if(mPlane->hasBot())
    {
        cnt2=mPlane->BotList()->count();
        scaleXb=(scaleXt*cnt)/(double)cnt2;
    }
    scaleX=scaleXt;
    scaleY=(height()-2*ofs)/(double)(cnt+cnt2+2+nPr);
    mouseZone=scaleY/4;
}



