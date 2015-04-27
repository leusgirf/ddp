#include "nplane.h"

nSurface::nSurface(nSurface *ns)
    :QGraphicsObject(ns),w(1),h(1),num(0)
{
    p1=ns->getP1();
    p2=ns->getP2();
    bot=ns->isBot();
    selectable=ns->isSelectable();
    setAcceptHoverEvents(true);
}


nSurface::nSurface(float width, float height, nSurface *ns)
    :QGraphicsObject(ns),num(0)
{
    p1=ns->getP1();
    p2=ns->getP2();
    bot=ns->isBot();
    highlighted= this->boundingRect().contains(QCursor::pos());
    selectable=ns->isSelectable();
    setAcceptHoverEvents(true);
    w=width;
    h=height;
}

nSurface::nSurface(surface *surf, QGraphicsItem *parent)
    :QGraphicsObject(parent),num(0)
{
    p1.setX(surf->getP1()->dbX());
    p1.setY(surf->getP1()->dbX());
    p2.setX(surf->getP2()->dbX());
    p2.setY(surf->getP2()->dbX());
    bot=false;
    highlighted=false;
    selectable=false;
    setAcceptHoverEvents(true);
}

nSurface::~nSurface()
{
    while(!aList.isEmpty())
        aList.takeLast()->deleteLater();
}

QRectF nSurface::boundingRect() const
{
    return QRect(-40,0,w+40,h);
}

void nSurface::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLine line(0,h/2,w,h/2);
    painter->setRenderHint(QPainter::Antialiasing,true);
    QColor color=Qt::black;
    painter->setPen(QPen(color,5));
    if(highlighted)
        color=Qt::green;
    else
        color=Qt::blue;
    painter->setPen(QPen(color,5));
    painter->drawLine(line);
    QRectF slash;
    QRectF nameRect;
    QPoint slashPoint;
    if(bot)
    {
        slash.setCoords(0,3/8*h,w,h/2);
        slashPoint=QPoint(0,3./8.*h);
        nameRect.setCoords(0,h,w,h/2);
    }
    else
    {
        slash.setCoords(0,h/2,w,5./8.*h);
        slashPoint=QPoint(0,h/2);
        nameRect.setCoords(0,0,w,h/2);
    }
    QPixmap px=slashPixmap(false, painter->pen(),h/8);
    painter->drawPixmap(slashPoint,px,px.rect());
    QFont font=painter->font();
    font.setPixelSize(30);
    painter->setFont(font);
    painter->drawText(QPointF(-40,h/2),name()+"0");

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void nSurface::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(selectable)
    {
        highlighted=false;
        update();
        emit changed();
    }
}

void nSurface::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(selectable)
    {
        highlighted=true;
        update();
        emit changed();
    }
}

void nSurface::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    event->accept();
}

void nSurface::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if(selectable)
    {
        emit selected(this);
        Q_UNUSED(event);
        event->accept();
    }
}

void nSurface::setWidth(float width)
{
    prepareGeometryChange();
    w=width;
    for(int i=0;i<aList.count();i++)
        aList.at(i)->setWidth(width);
    moveAllowances();
}

void nSurface::setHeight(float height)
{
    prepareGeometryChange();
    h=height;
    for(int i=0;i<aList.count();i++)
        aList.at(i)->setHeight(height);
    moveAllowances();
}

QPointF nSurface::getP1() const
{
    return p1;
}

void nSurface::setP1(const QPointF &value)
{
    p1 = value;
}
QPointF nSurface::getP2() const
{
    return p2;
}

void nSurface::setP2(const QPointF &value)
{
    p2 = value;
}

QPointF nSurface::midPoint() const
{
    return QPointF((p1.x()+p2.x())/2.,(p1.y()+p2.y())/2.);
}
bool nSurface::isBot() const
{
    return bot;
}

void nSurface::setBot(const bool &value)
{
    bot = value;
}

void nSurface::addAllowance()
{
    nAllowance* na = new nAllowance(w,h,this);
    na->setSelectable(this->selectable);
    connect(na,SIGNAL(addAllowanceMe(nAllowance*)),SLOT(addAllowance(nAllowance*)));
    connect(na,SIGNAL(removeMe(nAllowance*)),SLOT(removeAllowance(nAllowance*)));
    connect(na,SIGNAL(selected(QGraphicsObject*)),SIGNAL(selected(QGraphicsObject*)));
    aList.append(na);
    emit allowancesChanged();
    moveAllowances();
}

void nSurface::addAllowance(nAllowance *allowance)
{
    nAllowance* na = new nAllowance(w,h,this);
    na->setSelectable(this->selectable);
    connect(na,SIGNAL(addAllowanceMe(nAllowance*)),SLOT(addAllowance(nAllowance*)));
    connect(na,SIGNAL(removeMe(nAllowance*)),SLOT(removeAllowance(nAllowance*)));
    connect(na,SIGNAL(selected(QGraphicsObject*)),SIGNAL(selected(QGraphicsObject*)));
    aList.insert(aList.indexOf(allowance),na);
    emit allowancesChanged();
    moveAllowances();
}

void nSurface::removeAllowance()
{
    if(!aList.isEmpty())
    {
        aList.takeLast()->deleteLater();
        emit allowancesChanged();
        moveAllowances();
    }
}

void nSurface::removeAllowance(nAllowance *allowance)
{
    aList.takeAt(aList.indexOf(allowance))->deleteLater();
    emit allowancesChanged();
    moveAllowances();
}

QPixmap nSurface::slashPixmap(bool fdiag, const QPen &pen,  int hx) const
{
    hx-=5;
    QPixmap px(w,hx);
    px.fill();
    QPainter painter(&px);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(pen);
    QPoint p1,p2;
    int dx;
    fdiag ?  dx=0
            :dx=-hx;
    for(int i=0;i<w;i+=2*hx)
    {
        p1.setX(i);
        p1.setY(dx+hx);
        if((i+hx)<w)
        {
            p2.setX(i+hx);
            p2.setY(-dx);
        }
        else
        {
            p2.setX(w);
            p2.setY(hx-(w-i)-dx);
        }

        painter.drawLine(p1,p2);
    }
    painter.drawRect(0,0,w,hx);
    return px;

}

void nSurface::moveAllowances()
{
    float inc = -1*h;
    if(bot)
        inc=-inc;
    for(int i=0;i<aList.count();i++){
        aList.at(i)->setPos(0,inc*(i+1));
        aList.at(i)->setNum(i+1);
    }
    update();
}




nAllowance::nAllowance(float width, float height, nSurface *surface)
    :nSurface(width, height, surface),pSurf(surface)
{

}

void nAllowance::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(selectable)
    {
        highlighted=true;
        pSurf->setHighlighted(true);
        update();
        emit changed();
    }
}

void nAllowance::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLine line(0,h/2,w,h/2);
    QLine border1,border2;
    QColor color=Qt::black;

    if(this->bot)
    {
        border1.setLine(0,h/2,0,-1*h/2);
        border2.setLine(w,h/2,w,-1*h/2);
    }
    else
    {
        border1.setLine(0,h/2,0,h*1.5);
        border2.setLine(w,h/2,w,h*1.5);
    }

    painter->setPen(QPen(color,5));
    painter->drawLine(border1);
    painter->drawLine(border2);

    painter->setRenderHint(QPainter::Antialiasing,true);
    if(highlighted)
        color=Qt::green;
    else
        color=Qt::blue;
    painter->setPen(QPen(color,5));
    painter->drawLine(line);
    QPoint slashP;
    if(bot)
    {
        slashP=QPoint(0,0-5);
    }
    else
    {
        slashP=QPoint(0,h/2);
    }
    QPixmap px= slashPixmap(num%2,painter->pen(),h/2);
    QFont font=painter->font();
    font.setPixelSize(30);
    painter->setFont(font);
    painter->drawText(QPointF(-40,h/2),(pSurf->name() + name()));
    painter->drawPixmap(slashP,px,px.rect());

    Q_UNUSED(option);
    Q_UNUSED(widget);
}



nPlane::nPlane(QGraphicsItem *parent)
    :QGraphicsObject(parent),w(1),h(1)
{
}

nPlane::nPlane(float width, float height, QGraphicsItem *parent)
    :QGraphicsObject(parent),w(width),h(height)
{
}

nPlane::nPlane(plane *pl, QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    QList<nSurface *> bot, top;
    for(int i=0;i<pl->TopList()->count();i++)
        top.append(new nSurface(pl->TopList()->at(i),this));
    for(int i=0;i<pl->BotList()->count();i++)
    {
        nSurface* s = new nSurface(pl->BotList()->at(i),this);
        s->setBot(true);
        bot.append(s);
    }
    addSurfaces(bot,top);
    sortSurfaces();
    moveSurfaces();
}

QRectF nPlane::boundingRect() const
{
    return QRect(0,0,1500,1000);
}

void nPlane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal height = 1000 /(qreal)calcItemsHeight();
    qreal y1,y2,x;
    painter->setPen(QPen(Qt::black,5));
    painter->setRenderHint(QPainter::Antialiasing,true);

    if(!topList.isEmpty())
    {
        //leftBorder
        x=0;
        y1=topXList.at(0)->pos().ry()+height/2.;
        if(!botList.isEmpty())
            y2=botXList.at(0)->pos().ry()+height/2.;
        else
            y2=1000;
        painter->drawLine(x,y1,x,y2);


        //centerTop
        for(int i=0;i<topXList.count()-1;i++)
        {
            x=topXList.at(i+1)->pos().rx();
            y1=topXList.at(i)->pos().ry()+ height/2.;
            y2=topXList.at(i+1)->pos().ry()+ height/2.;
            painter->drawLine(x,y1,x,y2);
        }

        //centerBot

        for(int i=0;i<botXList.count()-1;i++)
        {
            x=botXList.at(i+1)->pos().rx();
            y1=botXList.at(i)->pos().ry()+ height/2.;
            y2=botXList.at(i+1)->pos().ry()+ height/2.;
            painter->drawLine(x,y1,x,y2);
        }

        //rightBorder
        x=1500;
        y1=topXList.last()->pos().ry()+height/2.;
        if(!botList.isEmpty())
            y2=botXList.last()->pos().ry()+height/2.;
        else
            y2=1000;
        painter->drawLine(x,y1,x,y2);
    }


    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void nPlane::addTopSurface(nSurface *surf)
{
    topList.append(surf);
    connect(surf,SIGNAL(selected(QGraphicsObject*)),this,SIGNAL(objectSelected(QGraphicsObject*)));
    connect(surf,SIGNAL(allowancesChanged()),this,SLOT(moveSurfaces()));
    sortSurfaces();
    moveSurfaces();
}

void nPlane::addBotSurface(nSurface *surf)
{
    botList.append(surf);
    connect(surf,SIGNAL(selected(QGraphicsObject*)),this,SIGNAL(objectSelected(QGraphicsObject*)));
    connect(surf,SIGNAL(allowancesChanged()),this,SLOT(moveSurfaces()));
    sortSurfaces();
    moveSurfaces();
}

void nPlane::addSurfaces(QList<nSurface *> &bot, QList<nSurface *> &top)
{
    QList<nSurface *>::iterator iter;
    for(iter=bot.begin();iter<bot.end();iter++)
    {
        botList.append(*iter);
        connect(*iter,SIGNAL(selected(QGraphicsObject*)),this,SIGNAL(objectSelected(QGraphicsObject*)));
        connect(*iter,SIGNAL(allowancesChanged()),this,SLOT(moveSurfaces()));
        connect(*iter,SIGNAL(changed()),SLOT(needUpdate()));
    }

    for(iter=top.begin();iter<top.end();iter++)
    {
        topList.append(*iter);
        connect(*iter,SIGNAL(selected(QGraphicsObject*)),this,SIGNAL(objectSelected(QGraphicsObject*)));
        connect(*iter,SIGNAL(allowancesChanged()),this,SLOT(moveSurfaces()));
        connect(*iter,SIGNAL(changed()),SLOT(needUpdate()));
    }
}

void nPlane::setSurfacesSelectable(bool flag)
{
    for(int i=0;i<topList.count();i++)
        topList.at(i)->setSelectable(flag);
    for(int i=0;i<botList.count();i++)
        botList.at(i)->setSelectable(flag);
}

void nPlane::setAllowancesSelectable(bool flag)
{
    for(int i=0;i<topList.count();i++)
        topList.at(i)->setAllowanceSelectable(flag);
    for(int i=0;i<botList.count();i++)
        botList.at(i)->setAllowanceSelectable(flag);
}

void nPlane::sortSurfaces()
{
    topXList.clear();
    topXList.append(topList);
    std::sort(topXList.begin(),topXList.end(),nSurface::compareX);
    topYList.clear();
    topYList.append(topList);
    std::sort(topYList.begin(),topYList.end(),nSurface::compareY);
    botXList.clear();
    botXList.append(botList);
    std::sort(botXList.begin(),botXList.end(),nSurface::compareX);
    botYList.clear();
    botYList.append(botList);
    std::sort(botYList.begin(),botYList.end(),nSurface::compareY);

    int num=1;
    foreach(nSurface* s,topYList)
        s->setNum(num++);
    foreach(nSurface* s,botYList)
        s->setNum(num++);
}

void nPlane::moveSurfaces()
{
    for(nSurface* x:topList)
    {
        qDebug()<<x->midPoint();
    }
    qreal topWidth, botWidth, height;
    qreal itemX, itemY;
    height = 1000 /(qreal)calcItemsHeight();
    if(!topList.isEmpty())
    {
        topWidth = 1500 /(qreal)topList.count();
        for(int i=0;i<topList.count();i++)
        {
            itemX=topWidth*topXList.indexOf(topList.at(i));
            itemY=(height*calcSurfaceY(topList.at(i))-1000)*-1;
            topList.at(i)->setPos(itemX,itemY);
            topList.at(i)->setHeight(height);
            topList.at(i)->setWidth(topWidth);
        }
    }
    if(!botList.isEmpty())
    {
        botWidth = 1500/(qreal)botList.count();
        for(int i=0;i<botList.count();i++)
        {
            itemX=botWidth*botXList.indexOf(botList.at(i));
            itemY=(height*calcSurfaceY(botList.at(i))-1000)*-1;
            botList.at(i)->setPos(itemX,itemY);
            botList.at(i)->setHeight(height);
            botList.at(i)->setWidth(botWidth);
        }
    }
}

int nPlane::calcSurfaceY(nSurface *surf)
{
    int pos = topYList.indexOf(surf);
    if(pos>=0)
    {
        int botInc=1+botList.count();
        for(int i=0;i<botList.count();i++)
        {
            botInc+=botList.at(i)->getInc();
        }
        botInc=qMax(botInc,2);

        int topInc=pos;
        for(int i=0;i<pos;i++)
        {
            topInc+=topYList.at(i)->getInc();
        }
        return (botInc + topInc);
    }
    pos = botYList.indexOf(surf);
    int inc= pos+1;
    for(int i=0;i<=pos;i++)
    {
        inc+=botYList.at(i)->getInc();
    }
    return inc;
}

int nPlane::calcItemsHeight()
{
    int h = topList.count() + qMax(botList.count(),2);
    for(int i=0;i<topList.count();i++)
        h+=topList.at(i)->getInc();
    for(int i=0;i<botList.count();i++)
        h+=botList.at(i)->getInc();
    return h;
}

void nPlane::needUpdate()
{
    update();
}

void nPlane::testSlot(QGraphicsObject *)
{
    qDebug()<<"test";
}

void nSurface::setAllowanceSelectable(bool flag)
{
    foreach (nAllowance* all, aList) {
        all->setSelectable(flag);
    }
}

void nSurface::setHighlighted(const bool &hl)
{
    this->boundingRect().contains(QCursor::pos()) ? highlighted=true : highlighted=hl;
}

QString nSurface::name()
{
    return QString::number(num);
}


nSimpleViewer::nSimpleViewer(QWidget *parent)
    :QWidget(parent)
{
    QVBoxLayout* vb = new QVBoxLayout();
    this->setLayout(vb);
    view = new QGraphicsView(this);
    vb->addWidget(view);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(-30,-30,1500,1000);
    axis =new nAxis(1500,1000);
    scene->addItem(axis);
    axis->setPos(0,0);
    view->setScene(scene);
}

void nSimpleViewer::setMainPlane(nPlane *pl)
{
    scene->addItem(pl);
    scene->update();
    mPlane=pl;
    connect(pl,SIGNAL(objectSelected(QGraphicsObject*)),SIGNAL(objectSelected(QGraphicsObject*)));
}


void nSimpleViewer::resizeEvent(QResizeEvent *)
{
    view->setTransform(QTransform::fromScale(0.97,0.97));
    view->scale(view->width()/1550.,view->height()/1550.);
}

void nSimpleViewer::testSlot(QGraphicsObject *)
{
}

void nSimpleViewer::setSelectionMode(nSimpleViewer::selectionMode mode)
{
    if(mPlane)
    {
        mPlane->setSurfacesSelectable(false);
        mPlane->setAllowancesSelectable(false);

        if(mode==nSimpleViewer::Surfaces || mode==nSimpleViewer::AllSurfaces)
            mPlane->setSurfacesSelectable(true);
        if(mode==nSimpleViewer::Allowances || mode==nSimpleViewer::AllSurfaces)
            mPlane->setAllowancesSelectable(true);
    }
}





void nAxis::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::black,5));
    painter->drawLine(0,_y + 50,0,0);
    painter->drawLine(-50,_y,_x,_y);
}
