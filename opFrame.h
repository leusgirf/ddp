#ifndef OPERATION_H
#define OPERATION_H

#include <QObject>
#include <QtWidgets>

class opFrame : public QObject
{
    Q_OBJECT
public:
    explicit opFrame(QObject *parent = 0);
    QFrame* frame;
    void notLast();
private:
    QPushButton* side;
    QPushButton* base;
    QPushButton* op;
    QLabel* sideL;
    QLabel* baseL;
    QLabel* opL;
    bool wantSide,wantBase;
    int bs,sd;
    void calmDown();

signals:
    void wSide();
    void wBase();
    void sideSet(int);
    void sideUnset(int);

public slots:
    void getWSide();
    void getWBase();
    void getChoose(int);


private slots:
    void clSide();
    void clBase();

};

#endif // OPERATION_H
