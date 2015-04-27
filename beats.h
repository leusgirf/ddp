#ifndef BEATS_H
#define BEATS_H

#include <QtWidgets>

enum beatType {blank, baseSur, before, after, vnp, tvr};

class beatPoint
{
public:
    beatPoint(bool top, int side, int pr, QString name);
    beatPoint(){;}
    bool top;
    int side;
    int pr;
    QString name;
};

class beat
{
public:
    beat(bool top, int side, int pr, QString name, beatType tp);
    beat(beatPoint bp, beatType tp);
    beatType type;

    void setSecondSide(bool top, int side, int pr, QString name);
    void setSecondSide(beatPoint *bp);
    bool twoSides;
    bool top1,top2;
    int side1, side2;
    int pr1,pr2;
    QString name1, name2;

    double res;
};


class operation
{
public:
    operation(){;}

    bool isActive(){return active;}
    void setActive(bool a){active=a;}

    int count(){return bList.count();}
    QList<beat*> bList;
private:
    bool active;
};


class beatParam
{
    beatParam(){;}
};

#endif // BEATS_H
