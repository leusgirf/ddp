#include "calc.h"

calcTable::calcTable(QList<operation*> *ol,QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* vb = new QVBoxLayout();
    this->setLayout(vb);
    QLabel* lb=new QLabel("Таблица расчетов биений",this);
    lb->setFont(QFont("Times", 32, QFont::Bold));
    vb->addWidget(lb);
    QScrollArea* sca = new QScrollArea(this);
    sca->setEnabled(true);
    vb->addWidget(sca);
        QWidget* fr=new QWidget(this);
       //vb->addWidget(fr);
    vb=new QVBoxLayout();
    vb->setSpacing(3);
    fr->setLayout(vb);
    QString nm;
    QFont font=QFont("Times", 18, QFont::Bold);
    for(int i=0;i<ol->count();i++)
    {
        nm=QString("%1").arg((i+1)*5);
        while(nm.size()!=3) nm.prepend("0");
        lb=new QLabel(nm.prepend("Операция "),this);
        lb->setFont(font);
        vb->addWidget(lb);
        for(int j=0;j<ol->at(i)->count();j++)
        {
            operation* op=ol->at(i);
            if(op->bList.at(j)->type==blank)
            {
                blankBeatCalc* bbc=new blankBeatCalc(op->bList.at(j)->name1,op->bList.at(j)->top1,this);
                vb->addWidget(bbc);
            }
            if(op->bList.at(j)->type==baseSur)
            {
                baseBeatCalc* bbc =new baseBeatCalc(op->bList.at(j)->name1,this);
                vb->addWidget(bbc);
            }
        }
    }
    vb->addStretch();
    sca->setWidget(fr);
    sca->show();
}


QSize calcTable::sizeHint() const
{
    return QSize(600, 500);
}

QSize calcTable::minimumSizeHint() const
{
    return QSize(600, 500);
}
