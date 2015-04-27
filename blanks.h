#ifndef BLANKS_H
#define BLANKS_H


#include <QtWidgets>
#include <QWidget>
#include <cmath>

class blankBeatPrm: public QWidget
{
    Q_OBJECT

public:
    blankBeatPrm(QString formula, bool line, bool kor=false, bool smes=false, bool per=false, bool check=false);
    ~blankBeatPrm(){;}
    QLineEdit *pr1,*pr2,*pr3;
    QCheckBox *cb1,*cb2,*cb3;
    bool p,s,k,isLine;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public slots:
    void calc();
signals:
    void result(double);
};


class blankBeatCalc: public QWidget
{
    Q_OBJECT

public:
    blankBeatCalc(QString nm, bool typeS, QWidget* parent);
    ~blankBeatCalc(){;}
    QHBoxLayout* mainHorLayout;
    QVBoxLayout* type;
    QCheckBox* isVal;
    QFrame* subType;
    QLineEdit* res;
    blankBeatPrm* prm;
    QComboBox* cb2;
    bool sType;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private slots:
    void typeChanged(int a);
    void subTypeChagedA(int a);
    void subTypeChagedB(int a);
    void getRes(double r);
    void isValToggled();
};

class baseBeatCalc: public QWidget
{
    Q_OBJECT
public:
    baseBeatCalc(QString nm, QWidget* parent);
    QComboBox* cb1;
    QComboBox* cb2;
    QLabel* cb2l;

    QLineEdit* result;
    QFrame* prm;
    QLineEdit *v1, *v2;
    QHBoxLayout* mainHorLayout;

private slots:
    void tchns(int);
    void cbChanged();
    void calc();

};



#endif // BLANKS_H
