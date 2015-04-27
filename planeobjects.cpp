#include "planeobjects.h"

pPoint::pPoint(double x, double y)
{
    this->x=x;
    this->y=y;
    round=false;
    strongConnection1=NULL;
    strongConnection2=NULL;
    weakConnection=NULL;
    round=0;
}

bool pPoint::isEquel(pPoint *c)
{
    if(this->X()==c->X() && this->Y()==c->Y()) return true;
    return false;
}

void pPoint::setConnection(pPoint *next, connectionType type)
{
    switch(type)
    {
    case Strong1:
        strongConnection1=next;
        break;
    case Strong2:
        strongConnection2=next;
        break;
    case Weak:
        weakConnection=next;
        break;
    case Strong:
        if(strongConnection1==NULL) strongConnection1=next;
        else strongConnection2=next;
        break;
    }
}

pPoint* pPoint::getConnection(connectionType type)
{
    switch (type) {
    case Strong1:
        return strongConnection1;
        break;
    case Strong2:
        return strongConnection2;
        break;
    case Strong:
        return strongConnection1;
        break;
    case Weak:
        return weakConnection;
        break;
    default:
        return strongConnection1;
        break;
    }
}

void pPoint::connectToPoint(pPoint *p, connectionType type)
{
    switch (type) {
    case Strong1:
        setConnection(p,Strong1);
        p->setConnection(this,Strong1);
        break;
    case Strong2:
        setConnection(p,Strong2);
        p->setConnection(this,Strong2);
        break;
    case Strong:
        setConnection(p,Strong);
        p->setConnection(this,Strong);
        break;
    case Weak:
        setConnection(p,Weak);
        p->setConnection(this,Weak);
        break;
    }
}

surface::surface(pPoint* P1, pPoint* P2)
{
    this->p1.setX(P1->dbX());
    this->p1.setY(P1->dbY());
    this->p2.setX(P2->dbX());
    this->p2.setY(P2->dbY());
    setMid((P1->dbY()+P2->dbY())/2.);
    nOfPr=0;
    inc=0;
    activePr=-1;
}

surface::surface(double x1, double y1, double x2, double y2)
{
    this->p1.setX(x1);
    this->p1.setY(y1);
    this->p2.setX(x2);
    this->p2.setY(y2);
    setMid((p1.dbY()+p2.dbY())/2.);
    nOfPr=0;
    inc=0;
}

void surface::addition(surface* add)
{
    if(this->p1.dbX()>add->getP1()->dbX()) p1.setX(add->getP1()->dbX());
    if(this->p2.dbX()<add->getP2()->dbX()) p2.setX(add->getP2()->dbX());
}


void surface::move(double dx)
{
    p1.setX(p1.dbX()+dx);
    p2.setX(p2.dbX()+dx);
}

void surface::flip()
{
    double tx=p1.dbX();
    double ty=p1.dbY();
    p1.setX(-p2.dbX());
    p1.setY(p2.dbY());
    p2.setX(-tx);
    p2.setY(ty);
}


plane::plane()
{
    bSurface=NULL;
    operation* op= new operation();
    oList.append(op);
}

void plane::addPoint(pPoint *p)
{
    bool ex=false;
    for(int i =0;i<pList.count();i++)
    {
        if(!ex)
            if(pList.at(i)->isEquel(p)) ex=true;
    }
    if(ex) ;//delete p;
    else pList.append(p);
}

void plane::addPoint(double x,double y)
{
    pPoint* p =new pPoint(x,y);
    this->addPoint(p);
}

void plane::addPlane(int x, int y, bool positive)
{
    Q_UNUSED(positive);
    Q_UNUSED(y);
    pPoint* p1=0;
    int sY=this->maxY();
    for(int i=sY;i!=-1;i--)
    {
        for(int k=0;k<pList.count();k++)
        {
            if(p1==NULL && pList.at(k)->X()==x && pList.at(k)->Y()==i)
            {
                p1=pList.at(k);
            }
            else if (pList.at(k)->X()==x && pList.at(k)->Y()==i)
            {
                pList.at(k)->connectToPoint(p1,Strong);
                p1=NULL;
            }
        }
    }
    if(p1)
    {
        pPoint* p =new pPoint(p1->dbX(),0);
        p->connectToPoint(p1,Weak);
        this->addPoint(p);
    }
}

void plane::addCone(double x, double y, double ang, bool positive)
{
    QList<pPoint*> tmpList;
    pPoint* p1;
    double r1,r2,tang;
    bool pl;
    tang=fabs(tan(ang));
    for(int i=0;i<pList.count();i++)
    {
        p1=pList.at(i);
        r1=(p1->dbY()-y)/(p1->dbX()-x);
        r2=fabs((fabs(r1)-tang));
        if(p1->dbX()==x && p1->dbY()==y) r2=0;
        if(positive)
        {
            pl=((p1->dbX()>=x && y==0) ||(p1->dbX()>=x && y!=0))  ;
        }
        else pl=((p1->dbX()<=x && y==0) ||(p1->dbX()<=x && y!=0));
        //pl=true;
        if(r2<0.0000001 && pl && p1->dbY()>=y)
        {
            if(tmpList.isEmpty()) tmpList.append(p1);
            else
            {
                bool isSet=false;
                for(int k=0;k<tmpList.count();k++)
                {
                    if(!isSet && tmpList.at(k)->dbX()>p1->dbX())
                    {
                        tmpList.insert(k,p1);
                        isSet=true;
                    }
                }
                if(!isSet) tmpList.append(p1);
            }
        }
    }
    if(tmpList.count()==2)
        tmpList.at(0)->connectToPoint(tmpList.at(1),Strong);
    else
    {
        for(int i=0;i<tmpList.count()-1;i++)
        {
            double dx=(tmpList.at(i)->dbX()-tmpList.at(i+1)->dbX());
            double dy=(tmpList.at(i)->dbY()-tmpList.at(i+1)->dbY());
            double res=pow(dx,2)+pow(dy,2);
            if(pow(res,0.5)<10)
                tmpList.at(i)->connectToPoint(tmpList.at(i+1),Strong);
        }
    }
}

void plane::addTor(double x, double y, double r)
{
    pPoint* p1=NULL;
    pPoint* p2;
    for(int i=0;i<pList.count();i++)
    {
        if((fabs(x-pList.at(i)->dbX())==r && fabs(y)==fabs(pList.at(i)->dbY()))
                || (fabs(y-pList.at(i)->dbY())==r && fabs(x)==fabs(pList.at(i)->dbX())))
        {
            if(p1==NULL) p1=pList.at(i);
            else p2=pList.at(i);
        }
    }
    p1->connectToPoint(p2,Strong);
    p1->setRounded(r);
    p2->setRounded(r);
}

void plane::enrich()
{
    double mnX=minX();
    double mxX=maxX();
    double y=0;
    pPoint* cur;
    pPoint* pre=NULL;

    for(int i=0;i<pList.count();i++)
    {
        if(pList.at(i)->dbX()==mnX && pList.at(i)->dbY()>y)
        {
            cur=pList.at(i);
            y=pList.at(i)->dbY();
        }
    }
    bool isFirst=true;
    bool debuger=false;
    first=cur;
    while(cur!=first || isFirst)
    {
        if(debuger) qDebug()<<"x:"<<cur->X()<<"y:"<<cur->Y();
        usedList.append(cur);
        if(cur->getConnection(Strong)!=NULL &&
                ((isFirst&&cur->getConnection(Strong)->dbX()!=cur->dbX())
                   || (!isFirst&& cur->getConnection(Strong)!=pre)))
        {
            cur->setNext(cur->getConnection(Strong));
            pre=cur;
            cur=cur->getConnection(Strong);
            cur->setPre(pre);
        }
        else if(cur->getConnection(Strong2)!=NULL &&
                ((isFirst && cur->getConnection(Strong)->dbX()!=cur->dbX()) || !isFirst))
        {
            cur->setNext(cur->getConnection(Strong2));
            pre=cur;
            cur=cur->getConnection(Strong2);
            cur->setPre(pre);
        }
        else if(cur->getConnection(Weak)!=NULL &&
                ((isFirst&& cur->getConnection(Weak)->dbX()!=cur->dbX())
                    || (!isFirst&& cur->getConnection(Weak)!=pre)))
        {
            cur->setNext(cur->getConnection(Weak));
            pre=cur;
            cur=cur->getConnection(Weak);
            cur->setPre(pre);
        }
        else
        {
            pPoint* nx;
            double dX=mxX-mnX+1;
            for(int i=0;i<pList.count();i++)
            {
                if(pList.at(i)->dbY()==cur->dbY()
                        && pList.at(i)->dbX()-cur->dbX()!=0
                        && fabs(pList.at(i)->dbX()-cur->dbX())<dX
                        && ((!isFirst && pList.at(i)!=pre)||isFirst)
                        && (usedList.indexOf(pList.at(i))==-1 || pList.at(i)==first))
                {
                    nx=pList.at(i);
                    dX=fabs(pList.at(i)->dbX()-cur->dbX());
                }
            }
            cur->setNext(nx);
            pre=cur;
            cur=nx;
            cur->setPre(pre);
        }
        if(isFirst) isFirst=false;
        if(pre->dbX()==cur->dbX()&& cur->dbX()==maxX()) botPoint=cur;
    }
    botFlag=(botPoint->getNext()->getNext()!=first || botPoint->dbY()!=0);
    makeSurfaces();
    simplifySurface();
    sortSurface();
}

void plane::makeSurfaces()
{
    bool isLast=false;
    pPoint* cur=first;
    while(!isLast)
    {
        if(cur->dbX()!=cur->getNext()->dbX())
        {
            surface* s;
            if(cur->dbY()==cur->getNext()->dbY())
                s=new surface(cur,cur->getNext());
            else
            {
                if(cur->dbY()>cur->getNext()->dbY())
                    s=new surface(cur->dbX(),cur->dbY(),cur->getNext()->dbX(),cur->dbY());
                else
                    s=new surface(cur->dbX(),cur->getNext()->dbY(),cur->getNext()->dbX(),cur->getNext()->dbY());
            }
            tList.append(s);
        }
        cur=cur->getNext();
        if(cur==this->botPoint) isLast=true;
    }
    tsList.append(tList.at(0));

    if(hasBot())
    {
        cur=first->getPre();
        isLast=false;
        while(!isLast)
        {
            if(cur->getPre()->dbX()!=cur->dbX())
            {
                surface* s;
                if(cur->dbY()==cur->getPre()->dbY())
                    s=new surface(cur,cur->getPre());
                else
                {
                    if(cur->dbY()>cur->getPre()->dbY())
                        s=new surface(cur->dbX(),cur->getPre()->dbY(),cur->getPre()->dbX(),cur->getPre()->dbY());
                    else
                        s=new surface(cur->dbX(),cur->dbY(),cur->getPre()->dbX(),cur->dbY());

                }
                bList.append(s);
            }
            if(cur==this->botPoint) isLast=true;
            cur=cur->getPre();
        }
    }
}

void plane::simplifySurface()
{
    for(int i=0;i<tList.count();i++)
        for(int j=i+1;j<tList.count();j++)
        {
            if(tList.at(i)->getP1()->dbX()==tList.at(j)->getP1()->dbX())
                for(int k=i;k<j;k++)
                    tList.removeAt(i);
            if(tList.at(i)->getP2()->dbX()==tList.at(j)->getP2()->dbX())
                for(int k=i;k<j;k++)
                    tList.removeAt(i+1);

        }
    bool hor;
    int end;
    int endX;
    for(int i=0;i<tList.count();i++)
    {
        hor=false;
        end=i;
        endX=tList.at(i)->getP2()->dbX();
        for(int j=i+1;j<tList.count();j++)
        {
            if(tList.at(i)->getMid()==tList.at(j)->getMid() && !hor)
            {
                end=j;
                endX=tList.at(j)->getP2()->dbX();
            }

            if(!hor && tList.at(i)->getMid()<tList.at(j)->getMid())
            {
                end=j-1;
                hor=true;
            }
        }
        for(int k=i+1;k<end+1;k++)
                tList.removeAt(i+1);
        if(hor)
            tList.at(i)->getP2()->setX(tList.at(i+1)->getP1()->dbX());
        else tList.at(i)->getP2()->setX(endX);
    }

    for(int i=0;i<bList.count();i++)
        for(int j=i+1;j<bList.count();j++)
        {
            if(bList.at(i)->getP1()->dbX()==bList.at(j)->getP1()->dbX())
                for(int k=i;k<j;k++)
                    bList.removeAt(i);
            if(bList.at(i)->getP2()->dbX()==bList.at(j)->getP2()->dbX())
                for(int k=i;k<j;k++)
                    bList.removeAt(i+1);

        }
    for(int i=0;i<bList.count();i++)
    {
        hor=false;
        end=i;
        endX=bList.at(i)->getP2()->dbX();
        if(bList.at(i)->getMid()==0) bSurface=bList.at(i);
        for(int j=i+1;j<bList.count();j++)
        {
            if(bList.at(i)->getMid()==bList.at(j)->getMid() && !hor)
            {
                end=j;
                endX=bList.at(j)->getP2()->dbX();
            }

            if(!hor && bList.at(i)->getMid()>bList.at(j)->getMid())
            {
                end=j-1;
                hor=true;
            }
        }
        for(int k=i+1;k<end+1;k++)
                bList.removeAt(i+1);
        if(hor)
            bList.at(i)->getP2()->setX(bList.at(i+1)->getP1()->dbX());
        else bList.at(i)->getP2()->setX(endX);
    }


}

void plane::sortSurface()
{
    tsList.clear();
    bsList.clear();
    bool isSet;
    tsList.append(tList.at(0));
    for(int i=1;i<tList.count();i++)
    {
        isSet=false;
        for(int j=0;j<tsList.count();j++)
        {
            if(!isSet)
            {
                if(tList.at(i)->getMid()<tsList.at(j)->getMid())
                {
                    tsList.insert(j,tList.at(i));
                    isSet=true;
                }
            }
        }
        if(!isSet) tsList.append(tList.at(i));
    }
    if(hasBot())
    {
        bsList.append(bList.at(0));
        for(int i=1;i<bList.count();i++)
        {
            isSet=false;
            for(int j=0;j<bsList.count();j++)
            {
                if(!isSet)
                {
                    if(bList.at(i)->getMid()<bsList.at(j)->getMid())
                    {
                        bsList.insert(j,bList.at(i));
                        isSet=true;
                    }
                }
            }
            if(!isSet) bsList.append(bList.at(i));
        }
    }



}

int plane::maxX()
{
    int max=pList.at(0)->X();
    for(int i=0;i<pList.count();i++)
    {
        if(pList.at(i)->X()>max) max=pList.at(i)->X();
    }
    return max;
}

int plane::minX()
{
    int min=pList.at(0)->X();
    for(int i=0;i<pList.count();i++)
    {
        if(pList.at(i)->X()<min) min=pList.at(i)->X();
    }
    return min;

}

int plane::maxY()
{
    int max=pList.at(0)->Y();
    for(int i=0;i<pList.count();i++)
    {
        if(pList.at(i)->Y()>max) max=pList.at(i)->Y();
    }
    return max;
}

void plane::moveToZero()
{
    double mnX=minX();
    for(int i=0;i<pList.count();i++)
        pList.at(i)->setX(pList.at(i)->dbX()-mnX);
    for(int i=0;i<tList.count();i++)
    {
        tList.at(i)->move(-mnX);
    }
    for(int i=0;i<bList.count();i++)
    {
        bList.at(i)->move(-mnX);
    }
}

void plane::reverse()
{
    for(int i=0;i<pList.count();i++)
        pList.at(i)->reverse();
    for(int i=0;i<tList.count();i++)
    {
        tList.at(i)->flip();
    }
    for(int i=0;i<bList.count();i++)
    {
        bList.at(i)->flip();
    }
    moveToZero();
}

int plane::totalPr()
{
    int n=0;
    for(int i=0;i<TopList()->count();i++)
        n+=TopList()->at(i)->nPr();
    for(int i=0;i<BotList()->count();i++)
        n+=BotList()->at(i)->nPr();
    return n;
}

int plane::botPr()
{
    int n=0;
    for(int i=0;i<BotList()->count();i++)
        n+=BotList()->at(i)->nPr();
    return n;
}

