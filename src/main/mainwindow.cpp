#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OptimalList<int> a;

    for (int i=0; i<10000000; i++)
    {
        a.append(i);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
