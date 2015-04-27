#include "viewer.h"

viewer::viewer(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedWidth(200);
    QVBoxLayout* vb = new QVBoxLayout();
    this->setLayout(vb);
    QFrame* fr= new QFrame(this);
    fr->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    vb->addWidget(fr);
    vb = new QVBoxLayout();
    fr->setLayout(vb);
    QScrollArea* ar= new QScrollArea(this);
    ar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ar->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    vb->addWidget(ar);
    QHBoxLayout* hb = new QHBoxLayout();
    vb->addLayout(hb);
    QToolButton* tb= new QToolButton(this);
    //tb->setText("");
    tb->setFixedSize(40,40);
    tb->setIcon(QIcon(":/pics/plus"));
    hb->addWidget(tb);
    tb= new QToolButton(this);
    tb->setText("");
    tb->setFixedSize(40,40);
    tb->setIcon(QIcon(":/pics/minus"));
    hb->addWidget(tb);
    tb= new QToolButton(this);
    tb->setText("");
    tb->setIcon(QIcon(":/pics/ext"));
    tb->setFixedSize(40,40);
    hb->addWidget(tb);

}



