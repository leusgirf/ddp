#include "tools.h"


myToolBar::myToolBar(nSimpleViewer* viewer,QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* vb= new QVBoxLayout(this);
    QFrame* frame= new QFrame(this);
    frame->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    vb->addWidget(frame);
    mLayout = new QVBoxLayout(this);
    frame->setLayout(mLayout);
    vw=viewer;
}

void myToolBar::addBtn(myToolButton *btn)
{
    mLayout->addWidget(btn);
    btnList.append(btn);
    btn->setViewer(vw);
    connect(btn,SIGNAL(clicked()),SLOT(buttonPressed()));
}

void myToolBar::objectSelected(QGraphicsObject *obj)
{
    if(activeTb)
    {
        activeTb->objectChoosed(obj);
    }
}

void myToolBar::buttonPressed()
{
    activeTb=0;
    for(int i=0;i<btnList.count();i++)
    {
        if(btnList.at(i)==sender() && btnList.at(i)->isChecked())
            activeTb=btnList.at(i);
        else
            btnList.at(i)->setChecked(false);
    }
    if(activeTb)
        vw->setSelectionMode(activeTb->selectionMode());
    else vw->setSelectionMode(nSimpleViewer::noSelection);
}

myToolButton::myToolButton(QString name, QWidget *parent)
    :QPushButton(parent)
{
    setText(name);
    setCheckable(true);
    mode = nSimpleViewer::noSelection;
}


myAddAllowanceButton::myAddAllowanceButton(QWidget *parent)
    :myToolButton("Добавить\nПрипуск",parent)
{
    mode = nSimpleViewer::AllSurfaces;
}

void myAddAllowanceButton::objectChoosed(QGraphicsObject *obj)
{
    dynamic_cast<nSurface*>(obj)->addAllowance();
}


myDelAllowanceButton::myDelAllowanceButton(QWidget *parent)
    :myToolButton("Удалить\nПрипуск",parent)
{
    mode = nSimpleViewer::AllSurfaces;
}

void myDelAllowanceButton::objectChoosed(QGraphicsObject *obj)
{
    dynamic_cast<nSurface*>(obj)->removeAllowance();
}
