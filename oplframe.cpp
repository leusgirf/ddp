#include "oplframe.h"

OPLFrame::OPLFrame(QObject *parent) :
    QObject(parent)
{
    mainFrame =new QFrame();
    mL=new QVBoxLayout();
    addButton= new QPushButton("add");
    QPushButton* btn= new QPushButton("Расчитать");
    QHBoxLayout* hb=new QHBoxLayout();
    QVBoxLayout* vb=new QVBoxLayout();
    hb->addWidget(addButton);
    hb->addStretch();
    hb->addWidget(btn);
    fr=new QFrame;
    scrArea= new QScrollArea;
    scrArea->setWidget(fr);
    scrArea->setWidgetResizable(true);
    vb->addWidget(scrArea);
    vb->addLayout(hb);
    fr->setLayout(mL);
    mL->setAlignment(Qt::AlignHCenter);
    mL->setAlignment(Qt::AlignTop);
    mainFrame->setLayout(vb);
    mainFrame->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    mainFrame->resize(1000,1000);
    connect(addButton,SIGNAL(clicked()),SLOT(addOpFrame()));
    connect(btn,SIGNAL(clicked()),SLOT(btnPushed()));


}

void OPLFrame::addOpFrame()
{
    opFrame* fr=new opFrame();
    mL->addWidget(fr->frame);
    if(oList.count()>0) oList.last()->notLast();
    oList.append(fr);
    connect(fr,SIGNAL(wBase()),SLOT(gotWBase()));
    connect(fr,SIGNAL(wSide()),SLOT(gotWSide()));
    connect(this,SIGNAL(resendWBase()),fr,SLOT(getWBase()));
    connect(this,SIGNAL(resendWSide()),fr,SLOT(getWSide()));
    connect(this,SIGNAL(resendChoose(int)),fr,SLOT(getChoose(int)));
    connect(fr,SIGNAL(sideUnset(int)),this,SIGNAL(resentSideUnset(int)));
    connect(fr,SIGNAL(sideSet(int)),this,SIGNAL(resendSideSet(int)));
}



void OPLFrame::gotWBase()
{
    emit resendWBase();
}

void OPLFrame::gotWSide()
{
    emit resendWSide();
}

void OPLFrame::btnPushed()
{
    oList.at(24)->frame->setSizeIncrement(40,60);
}
