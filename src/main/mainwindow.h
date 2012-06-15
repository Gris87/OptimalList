#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDateTime>

#include <windows.h>
#include <psapi.h>

#include <QList>
#include "src/list/optimallist.h"

#include "src/widgets/tablenumericitem.h"

namespace Ui {
class MainWindow;
}

struct sStruct
{
    int a;
    long b;
    double c;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;

    template <typename T> void testList(const QString aElementName);
};

#endif // MAINWINDOW_H
