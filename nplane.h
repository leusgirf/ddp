#ifndef NPLANE_H
#define NPLANE_H

#include "planeobjects.h"
#include <algorithm>


class nAxis: public QGraphicsItem
{
public:
    nAxis(int x,int y):_x(x),_y(y){setZValue(-5);}

    QRectF boundingRect()const { return QRectF(0,0,_x,_y);}


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int _x;
    int _y;

};

class nAllowance;

class nSurface: public QGraphicsObject
{
    Q_OBJECT
public:
    nSurface(QGraphicsItem* parent=0)
        :QGraphicsObject(parent),w(1),h(1),num(0){}

    nSurface(nSurface*);
    nSurface(float width, float height, QGraphicsItem* parent=0)
        :QGraphicsObject(parent),w(width),h(height),num(0){}

    nSurface(float width, float height, nSurface* ns=0);
    nSurface(surface* surf, QGraphicsItem* parent=0);
    ~nSurface();


    QRectF boundingRect() const;

    static bool compareX (const nSurface *left, const nSurface *right){
        return left->midPoint().rx()<right->midPoint().rx();
    }

    static bool compareY(const nSurface *left, const nSurface *right){
        return left->midPoint().ry()<right->midPoint().ry();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void setSelectable(bool flag){selectable=flag;}
    bool isSelectable(){return selectable;}
    void setAllowanceSelectable(bool flag);
    void setHighlighted(const bool &hl);
    void setNum(const int &number){num=number;update();}
    QString name();

    void setWidth(float width);
    void setHeight(float height);

    QPointF getP1() const;
    void setP1(const QPointF &value);

    QPointF getP2() const;
    void setP2(const QPointF &value);


    QPointF midPoint() const;
    int getInc(){return aList.count();}

    bool isBot() const;
    void setBot(const bool &value);

    void moveAllowances();

public slots:
    virtual void addAllowance();
    void addAllowance(nAllowance* allowance);
    virtual void removeAllowance();
    void removeAllowance(nAllowance* allowance);
protected:

    QPixmap slashPixmap(bool fdiag, const QPen &pen, int hx) const;

    float w,h;
    QPointF p1,p2;
    bool bot;
    bool highlighted;
    bool selectable;
    int num;
    QList<nAllowance*> aList;

private:
signals:
    void changed();
    void selected(QGraphicsObject*);
    void allowancesChanged();
};

class nAllowance : public nSurface
{
    Q_OBJECT
public:
    nAllowance(nSurface* surface):nSurface(surface),pSurf(surface){}
    nAllowance(float width, float height, nSurface *surface=0);

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    nSurface *pSurf;

signals:
    void removeMe(nAllowance*);
    void addAllowanceMe(nAllowance*);

public slots:
    void addAllowance(){qDebug()<<"na clicked";emit addAllowanceMe(this);}
    void removeAllowance(){qDebug()<<"na clicked";emit removeMe(this);}

};


class nPlane: public QGraphicsObject
{
    Q_OBJECT
public:
    nPlane(QGraphicsItem* parent=0);
    nPlane(float width, float height, QGraphicsItem* parent=0);
    nPlane(plane* pl, QGraphicsItem* parent=0);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setWidth(float width){
        prepareGeometryChange(); w=width;}
    void setHeight(float height){
        prepareGeometryChange(); h=height;}

    void addTopSurface(nSurface* surf);
    void addBotSurface(nSurface* surf);
    void addSurfaces(QList<nSurface*> &bot, QList<nSurface*> &top);

    void setSurfacesSelectable(bool flag);
    void setAllowancesSelectable(bool flag);

    void sortSurfaces();
    int calcSurfaceY(nSurface* surf);
    int calcItemsHeight();

private:
    float w,h;
    QList<nSurface*> botList;
    QList<nSurface*> topList;
    QList<nSurface*> botXList;
    QList<nSurface*> topXList;
    QList<nSurface*> botYList;
    QList<nSurface*> topYList;
public slots:
    void needUpdate();
    void testSlot(QGraphicsObject*);
    void moveSurfaces();

signals:
    void objectSelected(QGraphicsObject*);
};

class nSimpleViewer: public QWidget
{
    Q_OBJECT
public:
    nSimpleViewer(QWidget* parent=0);

    void setMainPlane(nPlane* pl);

    enum selectionMode {noSelection, Surfaces, Allowances, AllSurfaces};
    void setSelectionMode(selectionMode mode);

private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    nPlane* mPlane;
    nAxis* axis;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void testSlot(QGraphicsObject*);
signals:
    void objectSelected(QGraphicsObject* object);

};

#endif // NPLANE_H
