#ifndef SSCETCH_H
#define SSCETCH_H
#include <truescetch.h>
#include <QMouseEvent>

class sScetch : public tScetch
{
    Q_OBJECT

public:
    sScetch(QWidget* parent);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    void setTrs();
    int trXt(double x){return (x*scaleXt+ofs);}
    int trXb(double x){return (x*scaleXb+ofs);}

    double scaleXb, scaleXt;
    double mouseZone;
signals:

public slots:

private slots:

};

#endif // SSCETCH_H
