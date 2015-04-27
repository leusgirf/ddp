#include "npage.h"

nPage::nPage(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* vb= new QVBoxLayout(this);

    mainL = new QHBoxLayout();
    vb->addLayout(mainL);
    QHBoxLayout* hb = new QHBoxLayout(this);
    vb->addLayout(hb);

    leftControl = new QHBoxLayout();
    rightControl = new QHBoxLayout();
    hb->addLayout(leftControl);
    hb->addStretch();
    hb->addLayout(rightControl);

    nextButton = new QPushButton("Далее",this);
    rightControl->addWidget(nextButton);
    connect(nextButton,SIGNAL(clicked()),SIGNAL(nextPage()));

    preButton = new QPushButton("Назад",this);
    leftControl->addWidget(preButton);
    connect(preButton,SIGNAL(clicked()),SIGNAL(prePage()));
}


nFirstPage::nFirstPage(QWidget *parent)
    :nPage(parent)
{
    tsc = new tScetch(this);
    mainL->addWidget(tsc);
    nextButton->setEnabled(false);
    preButton->hide();

    QPushButton* btn = new QPushButton("Open stp",this);
    leftControl->addWidget(btn);
    connect(btn,SIGNAL(clicked()),SLOT(openFile()));
}

void nFirstPage::openFile()
{
    QList<plane*> pList;
    QString fname=QFileDialog::getOpenFileName(this,
                                                   tr("Open stp"),
                                                   "./sessions/",
                                                   tr("stp (*.stp)"));
    if(!fname.isEmpty())
    {
        QStringList sl;
        sl.clear();
        QString text;
        QFile file(fname);
        qDebug()<<fname;
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            {
                while(!stream.atEnd())
                {
                    text=stream.readLine();
                    if(text=="DATA;")
                    {
                        while(1)
                        {
                            text=stream.readLine();
                            if(text!="ENDSEC;") sl.append(text);
                            else break;
                        }
                    }
                }

            }
        }
        pList=parser(sl).getPlanesList();
        tsc->addMainPlane(pList.at(0));
        nextButton->setEnabled(true);
        pl = new nPlane(tsc->getMainPlane());
    }
}


nSecondPage::nSecondPage(QWidget *parent)
    :nPage(parent)
{
    viewer = new nSimpleViewer(this);
    mainL->addWidget(viewer);

    toolBar = new myToolBar(viewer,this);
    mainL->addWidget(toolBar);

    myToolButton* btn = new myAddAllowanceButton(this);
    toolBar->addBtn(btn);

    btn =new myDelAllowanceButton(this);
    toolBar->addBtn(btn);

    connect(toolBar,SIGNAL(modeSelected(nSimpleViewer::selectionMode)),
            SLOT(setSelectionMode(nSimpleViewer::selectionMode)));
    connect(viewer,SIGNAL(objectSelected(QGraphicsObject*)),
            toolBar,SLOT(objectSelected(QGraphicsObject*)));
}

void nSecondPage::setPlane(nPlane *plane)
{
    viewer->setMainPlane(plane);
}

void nSecondPage::setSelectionMode(nSimpleViewer::selectionMode mode)
{
    viewer->setSelectionMode(mode);
}
