#ifndef MSCETCHER_H
#define MSCETCHER_H

#include <QWidget>
#include <QtWidgets>

#include <npage.h>

class MScetcher : public QWidget
{
    Q_OBJECT
public:
    explicit MScetcher(QWidget *parent = 0);


private:
    QFrame* mFrame;

    QStackedLayout* stackedW;
    QList<nPage*> pList;

public slots:
    void nextPage();
    void prePage();

};

#endif // MSCETCHER_H
