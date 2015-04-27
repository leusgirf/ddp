#ifndef TOOLS_H
#define TOOLS_H
#include <beatscetch.h>
#include <nplane.h>



class myToolButton;

class myToolBar: public QWidget
{
    Q_OBJECT
public:
    myToolBar(nSimpleViewer* viewer,QWidget* parent);

    void addBtn(myToolButton* btn);
    void addStretch(){mLayout->addStretch();}
    QLayout* getLayout(){return mLayout;}

    nSimpleViewer* getViewer(){return vw;}

    QSize minimumSizeHint() {return QSize(100,300);}
    QSize sizeHint() {return QSize(100,300);}
private:
    QVBoxLayout* mLayout;
    nSimpleViewer* vw;
    QList<myToolButton*> btnList;
    myToolButton* activeTb;

signals:
    void modeSelected(nSimpleViewer::selectionMode mode);
private slots:
    void buttonPressed();
    void objectSelected(QGraphicsObject* obj);
};

class myToolButton: public QPushButton
{
    Q_OBJECT
public:
    myToolButton(QString name,QWidget* parent);
    QSize minimumSizeHint() {return QSize(100,100);}
    QSize sizeHint() {return QSize(100,100);}

    virtual void objectChoosed(QGraphicsObject* obj)=0;
    void setViewer(nSimpleViewer* viewer){vw=viewer;}
    nSimpleViewer::selectionMode selectionMode() {return mode;}
protected:
    nSimpleViewer* vw;
    nSimpleViewer::selectionMode mode;
};

class myAddAllowanceButton: public myToolButton
{
    Q_OBJECT
public:
    myAddAllowanceButton(QWidget* parent);

    void objectChoosed(QGraphicsObject* obj);
};

class myDelAllowanceButton: public myToolButton
{
    Q_OBJECT
public:
    myDelAllowanceButton(QWidget* parent);

    void objectChoosed(QGraphicsObject* obj);
};


#endif // TOOLS_H
