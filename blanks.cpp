#include "blanks.h"

blankBeatCalc::blankBeatCalc(QString nm, bool typeS, QWidget* parent)
    :QWidget(parent)
{
    sType=typeS;
    this->setFixedHeight(170);
    prm=NULL;
    subType=NULL;
    isVal=NULL;
    QHBoxLayout* hb= new QHBoxLayout();
    this->setLayout(hb);
    QFrame* fr=new QFrame(this);
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    hb->addWidget(fr);
    mainHorLayout=new QHBoxLayout();
    fr->setLayout(mainHorLayout);
    //
    hb->addStretch();

    fr=new QFrame(this);
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    QVBoxLayout* vb=new QVBoxLayout();
    QLabel* lb=new QLabel("Результат расчета\nбиения:");
    res=new QLineEdit("Результат не определен",this);
    res->setEnabled(false);
    vb->addWidget(lb);
    vb->addWidget(res);
    vb->addStretch();
    fr->setLayout(vb);
    hb->addWidget(fr);
    //name
    fr=new QFrame(this);
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    mainHorLayout->addWidget(fr);
    lb= new QLabel(nm,this);
    lb->setFont(QFont("Times", 16, QFont::Bold));
    vb=new QVBoxLayout();
    fr->setLayout(vb);
    vb->addWidget(lb,0,(Qt::AlignLeft|Qt::AlignTop));
    //type
    fr= new QFrame();
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    lb=new QLabel("Выберете способ\nизготовления заготовки:",this);
    QComboBox* cb=new QComboBox(this);
    cb->addItems(QStringList()<<"Литьё"<<"Штамповка");
    cb->setCurrentIndex(-1);
    type=new QVBoxLayout();
    fr->setLayout(type);
    type->addWidget(lb,0,Qt::AlignLeft);
    type->addWidget(cb,0,Qt::AlignLeft);
    type->setSpacing(2);
    mainHorLayout->addWidget(fr);
    connect(cb,SIGNAL(currentIndexChanged(int)),SLOT(typeChanged(int)));
}

QSize blankBeatCalc::minimumSizeHint() const
{
    return QSize(500, 100);
}

QSize blankBeatCalc::sizeHint() const
{
    return QSize(500, 100);
}

void blankBeatCalc::typeChanged(int a)
{
    QLabel* lb;
    QComboBox* cb;
    QHBoxLayout* hb;
    QVBoxLayout* vb;
    if(subType!=NULL)
    {
        subType->deleteLater();
        subType=NULL;
    }
    if(isVal!=NULL)
    {
        isVal->deleteLater();
        isVal=NULL;
    }
    if(prm!=NULL)
    {
        prm->deleteLater();
        prm=NULL;
    }
    switch (a) {
    case 0:
        subType= new QFrame();
        hb= new QHBoxLayout();
        subType->setLayout(hb);
        lb=new QLabel("Тип литья:",subType);
        cb=new QComboBox(subType);
        cb->addItems(QStringList()<<"В песчанные формы"<<"В кокель"<<"Под давлением"<<"Оболоч./ По выпловл. мод.");
        cb->setCurrentIndex(-1);
        hb->addWidget(lb);
        hb->addWidget(cb);
        hb->addStretch();
        type->addWidget(subType);
        connect(cb,SIGNAL(currentIndexChanged(int)),SLOT(subTypeChagedA(int)));



        break;
    case 1:

        subType= new QFrame();
        vb=new QVBoxLayout();
        hb= new QHBoxLayout();
        subType->setLayout(vb);
        vb->addLayout(hb);
        lb=new QLabel("Тип штамповки:",subType);
        cb=new QComboBox(subType);
        cb2=cb;
        cb->addItems(QStringList()<<"Поковка"<<"Штамповка");
        cb->setCurrentIndex(-1);
        hb->addWidget(lb);
        hb->addWidget(cb);
        hb->addStretch();
        type->addWidget(subType,0,Qt::AlignLeft);
        connect(cb,SIGNAL(currentIndexChanged(int)),SLOT(subTypeChagedB(int)));
        break;
    default:
        break;
    }

}


void blankBeatCalc::subTypeChagedA(int a)
{
    if(prm!=NULL)
    {
        prm->deleteLater();
        prm=NULL;
    }
    QChar delta(0x0394);
    QChar sqr(0x221A);
    switch(a)
    {
    case 0:
        if(sType)
            prm = new blankBeatPrm(QString("Б=").append(sqr).append(delta).append("кор^2"),false,true);
        else
            prm = new blankBeatPrm(QString("Б=%1(%2кор^2 +%2см^2 +%2пер^2)").arg(sqr,delta),false,true,true,true);
        break;
    case 1:
        if(sType)
            prm = new blankBeatPrm(QString("Б=0"),false,false);
        else
            prm = new blankBeatPrm(QString("Б=").append(sqr).append(delta).append("пер^2"),false,false,false,true);
        break;
    case 2:
        if(sType)
            prm = new blankBeatPrm(QString("Б=0"),false,false);
        else
            prm = new blankBeatPrm(QString("Б=").append(sqr).append(delta).append("пер^2"),false,false,false,true);
        break;
    case 3:
        if(sType)
            prm = new blankBeatPrm(QString("Б=").append(sqr).append(delta).append("кор^2"),false,true,false,false,true);
        else
            prm = new blankBeatPrm(QString("Б=%1(%2кор^2 +%2см^2 +%2пер^2)").arg(sqr,delta),false,true,true,true,true);
        break;
    }
    connect(prm,SIGNAL(result(double)),SLOT(getRes(double)));
    prm->calc();
    mainHorLayout->addWidget(prm);

}

void blankBeatCalc::subTypeChagedB(int a)
{
    if(prm!=NULL)
    {
        prm->deleteLater();
        prm=NULL;
    }
    QChar delta(0x0394);
    QChar sqr(0x221A);
    switch(a)
    {
    case 0:
            prm = new blankBeatPrm(QString("Б=2%1(%2кор^2 +%2см^2);\n%2см=%2k*l").arg(sqr,delta),true);
            if(isVal!=NULL)
            {
                isVal->deleteLater();
                isVal=NULL;
            }
        break;
    case 1:
        if(isVal==NULL)
        {

            isVal=new QCheckBox();
            isVal->setText("Вал?");
            connect(isVal,SIGNAL(toggled(bool)),SLOT(isValToggled()));
            type->addWidget(isVal,0,Qt::AlignLeft);

            if(sType)
                prm = new blankBeatPrm(QString("Б=2%1(%2кор^2 +%2см^2);\n%2см=%2k*l").arg(sqr,delta),true);
            else
                prm = new blankBeatPrm(QString("Б=2%1(%2кор^2 +%2см^2);\n%2см=%2eks^2").arg(sqr,delta),false,true,true);
        }
        else
        {
            if(isVal->isChecked())
                prm = new blankBeatPrm(QString("Б=2%1(%2кор^2 +%2см^2);\n%2см=%2k*l").arg(sqr,delta),true);
            else
            {
                if(sType)
                    prm = new blankBeatPrm(QString("Б=2%1(%2кор^2 +%2см^2);\n%2см=%2k*l").arg(sqr,delta),true);
                else
                    prm = new blankBeatPrm(QString("Б=2%1(%2кор^2 +%2см^2);\n%2см=%2eks^2").arg(sqr,delta),false,true,true);
            }
        }
        break;
    }
    connect(prm,SIGNAL(result(double)),SLOT(getRes(double)));
    prm->calc();
    mainHorLayout->addWidget(prm);
}

void blankBeatCalc::getRes(double r)
{
    if(r<0)
    {
        res->setText("Результат не определен");
    }
    else res->setText(QString("%1").arg(r));
}

void blankBeatCalc::isValToggled()
{
    if(cb2->currentIndex()!=-1)
    {
        subTypeChagedB(cb2->currentIndex());
    }
}


blankBeatPrm::blankBeatPrm(QString formula, bool line, bool kor, bool smes, bool per, bool check)
{
    isLine=line;
    k=kor;
    p=per;
    s=smes;
    QChar delta(0x0394);
    QVBoxLayout *vb1= new QVBoxLayout();
    QVBoxLayout *vb2= new QVBoxLayout();
    QVBoxLayout* vb= new QVBoxLayout();
    QHBoxLayout* hb=new QHBoxLayout();
    QCheckBox* cb;
    hb->addLayout(vb1);
    hb->addLayout(vb2);
    this->setLayout(vb);
    QFrame* fr=new QFrame;
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    vb->addWidget(fr);
    vb= new QVBoxLayout();
    fr->setLayout(vb);
    QLabel* lb=new QLabel(formula,this);
    vb->addWidget(lb);
    vb->addLayout(hb);


    if(line)
    {
        lb=new QLabel(QString("кор").prepend(delta),this);
        pr1=new QLineEdit(this);
        vb1->addWidget(lb);
        vb2->addWidget(pr1);
        connect(pr1,SIGNAL(textChanged(QString)),SLOT(calc()));

        lb=new QLabel(QString("l"),this);
        pr2=new QLineEdit(this);
        vb1->addWidget(lb);
        vb2->addWidget(pr2);
        connect(pr2,SIGNAL(textChanged(QString)),SLOT(calc()));

        lb=new QLabel(QString("k").prepend(delta),this);
        pr3=new QLineEdit(this);
        vb1->addWidget(lb);
        vb2->addWidget(pr3);
        connect(pr3,SIGNAL(textChanged(QString)),SLOT(calc()));
    }
    else
    {
        if(kor)
        {
            pr1=new QLineEdit(this);
            vb2->addWidget(pr1);
            connect(pr1,SIGNAL(textChanged(QString)),SLOT(calc()));
            if(check)
            {
                cb=new QCheckBox(this);
                cb->setText(QString("кор").prepend(delta));
                connect(cb,SIGNAL(toggled(bool)),pr1,SLOT(setEnabled(bool)));
                connect(cb,SIGNAL(toggled(bool)),SLOT(calc()));
                cb->setChecked(false);
                pr1->setEnabled(false);
                vb1->addWidget(cb);
            }
            else
            {
                lb=new QLabel(QString("кор").prepend(delta),this);
                vb1->addWidget(lb);
            }
        }
        if(smes)
        {

            pr2=new QLineEdit(this);
            vb2->addWidget(pr2);
            connect(pr2,SIGNAL(textChanged(QString)),SLOT(calc()));
            if(check)
            {
                cb=new QCheckBox(this);
                cb->setText(QString("см").prepend(delta));
                connect(cb,SIGNAL(toggled(bool)),pr2,SLOT(setEnabled(bool)));
                connect(cb,SIGNAL(toggled(bool)),SLOT(calc()));
                cb->setChecked(false);
                pr2->setEnabled(false);
                vb1->addWidget(cb);
            }
            else
            {
                lb=new QLabel(QString("см").prepend(delta),this);
                vb1->addWidget(lb);
            }
        }
        if(per)
        {
            pr3=new QLineEdit(this);
            vb2->addWidget(pr3);
            connect(pr3,SIGNAL(textChanged(QString)),SLOT(calc()));
            if(check)
            {
                cb=new QCheckBox(this);
                cb->setText(QString("пер").prepend(delta));
                connect(cb,SIGNAL(toggled(bool)),pr3,SLOT(setEnabled(bool)));
                connect(cb,SIGNAL(toggled(bool)),SLOT(calc()));
                cb->setChecked(false);
                pr3->setEnabled(false);
                vb1->addWidget(cb);
            }
            else
            {
                lb=new QLabel(QString("пер").prepend(delta),this);
                vb1->addWidget(lb);
            }
        }
    }

}

void blankBeatPrm::calc()
{
    double r1,r2,r3,res;
    bool rb1,rb2,rb3;
    if(isLine)
    {
        r1=pr1->text().toDouble(&rb1);
        if(pr1->text().isEmpty())
        {
            r1=0;
            rb1=true;
        }
        r2=pr2->text().toDouble(&rb2);
        if(pr2->text().isEmpty())
        {
            r2=0;
            rb2=true;
        }
        r3=pr3->text().toDouble(&rb3);
        if(pr3->text().isEmpty())
        {
            r3=0;
            rb3=true;
        }
        if(rb1 && rb2 && rb3)
            res=2* pow((pow(r3*r2,2)+pow(r1,2)),0.5);
        else res=-1;
    }
    else
    {
        if(k)
        {
            if(pr1->isEnabled())
            {
                r1=pr1->text().toDouble(&rb1);
                if(pr1->text().isEmpty())
                {
                    r1=0;
                    rb1=true;
                }
            }
            else
            {
                rb1=true;
                r1=0;
            }
        }
        else
            {
                rb1=true;
                r1=0;
            }
        if(s)
        {
            if(pr2->isEnabled())
            {
                r2=pr2->text().toDouble(&rb2);
                if(pr2->text().isEmpty())
                {
                    r2=0;
                    rb2=true;
                }
            }
            else
            {
                rb2=true;
                r2=0;
            }
        }
        else
            {
                rb2=true;
                r2=0;
            }
        if(p)
        {
            if(pr3->isEnabled())
            {
                r3=pr3->text().toDouble(&rb3);
                if(pr3->text().isEmpty())
                {
                    r3=0;
                    rb3=true;
                }
            }
            else
            {
                rb3=true;
                r3=0;
            }
        }
        else
            {
                rb3=true;
                r3=0;
            }
        if(rb1 && rb2 && rb3)
            res=pow((pow(r1,2)+pow(r2,2)+pow(r3,2)),0.5);
        else
            res=-1;
    }
    emit result(res);
}

QSize blankBeatPrm::minimumSizeHint() const
{
    return QSize(200, 150);
}

QSize blankBeatPrm::sizeHint() const
{
    return QSize(200, 150);
}



baseBeatCalc::baseBeatCalc(QString nm, QWidget *parent)
    :QWidget(parent)
{
    prm=NULL;
    this->setFixedHeight(170);
    QHBoxLayout* hb= new QHBoxLayout();
    this->setLayout(hb);
    QFrame* fr=new QFrame(this);
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    hb->addWidget(fr);
    mainHorLayout=new QHBoxLayout();
    fr->setLayout(mainHorLayout);
    //
    hb->addStretch();

    fr=new QFrame(this);
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    QVBoxLayout* vb=new QVBoxLayout();
    QLabel* lb=new QLabel("Результат расчета\nбиения:");
    result=new QLineEdit("Результат не определен",this);
    result->setEnabled(false);
    vb->addWidget(lb);
    vb->addWidget(result);
    vb->addStretch();
    fr->setLayout(vb);
    hb->addWidget(fr);
    //name
    fr=new QFrame(this);
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    mainHorLayout->addWidget(fr);
    lb= new QLabel(nm,this);
    lb->setFont(QFont("Times", 16, QFont::Bold));
    vb=new QVBoxLayout();
    fr->setLayout(vb);
    vb->addWidget(lb,0,(Qt::AlignLeft|Qt::AlignTop));
    //type
    fr= new QFrame();
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    lb=new QLabel("Выберете способ установки:",this);
    cb1=new QComboBox(this);
    cb1->addItems(QStringList()<<"В трех-кулачковых пат./на плунжерных опр."
                  <<"В цанговых пат./на цанговых опр."
                  <<"В пат. и на опр. с гидропластом/В мембранных пат."
                  <<"В центрах");
    cb1->setCurrentIndex(-1);
    connect(cb1,SIGNAL(currentIndexChanged(int)),SLOT(tchns(int)));
    connect(cb1,SIGNAL(currentIndexChanged(int)),SLOT(cbChanged()));
    vb=new QVBoxLayout();
    fr->setLayout(vb);
    vb->addWidget(lb,0,Qt::AlignLeft);
    vb->addWidget(cb1,0,Qt::AlignLeft);
    cb2l=new QLabel("Выберете точность установки:",this);
    cb2=new QComboBox(this);
    cb2->addItems(QStringList()<<"Нормальная"
                  <<"Повышенная"
                  <<"Высокая");
    cb2->setCurrentIndex(-1);
    vb->addWidget(cb2l,0,Qt::AlignLeft);
    vb->addWidget(cb2,0,Qt::AlignLeft);
    cb2l->hide();
    cb2->hide();
    vb->setSpacing(2);
    vb->addStretch();
    connect(cb2,SIGNAL(currentIndexChanged(int)),SLOT(calc()));
    mainHorLayout->addWidget(fr);
}

void baseBeatCalc::tchns(int a)
{
    if(a==2)
    {
        cb2->hide();
        cb2->setCurrentIndex(-1);
        cb2l->hide();
    }
    else
    {
        cb2->show();
        cb2l->show();
    }
}

void baseBeatCalc::cbChanged()
{
    QChar delta(0x0394);
    QChar sqr(0x221A);
    if(prm!=NULL)
    {
        prm->deleteLater();
        prm=NULL;
    }
    if((cb1->currentIndex()!=-1) || cb1->currentIndex()==2)
    {

        prm=new QFrame(this);
        mainHorLayout->addWidget(prm);
        QVBoxLayout* vb=new QVBoxLayout();
        prm->setLayout(vb);
        QLabel* lb;
        QVBoxLayout* vb1 =new QVBoxLayout();
        QVBoxLayout* vb2 =new QVBoxLayout();
        switch(cb1->currentIndex())
        {
        case 0:
            lb=new QLabel(QString("Б = 2 * a1 * %1IT * (1 + 0.02 * l)").arg(sqr),prm);
            vb->addWidget(lb);
            lb=new QLabel(QString(sqr).append("IT="),prm);
            v1=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v1);

            lb=new QLabel(QString("l="),prm);
            v2=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v2);

            break;
        case 1:
            lb=new QLabel(QString("Б = 2 * a1 * %1IT * (1 + 0.01 * l)").arg(sqr),prm);
            vb->addWidget(lb);
            lb=new QLabel(QString(sqr).append("IT="),prm);
            v1=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v1);

            lb=new QLabel(QString("l="),prm);
            v2=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v2);
            break;
        case 2:

            lb=new QLabel(QString("Б = 0.08 *%1IT * (1+ 0.01*l)").arg(sqr),prm);
            vb->addWidget(lb);
            lb=new QLabel(QString(sqr).append("IT="),prm);
            v1=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v1);

            lb=new QLabel(QString("l="),prm);
            v2=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v2);
            break;
        case 3:

            lb=new QLabel(QString("Б = 2 *(a * %1D + (b + L)").arg(sqr),prm);
            vb->addWidget(lb);
            lb=new QLabel(QString("D="),prm);
            v1=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v1);

            lb=new QLabel(QString("L="),prm);
            v2=new QLineEdit(prm);
            vb1->addWidget(lb);
            vb2->addWidget(v2);
            break;


        }

        vb1->addStretch();
        vb2->addStretch();
        QHBoxLayout* hb=new QHBoxLayout();
        hb->addLayout(vb1);
        hb->addLayout(vb2);
        vb->addLayout(hb);
        connect(v1,SIGNAL(textChanged(QString)),this,SLOT(calc()));
        connect(v2,SIGNAL(textChanged(QString)),this,SLOT(calc()));
    }
}

void baseBeatCalc::calc()
{
    double res,r1,r2;
    bool br1,br2;
    if(cb1->currentIndex()==-1 || (cb1->currentIndex()!=3 && cb2->currentIndex()==-1) )
    {
        res=-1;
    }
    else
    {
        if(v1->text()=="")
        {
            r1=0;
            br1=true;
        }
        else
            r1=v1->text().toDouble(&br1);
        if(v2->text()=="")
        {
            r2=0;
            br2=true;
        }
        else
            r2=v2->text().toDouble(&br2);
        if(br1 && br2)
        {
            double a,b;
            switch (cb1->currentIndex())
            {
            case 0:
                switch(cb2->currentIndex())
                {
                case 0:
                    a=0.17;
                    break;
                case 1:
                    a=0.11;
                    break;
                case 2:
                    a=0.08;
                    break;
                }
                res=2* a * pow(r1,0.5) * (1+0.02*r2);
                break;
            case 1:
                switch(cb2->currentIndex())
                {
                case 0:
                    a=0.12;
                    break;
                case 1:
                    a=0.09;
                    break;
                case 2:
                    a=0.06;
                    break;
                }
                res=2* a * pow(r1,0.5) * (1+0.01*r2);
                break;
            case 2:
                res=0.08* pow(r1,0.5) * (1+0.01*r2);
                break;
            case 3:
                switch(cb2->currentIndex())
                {
                case 0:
                    a=0.006;
                    b=0.0005;
                    break;
                case 1:
                    a=0.0018;
                    b=0.00015;
                    break;
                case 2:
                    a=0.0009;
                    b=0.00007;
                    break;
                }
                res=2* (a * pow(r1,0.5) + (b*r2));
                qDebug()<<a<<b;
                break;
            default:
                res=-1;
            }
        }
        else res=-1;
    }
    if(res<0) result->setText("Результат не определен");
    else result->setText(QString("%1").arg(res));
}
