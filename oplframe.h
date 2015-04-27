#ifndef OPLFRAME_H
#define OPLFRAME_H

#include <QObject>
#include <opFrame.h>

class OPLFrame : public QObject
{
    Q_OBJECT
public:
    explicit OPLFrame(QObject *parent = 0);
    QFrame* mainFrame;
    QVBoxLayout* mL;
    QPushButton* addButton;
    QScrollArea* scrArea;
    QFrame* fr;
    QList<opFrame*> oList;


signals:
    void resendWSide();
    void resendWBase();
    void resendChoose(int);
    void resendSideSet(int);
    void resentSideUnset(int);

public slots:

private slots:
    void addOpFrame();
    void gotWSide();
    void gotWBase();
    void btnPushed();

};

#endif // OPLFRAME_H
