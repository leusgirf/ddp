#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <parserS.h>
#include <nplane.h>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout* hb= new QHBoxLayout();
    ui->centralWidget->setLayout(hb);
    sct= new MScetcher();
    hb->addWidget(sct);
}


MainWindow::~MainWindow()
{
    delete ui;
}

