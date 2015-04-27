#ifndef NPAGE_H
#define NPAGE_H

#include <QWidget>
#include <parserS.h>
#include <QFileDialog>
#include "tools.h"

class nPage : public QWidget
{
    Q_OBJECT
public:
    explicit nPage(QWidget *parent = 0);

    virtual void setPlane(nPlane* plane){Q_UNUSED(plane);}
    nPlane* getPlane(){return pl;}

protected:
    QPushButton* nextButton;
    QPushButton* preButton;
    QHBoxLayout* leftControl;
    QHBoxLayout* rightControl;
    QHBoxLayout* mainL;

    nPlane* pl;
signals:
    void nextPage();
    void prePage();
public slots:

};

class nFirstPage : public nPage
{
    Q_OBJECT
public:
    nFirstPage(QWidget* parent);

private:
    tScetch* tsc;

private slots:

    void openFile();

};

class nSecondPage: public nPage
{
    Q_OBJECT
public:
    nSecondPage(QWidget* parent);

    void setPlane(nPlane* plane);

private:
    nSimpleViewer* viewer;
    myToolBar* toolBar;

private slots:
    void setSelectionMode(nSimpleViewer::selectionMode);
};

#endif // NPAGE_H
