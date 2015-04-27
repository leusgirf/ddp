#include "drawcore.h"

drawCore::drawCore(QList<circle *> cList, QList<rObj *> rList, tScetch* tsc, QWidget *parent):QWidget(parent)
{
    drawCore::cList=cList;
    drawCore::tsc = tsc;
    drawCore::rList=rList;
    for(int i=0;i<rList.count();i++)
    {
        rList.at(i)->enrich(cList);
    }
    generateAxis();
    paintAxis();
    //connect(fr, SIGNAL(resendSideSet(int)),SLOT(sideSet(int)));
    //connect(fr,SIGNAL(resentSideUnset(int)),SLOT(sideUnset(int)));
}

void drawCore::generateAxis()
{
    while(rList.count()>0)
    {
        bool cmp=false;
        int nAx=0;
        for(int i=0;i<aList.count();i++)
        {
            cmp=aList.at(i)->compare(rList.at(0));
            if(cmp) nAx=i;
        }
        if (cmp) aList.at(nAx)->addRObj(rList.takeFirst());
        else
        {
            axis* ax= new axis(rList.takeFirst());
            aList.append(ax);
        }
    }
}

void drawCore::paintAxis()
{
    makePolygonList(aList.at(0));
    tsc->pList=pList;
    tsc->update();


}

void drawCore::makePolygonList(axis *a)
{
    int b1,b2,t1,t2;
    polygon* p;
    for(int i=0;i<a->rList.count();i++)
    {
        b1=a->rList.at(i)->side1->cptn->getCoard(a->type);
        t1=(int)a->rList.at(i)->size1;
        b2=a->rList.at(i)->side2->cptn->getCoard(a->type);
        t2=(int)a->rList.at(i)->size2;
        p=new polygon(b1,b2,t1,t2);
        pList.append(p);

    }
}

void drawCore::sideSet(int a)
{
    ssc->sortYList.at(a)->nOp++;
    for(int i=ssc->sortYList.count()-1;i>a;i--)
    {
        ssc->sortYList.at(i)->inc++;
    }
    ssc->update();
}

void drawCore::sideUnset(int a)
{
    ssc->sortYList.at(a)->nOp--;
    for(int i=ssc->sortYList.count()-1;i>a;i--)
    {
        ssc->sortYList.at(i)->inc--;
    }
    ssc->update();
}
