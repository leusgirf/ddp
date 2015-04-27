#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <oplframe.h>
#include <mscetcher.h>
#include <viewer.h>
#include <calc.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStackedWidget* stc;
    MScetcher* sct;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
