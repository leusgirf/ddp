#include "mscetcher.h"

MScetcher::MScetcher(QWidget *parent) :
    QWidget(parent)
{
    mFrame= new QFrame(this);


    QHBoxLayout* hb=new QHBoxLayout(this);
    hb->addWidget(mFrame);
    hb = new QHBoxLayout(mFrame);
    stackedW = new QStackedLayout();
    hb->addLayout(stackedW);

    nPage* pg = new nFirstPage(this);
    pList.append(pg);
    stackedW->addWidget(pg);
    connect(pg,SIGNAL(nextPage()),SLOT(nextPage()));
    connect(pg,SIGNAL(prePage()),SLOT(prePage()));

    pg = new nSecondPage(this);
    pList.append(pg);
    stackedW->addWidget(pg);
    connect(pg,SIGNAL(nextPage()),SLOT(nextPage()));
    connect(pg,SIGNAL(prePage()),SLOT(prePage()));

}

 void MScetcher::nextPage()
 {
     int index = stackedW->currentIndex();
     pList.at(index+1)->setPlane(pList.at(index)->getPlane());
     stackedW->setCurrentIndex(index+1);
 }

 void MScetcher::prePage()
 {
     int index = stackedW->currentIndex();
     stackedW->setCurrentIndex(index -1);
 }

