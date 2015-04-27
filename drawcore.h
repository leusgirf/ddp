#ifndef DRAWCORE_H
#define DRAWCORE_H
#include <QObject>
#include <QtWidgets>
#include <sscetch.h>
#include <robj.h>
#include <oplframe.h>


class drawCore: public QWidget
{
    Q_OBJECT

public:
    drawCore(QList<circle*> cList, QList<rObj*> rList, tScetch* tsc,QWidget* parent=0);
private:
    QList<circle*> cList;
    QList<rObj*> rList;
    QList<axis*> aList;

    QList<polygon*> pList;
    tScetch* tsc;
    sScetch* ssc;

    void generateAxis();
    void paintAxis();
    void makePolygonList(axis* a);
public slots:
    void sideSet(int a);
    void sideUnset(int a);

};

#endif // DRAWCORE_H
