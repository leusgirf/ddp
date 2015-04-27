#ifndef OBJECTS_H
#define OBJECTS_H

#include <planeobjects.h>


enum AugType {augCone, augPlane, augTor};

class obj
{
public:
    obj(int index);
    obj();
    virtual ~obj(){;}

    virtual int index(){return ind;}
    virtual void setIndex(int index){ind=index;}
private:
    int ind;
};


class cPoint: virtual public obj
{
public:

    cPoint(int index, double x,double y, double z);
    cPoint(cPoint* c);
    cPoint();
    ~cPoint(){;}

    double X(){return x;}
    double Y(){return y;}
    double Z(){return z;}

    void setX(double x){this->x=x;}
    void setY(double y){this->y=y;}
    void setZ(double z){this->z=z;}

    double getDif(cPoint* c);
    bool has2Same(cPoint* c);

private:
    double x,y,z;
};

class direction: virtual public obj
{
public:

    direction(int index, double x,double y, double z);
    direction(direction* c);
    direction();
    ~direction(){;}

    double vX(){return x;}
    double vY(){return y;}
    double vZ(){return z;}

    void setvX(double x){this->x=x;}
    void setvY(double y){this->y=y;}
    void setvZ(double z){this->z=z;}

    bool hasSameDirection(direction* d);
    bool hasSameOrient(direction* d);
    bool isDirPositive();

private:
    double x,y,z;

};

class axis2Plct: public cPoint, public direction
{

public:
    axis2Plct(int index, cPoint* point, direction* d);
    axis2Plct(axis2Plct* a);
    axis2Plct();
    ~axis2Plct(){;}

    bool hasSameAxis(axis2Plct *a);
};

class circle: public axis2Plct
{
public:
    circle(int index, axis2Plct* a, double r);
    circle(circle* c);
    ~circle(){;}

    double R(){return r;}
    void setR(double r){this->r=r;}
private:
    double r;
};

class augment: public axis2Plct
{
public:
    augment(axis2Plct* a, AugType type, double Y=0, double arg=0, bool positive=true);
    ~augment(){;}
    AugType getType(){return type;}
    double getX(){return arg1;}
    double getY(){return arg2;}
    double getArg(){return arg3;}
    void setX(double a){arg1=a;}
    void setY(double a){arg2=a;}
    void setPositive(bool p){flag=p;}
    bool isEquel(augment* a);
    bool isPositive(){return flag;}

private:
    AugType type;
    double arg1,arg2,arg3;
    bool flag;
};

class axis: public axis2Plct
{
public:

    axis(circle* a);
    ~axis();

    bool addCircle(circle* c);
    void addAugment(augment* a);
    pPoint* makePoint(int a);
    plane* makePlane();
    int count() {return cList.count();}
private:
    QList<circle *> cList;
    QList<augment*> aList;
};




#endif // OBJECTS_H
