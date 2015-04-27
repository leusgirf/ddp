#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <beats.h>
#include <blanks.h>

class calcTable : public QWidget
{
    Q_OBJECT
public:
    explicit calcTable(QList<operation*> *ol, QWidget *parent = 0);
    QList<operation> oList;


signals:


public slots:

public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
};

#endif // WIDGET_H
