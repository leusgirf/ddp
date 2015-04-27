#ifndef BEATSCETCH_H
#define BEATSCETCH_H

#include <sscetch.h>
#include <beats.h>

class beatScetch : public sScetch
{
public:
    beatScetch(QWidget *parent);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    void setTrs();
private:

};





#endif // BEATSCETCH_H
