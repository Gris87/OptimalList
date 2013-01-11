#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CopyableTable;

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
    qint64 mRandomSeed;

    void setItemAndScroll(CopyableTable *aTable, int row, int column, QString aText);
    void calculateRate(CopyableTable *aTable, int row);
    void setAndCalculate(CopyableTable *aTable, int row, QString aText);
    template <typename T> void testList(const QString aElementName);
};

#endif // MAINWINDOW_H
