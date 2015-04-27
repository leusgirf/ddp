#include "parserS.h"

parser::parser(QStringList sl, QObject *parent) :
    QObject(parent)
{
    for(int i=0;i<sl.count();i++)
    {
        getCrtnPntList(disassemble(sl.at(i)));
    }
    for(int i=0;i<sl.count();i++)
    {
        getdirectionsList(disassemble(sl.at(i)));
    }
    for(int i=0;i<sl.count();i++)
    {
        getAxis2PlacementList(disassemble(sl.at(i)));
    }
    for(int i=0;i<sl.count();i++)
    {
        getcircleList(disassemble(sl.at(i)));
    }
    if(!circleList.isEmpty())
    {
        makeAxis();
        for(int i=0;i<sl.count();i++)
        {
            getAugments(disassemble(sl.at(i)));
        }
        makePlanes();
    }
}


double parser::argToDouble(QString t)
{
    QString c="E";
    for(int i=0; i<t.size();i++)
    {
        if(t.at(i)==c) return 0;
    }
    return t.toDouble();

}

void parser::getCrtnPntList(QStringList in)
{
    if(in.at(1)=="CARTESIAN_POINT")
    {
        int a,b;
        a=in.at(2).indexOf("(");
        b=in.at(2).lastIndexOf(")");
        QString args=in.at(2).mid(a+1,b-a-1);
        a=args.indexOf(",");
        b=args.lastIndexOf(",");
        double arg1,arg2,arg3;
        arg1=argToDouble(args.mid(0,a));
        arg2=argToDouble(args.mid(a+1,b-a-1));
        arg3=argToDouble(args.mid(b+1,args.size()-b));
        cPoint* c=new cPoint(in.at(0).toInt(),arg1,arg2,arg3);
        objList.append(c);

    }
}

void parser::getdirectionsList(QStringList in)
{
    if(in.at(1)=="DIRECTION")
    {
        int a,b;
        a=in.at(2).indexOf("(");
        b=in.at(2).lastIndexOf(")");
        QString args=in.at(2).mid(a+1,b-a-1);
        a=args.indexOf(",");
        b=args.lastIndexOf(",");
        double arg1,arg2,arg3;
        arg1=argToDouble(args.mid(0,a));
        arg2=argToDouble(args.mid(a+1,b-a-1));
        arg3=argToDouble(args.mid(b+1,args.size()-b));
        direction* d=new direction(in.at(0).toInt(),arg1,arg2,arg3);
        objList.append(d);

    }
}

void parser::getAxis2PlacementList(QStringList in)
{
    if(in.at(1)=="AXIS2_PLACEMENT_3D")
    {
        int a,b;
        int arg1,arg2;
        a=in.at(2).indexOf(",");
        b=in.at(2).indexOf(",",a+1);
        arg1=in.at(2).mid(a+2,b-2-a).toInt();
        a=b;
        b=in.at(2).indexOf(",",a+1);
        arg2=in.at(2).mid(a+2,b-2-a).toInt();
        if(arg1!=0 && arg2!=0)
        {
            cPoint* c=dynamic_cast<cPoint*>(findObj(arg1));
            direction* d=dynamic_cast<direction*>(findObj(arg2));
            axis2Plct* ax = new axis2Plct(in.at(0).toInt(),c,d);
            objList.append(ax);
        }
    }
}

void parser::getcircleList(QStringList in)
{
    if(in.at(1)=="CIRCLE")
    {

        int a,b;
        double arg1,arg2;
        a=in.at(2).indexOf(",");
        b=in.at(2).indexOf(",",a+1);
        arg1=in.at(2).mid(a+2,b-2-a).toDouble();
        arg2=in.at(2).mid(b+1).toDouble();
        axis2Plct* ax = dynamic_cast<axis2Plct*>(findObj(arg1));
        circle* c= new circle(in.at(0).toInt(),ax,arg2);
        circleList.append(c);
        objList.append(c);
    }

}

void parser::getAugments(QStringList in)
{
    augment* aug=NULL;
    axis2Plct* ax=NULL;
    if(in.at(1)=="CONICAL_SURFACE")
    {
        int a,b;
        int arg1;
        double arg2,arg3;
        a=in.at(2).indexOf(",");
        b=in.at(2).indexOf(",",a+1);
        arg1=in.at(2).mid(a+2,b-2-a).toInt();
        a=b;
        b=in.at(2).indexOf(",",a+1);
        arg2=in.at(2).mid(a+1,b-a-1).toDouble();
        arg3=in.at(2).mid(b+2).toDouble();

        ax = dynamic_cast<axis2Plct*>(findObj(arg1));
        aug= new augment(ax,augCone,arg2,arg3);
    }
    if(in.at(1)=="PLANE")
    {
        int a,b;
        int arg1;
        a=in.at(2).indexOf(",");
        b=in.at(2).indexOf(")",a+1);
        arg1=in.at(2).mid(a+2,b-2-a).toInt();
        ax = dynamic_cast<axis2Plct*>(findObj(arg1));
        aug= new augment(ax,augPlane);
    }
    if(in.at(1)=="TOROIDAL_SURFACE")
    {
        int a,b;
        int arg1;
        double arg2,arg3;
        a=in.at(2).indexOf(",");
        b=in.at(2).indexOf(",",a+1);
        arg1=in.at(2).mid(a+2,b-2-a).toInt();
        a=b;
        b=in.at(2).indexOf(",",a+1);
        arg2=in.at(2).mid(a+1,b-2-a).toDouble();
        arg3=in.at(2).mid(b+1).toDouble();

        ax = dynamic_cast<axis2Plct*>(findObj(arg1));
        aug= new augment(ax,augTor,arg2,arg3);
    }
    if(aug!=NULL)
    {
        objList.append(aug);
        for(int i=0;i<aList.count();i++)
        {
            aList.at(i)->addAugment(aug);
        }
    }
}

void parser::makeAxis()
{
    bool isSet;
    axis* ax=new axis(circleList.at(0));
    aList.append(ax);
    for(int i=1;i<circleList.count();i++)
    {
        isSet=false;
        for(int k=0;k<aList.count();k++)
        {
            if(!isSet)
                isSet=aList.at(k)->addCircle(circleList.at(i));
        }
        if(!isSet)
        {
            ax=new axis(circleList.at(i));
            aList.append(ax);
        }
    }
    for(int i=0;i<aList.count();i++)
    {
        if(aList.at(i)->count()<2)
        {
            delete aList.takeAt(i);
            i--;
        }
    }
}

void parser::makePlanes()
{
    for(int i=0;i<aList.count();i++)
    {
        pList.append(aList.at(i)->makePlane());
    }
}

QStringList parser::disassemble(QString in)
{
    QStringList out;
    int a;
    int b= in.indexOf("=");
    QString str=in.mid(1,b-1);
    out.append(str);
    a =in.indexOf("=");
    b= in.indexOf("(");
    str=in.mid(a+1,b-a-1);
    out.append(str);
    a=b;
    b=in.lastIndexOf(")");
    str=in.mid(a+1,b-a-1);
    out.append(str);
    return out;

}

obj* parser::findObj(int index)
{
    for(int i=0;i<objList.count();i++)
    {
        if(objList.at(i)->index()==index) return objList.at(i);
    }
    return NULL;
}



