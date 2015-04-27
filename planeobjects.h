#ifndef PLANEOBJECTS_H
#define PLANEOBJECTS_H

#include <QList>
#include <QString>
#include <QDebug>
#include <cmath>
#include <beats.h>


enum connectionType {Strong,Strong1,Strong2,Weak};

class pPoint
{
public:
    pPoint(){;}
    pPoint(double x, double y);


    void setX(double x){this->x=x;}
    void setY(double y){this->y=y;}
    int X(){return (int)x;}
    int Y(){return (int)y;}
    double dbX(){return x;}
    double dbY(){return y;}

    pPoint* getNext(){return next;}
    void setNext(pPoint* next){this->next=next;}

    pPoint* getPre() {return pre;}
    void setPre(pPoint* pre){this->pre=pre;}

    void reverse(){x=-x;}

    void setRounded(int r){round=r;}
    int getRounded(){return round;}
    void setConnection(pPoint* next, connectionType type);
    pPoint* getConnection(connectionType type);
    void connectToPoint(pPoint* p, connectionType type);

    bool isEquel(pPoint* c);

private:
    double x,y;
    int round;
    pPoint* strongConnection1;
    pPoint* strongConnection2;
    pPoint* weakConnection;
    pPoint* next;
    pPoint* pre;

};

class surface
{
public:
    surface(pPoint* P1, pPoint* P2);
    surface(double x1, double y1, double x2, double y2);

    pPoint* getP1(){return &p1;}
    pPoint* getP2(){return &p2;}

    void move(double dx);

    void flip();

    void addInc(int ic) {inc=inc+ic;}
    int Inc() {return inc;}

    void addPr(int ic) {nOfPr=nOfPr+ic;}
    int nPr() {return nOfPr;}

    void setMid(double m) {mid=m;}
    double getMid() {return mid;}

    void setActive(bool f){active=f;}
    bool isActive(){return active;}

    void setActivePr(int a){activePr=a;}
    int ActivePr(){return activePr;}

    void addition(surface* add);

private:
    pPoint p1,p2;
    int inc;
    int nOfPr;
    int activePr;
    double mid;
    bool active;
};


class plane
{
public:
    plane();

    int maxY();
    int minX();
    int maxX();

    pPoint* getFirst(){return first;}
    pPoint* getBotPoint(){return botPoint;}
    surface* getBotSurface(){return bSurface;}
    QList<surface*>* BotList(){return &bList;}
    QList<surface*>* TopList(){return &tList;}
    QList<surface*>* BotSortedList(){return &bsList;}
    QList<surface*>* TopSortedList(){return &tsList;}

    QList<operation*>* opList(){return &oList;}

    void addPoint(pPoint* p);
    void addPoint(double x, double y);
    void addPlane(int x, int y, bool positive);
    void addCone(double x, double y, double ang, bool positive);
    void addTor(double x,double y, double r);
    void enrich();
    void sortSurface();

    void moveToZero();
    void reverse();

    int totalPr();
    int botPr();

    bool hasBot(){return botFlag;}



private:
    QList<pPoint*> pList;
    QList<pPoint*> usedList;
    QList<surface*> bList;
    QList<surface*> tList;
    QList<surface*> bsList;
    QList<surface*> tsList;
    QList<operation*> oList;
    pPoint* first;
    pPoint* botPoint;
    surface* bSurface;
    bool botFlag;


    void makeSurfaces();
    void simplifySurface();
};



#endif // PLANEOBJECTS_H
