#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QStringList>
#include "objects.h"

class parser : public QObject
{
    Q_OBJECT
public:
    explicit parser(QStringList sl,QObject *parent = 0);

    QList<plane*> getPlanesList(){return pList;}

private:
    QList<obj*> objList;
    QList<circle*> circleList;
    QList<axis*> aList;
    QList<plane*> pList;

    obj* findObj(int index);
    double argToDouble(QString t);


    QStringList disassemble(QString in);
    void getCrtnPntList(QStringList in);
    void getdirectionsList(QStringList in);
    void getAxis2PlacementList(QStringList in);
    void getcircleList(QStringList in);
    void getAugments(QStringList in);
    void makeAxis();
    void makePlanes();





signals:

public slots:

};
#endif // PARSER_H
