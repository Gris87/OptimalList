#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDateTime>

#include <windows.h>
#include <psapi.h>

#include <QList>
#include "src/list/optimallist.h"

#include "src/widgets/tablenumericitem.h"
#include "src/widgets/copyabletable.h"

#include "types.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_testsTabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    int mLastTabIndex;

    void setItemAndScroll(CopyableTable *aTable, int row, int column, QString aText);
    void calculateRate(CopyableTable *aTable, int row);
    void setAndCalculate(CopyableTable *aTable, int row, QString aText);
    template <typename T> void testList(const QString aElementName);
};

#endif // MAINWINDOW_H
