#include "beats.h"

beat::beat(bool top, int side, int pr, QString name, beatType tp)
{
    twoSides=false;
    top1=top;
    side1=side;
    pr1=pr;
    name1=name;
    type=tp;
}

beat::beat(beatPoint bp, beatType tp)
{
    twoSides=false;
    top1=bp.top;
    side1=bp.side;
    pr1=bp.pr;
    name1=bp.name;
    type=tp;
}

void beat::setSecondSide(bool top, int side, int pr, QString name)
{
    twoSides=true;
    top2=top;
    side2=side;
    pr2=pr;
    name2=name;
}

void beat::setSecondSide(beatPoint* bp)
{
    twoSides=true;
    top2=bp->top;
    side2=bp->side;
    pr2=bp->pr;
    name2=bp->name;
}




beatPoint::beatPoint(bool top, int side, int pr, QString name)
{
    this->top=top;
    this->side=side;
    this->pr=pr;
    this->name=name;
}
