#include "opFrame.h"

opFrame::opFrame(QObject *parent) :
    QObject(parent)
{
    frame = new QFrame;
    side = new QPushButton(QString("Поверхность"));
    base= new QPushButton(QString("База"));
    base->setEnabled(false);
    bs=-1;
    op= new QPushButton(QString("Операция"));
    sideL=new QLabel("Не выбрана");
    baseL=new QLabel("Ось");
    opL=new QLabel("Не выбрана");
    QGridLayout *gr=new QGridLayout();
    gr->addWidget(side,0,0);
    gr->addWidget(base,1,0);
    gr->addWidget(op,2,0);
    gr->addWidget(sideL,0,1);
    gr->addWidget(baseL,1,1);
    gr->addWidget(opL,2,1);
    frame->setLayout(gr);
    frame->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    wantSide=false;
    wantBase=false;
    bs=-2;
    sd=-2;
    connect(side,SIGNAL(clicked()),SLOT(clSide()));
    connect(base,SIGNAL(clicked()),SLOT(clBase()));
}


void opFrame::clBase()
{
    if(wantBase)
    {
        base->setDown(false);
    }
    else
    {
        base->setDown(true);
        emit wBase();
    }
    wantBase=!wantBase;
}

void opFrame::clSide()
{
    if(wantSide)
    {
        side->setDown(false);
    }
    else
    {
        side->setDown(true);
        emit wSide();
    }
    wantSide=!wantSide;
}

void opFrame::getWBase()
{
    if(wantBase)
    {
        clBase();
    }
    if(wantSide)
    {
        clSide();
    }
}

void opFrame::getWSide()
{
    if(wantSide)
    {
        clSide();
    }
    if(wantBase)
    {
        clBase();
    }
}

void opFrame::getChoose(int a)
{
    if(wantBase)
    {
        bs=a;
        if(a!=-1)baseL->setText(QString("Поверхность %1").arg(a));
        else    baseL->setText(QString("Ось"));
        calmDown();
    }
    if(wantSide)
    {
        if(a>-1)
        {
            if(sd!=-2)
            {
                emit sideUnset(sd);
            }
            emit sideSet(a);
            sd=a;
            sideL->setText(QString("Поверхность %1").arg(a));
        }
        calmDown();
    }
}

void opFrame::calmDown()
{
    if(wantBase)
    {
        wantBase=false;
        base->setDown(false);
    }
    if(wantSide)
    {
        wantSide=false;
        side->setDown(false);
    }
}

void opFrame::notLast()
{
    bs=-2;
    base->setEnabled(true);
    baseL->setText("Не выбрана");

}

