#include "objects.h"


obj::obj(int index)
{
    this->ind=index;
}

obj::obj()
{
}


cPoint::cPoint(int index, double x,double y, double z):obj(index)
{
    this->x=x;
    this->y=y;
    this->z=z;
}

cPoint::cPoint(cPoint *c):obj()
{
    this->x=c->X();
    this->y=c->Y();
    this->z=c->Z();
}

cPoint::cPoint():obj()
{

}

double cPoint::getDif(cPoint *c)
{
    double r,r1,r2,r3;
    r1=pow((this->X()-c->X()),2);
    r2=pow((this->Y()-c->Y()),2);
    r3=pow((this->Z()-c->Z()),2);
    r=pow((r1+r2+r3),0.5);
    if(fabs(r)<0.0000001) return r=0;
    if((this->X()>c->X()&& this->Y()>=c->Y()) ||
            (this->Y()>c->Y() && this->Z()>=c->Z()) ||
             (this->X()>c->X() && this->Z()>=c->Z()))
        return -r;
    if(this->X()>=c->X() && this->Y()>=c->Y() && this->Z()>=c->Z()) return -r;
    return r;
}

bool cPoint::has2Same(cPoint *c)
{
    if((x==c->X() && y==c->Y()) || (x==c->X() && z==c->Z()) || (y==c->Y() && z==c->Z())) return true;
    return false;
}

/////
///
///
direction::direction(int index, double x,double y, double z):obj(index)
{
    this->x=x;
    this->y=y;
    this->z=z;
}

direction::direction(direction *c):obj()
{
    this->x=c->vX();
    this->y=c->vY();
    this->z=c->vZ();
}

direction::direction():obj()
{
}

bool direction::hasSameDirection(direction* d)
{
    if(fabs(this->vX())==fabs(d->vX()) && fabs(this->vY())==fabs(d->vY())
            && fabs(this->vZ())==fabs(d->vZ()))
        return true;
    return false;
}

bool direction::hasSameOrient(direction* d)
{
    if(((this->vX()==d->vX())&&(this->vY()==d->vY())
            && (this->vZ()==d->vZ())))
        return true;
    return false;
}

bool direction::isDirPositive()
{
    if(x>=0 && y>=0 && z>=0) return true;
    return false;

}

////
////
///

axis2Plct::axis2Plct(int index, cPoint *point, direction *d):cPoint(point),direction(d)
{
    setIndex(index);
}

axis2Plct::axis2Plct(axis2Plct* a):cPoint(),direction()
{
    setIndex(a->index());
    this->setX(a->X());
    this->setY(a->Y());
    this->setZ(a->Z());
    this->setvX(a->vX());
    this->setvY(a->vY());
    this->setvZ(a->vZ());
}

axis2Plct::axis2Plct():cPoint(),direction()
{
}

bool axis2Plct::hasSameAxis(axis2Plct *a)
{
    if(this->hasSameDirection(a))
    {
        double r=fabs(this->getDif(a));
        if(r==0) return true;
        if(fabs(fabs(this->X()-a->X())/r-fabs(this->vX())<0.000001) &&
                fabs(fabs(this->Y()-a->Y())/r-fabs(this->vY())<0.000001) &&
                    fabs(fabs((this->Z()-a->Z()))/r-fabs(this->vZ()))<0.000001) return true;
        else
            return false;

    }
    return false;
}

///
///

circle::circle(int index, axis2Plct *a, double r):axis2Plct(a)
{
    setIndex(index);
    setR(r);
}

circle::circle(circle *c): axis2Plct()
{
    this->setR(c->R());
}

///
///



axis::axis(circle *a):axis2Plct(a)
{
    cList.append(a);
}

bool axis::addCircle(circle *c)
{
    if(this->hasSameAxis(c))
    {
        cList.append(c);
        return true;
    }
    return false;
}

pPoint* axis::makePoint(int a)
{
    pPoint* p=new pPoint(getDif(cList.at(a)),cList.at(a)->R());
    return p;
}

plane* axis::makePlane()
{
    plane* pl= new plane();
    for(int i=0;i<cList.count();i++)
    {
        pl->addPoint(makePoint(i));
    }
    for(int i=0; i<aList.count();i++)
    {
        switch (aList.at(i)->getType())
        {
        case augCone:
            pl->addCone(aList.at(i)->getX(),aList.at(i)->getY(),
                        aList.at(i)->getArg(),aList.at(i)->isPositive());
            break;
        case augTor:
            pl->addTor(aList.at(i)->getX(),aList.at(i)->getY(),aList.at(i)->getArg());
            break;
        case augPlane:
            pl->addPlane(int(aList.at(i)->getX()),aList.at(i)->getY(),aList.at(i)->isPositive());
            break;

        }
    }
    pl->enrich();
    pl->moveToZero();

    return pl;
}

void axis::addAugment(augment *a)
{
    bool seted=false;
    switch (a->getType())
    {
    case augPlane:
        if(a->hasSameAxis(this))
        {
            a->setX(this->getDif(a));
            a->setPositive(false);
            seted=true;
        }
        else
        {
            for(int i=0;i<cList.count();i++)
            {
                if(!seted)
                {
                    if(a->has2Same(cList.at(i)))
                    {
                        a->setX(this->getDif(cList.at(i)));
                        seted=true;
                    }
                }
            }
        }
        break;
    case augCone:
        if(this->hasSameAxis(a))
        {
            a->setX(this->getDif(a));
            a->setPositive(a->isDirPositive());
            seted=true;
        }
        break;
    case augTor:
        if(this->hasSameAxis(a))
        {
            a->setX(this->getDif(a));
            seted=true;
        }
        break;
    }
    bool ex=false;
    for(int i=0;i<aList.count();i++)
    {
        if(aList.at(i)->isEquel(a)) ex=true;
    }
    if(!ex && seted) aList.append(a);
}

axis::~axis()
{
}

augment::augment(axis2Plct* a, AugType type, double Y, double arg, bool positive):axis2Plct(a)
{
    this->type=type;
    this->arg2=Y;
    this->arg3=arg;
    this->flag=positive;
}

bool augment::isEquel(augment *a)
{
    if(this->hasSameDirection(a) &&
            this->getDif(a)==0 &&
            this->type==a->type &&
            this->Y()==a->Y() &&
            this->getArg()==a->getArg() &&
            this->flag==a->isPositive() )
        return true;
    return false;
}


