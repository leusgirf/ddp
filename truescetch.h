#ifndef TSCETCH_H
#define TSCETCH_H

#include <planeobjects.h>
#include <QPainter>

class tScetch : public QWidget
{
    Q_OBJECT
public:
    explicit tScetch(QWidget *parent = 0);
    virtual void addMainPlane(plane* pl){mPlane=pl;update();}
    plane* getMainPlane(){return mPlane;}


    void setActive(bool a){active=a;}
    bool isActive(){return active;}

    void setState(int a){state=a;}
protected:


    plane* mPlane;
    double scaleX,scaleY;
    int minX();
    int maxX();
    int maxY();

    virtual int trX(double x){return (x*scaleX+ofs);}
    virtual int trY(double y){return (height())-y*scaleY-ofs;}
    virtual void setTrs();


    bool active;
    int ofs;
    int state;

    virtual void paintEvent(QPaintEvent *event);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
signals:
    void sideChoosed(int,int);
    void operationChoosed(int);

public slots:

private slots:

};

#endif // TSCETCH_H
