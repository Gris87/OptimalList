#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->testsTableWidget->setColumnWidth(0, 25);
    ui->testsTableWidget->setColumnWidth(1, 400);
    ui->testsTableWidget->setColumnWidth(2, 125);
    ui->testsTableWidget->setColumnWidth(3, 125);
    ui->testsTableWidget->setColumnWidth(4, 75);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setItemAndScroll(int row, int column, QString aText)
{
    TableNumericItem *aItem=new TableNumericItem(aText);

    ui->testsTableWidget->setItem(row, column, aItem);
    ui->testsTableWidget->scrollToItem(aItem);

    ui->progressBar->setValue(ui->progressBar->value()+1);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MainWindow::calculateRate(int row)
{
    double aValue1=ui->testsTableWidget->item(row, 2)->text().toDouble();
    double aValue2=ui->testsTableWidget->item(row, 3)->text().toDouble();

    if (aValue2==0)
    {
        if (aValue1==0)
        {
            setItemAndScroll(row, 4, QString::number(1, 'f', 3));
        }
        else
        {
            setItemAndScroll(row, 4, QString::number(aValue1, 'f', 3));
        }
    }
    else
    {
        setItemAndScroll(row, 4, QString::number(aValue1/aValue2, 'f', 3));
    }

    ui->testsTableWidget->item(row, 3)->setFont(QFont("Arial", 12, QFont::Bold));
    ui->testsTableWidget->item(row, 4)->setFont(QFont("Arial", 12, QFont::Bold));

    if (aValue2<=aValue1)
    {
        ui->testsTableWidget->item(row, 3)->setTextColor(QColor(128, 128, 255));
        ui->testsTableWidget->item(row, 4)->setTextColor(QColor(128, 128, 255));
    }
    else
    {
        ui->testsTableWidget->item(row, 3)->setTextColor(QColor(255, 128, 128));
        ui->testsTableWidget->item(row, 4)->setTextColor(QColor(255, 128, 128));
    }
}

void MainWindow::setAndCalculate(int row, QString aText)
{
    setItemAndScroll(row, 3, aText);
    calculateRate(row);
}

template <typename T>
void MainWindow::testList(const QString aElementName)
{
    OptimalList<T> aMyList;
    QList<T> aQtList;
    T aValue;

//  int aElemCount=30000000; // 1GB RAM free required
    int aElemCount=3000000;

    int lastRow=ui->testsTableWidget->rowCount();

    qint64 aStart;
    qint64 aTimeStamp;

    qint64 aMemoryBefore;
    qint64 aMemoryAfter;
    HANDLE aProcess=GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS aMemory;

    //------------------------------------------------------------

    ui->testsTableWidget->setRowCount(lastRow+3);

    for (int i=0; i<3; ++i)
    {
        ui->testsTableWidget->setItem(lastRow+i, 0, new TableNumericItem(QString::number(lastRow+i+1)));
    }

    ui->testsTableWidget->setItem(lastRow,   1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Memory, bytes ("+QString::number(sizeof(T))+" bytes per element)"));
    ui->testsTableWidget->setItem(lastRow+1, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for appending, ms"));
    ui->testsTableWidget->setItem(lastRow+2, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for clearing, ms"));

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    //------------------------------------------------------------

    GetProcessMemoryInfo(aProcess, &aMemory, sizeof(PROCESS_MEMORY_COUNTERS));
    aMemoryBefore=aMemory.WorkingSetSize;

    aStart=QDateTime::currentMSecsSinceEpoch();

    for (int i=0; i<aElemCount; ++i)
    {
        aQtList.append(aValue);
    }

    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    GetProcessMemoryInfo(aProcess, &aMemory, sizeof(PROCESS_MEMORY_COUNTERS));
    aMemoryAfter=aMemory.WorkingSetSize;



    setItemAndScroll(lastRow,   2, QString::number(aMemoryAfter-aMemoryBefore));
    setItemAndScroll(lastRow+1, 2, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();
    aQtList.clear();
    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setItemAndScroll(lastRow+2, 2, QString::number(aTimeStamp-aStart));

    //============================================================

    GetProcessMemoryInfo(aProcess, &aMemory, sizeof(PROCESS_MEMORY_COUNTERS));
    aMemoryBefore=aMemory.WorkingSetSize;

    aStart=QDateTime::currentMSecsSinceEpoch();

    for (int i=0; i<aElemCount; ++i)
    {
        aMyList.append(aValue);
    }

    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    GetProcessMemoryInfo(aProcess, &aMemory, sizeof(PROCESS_MEMORY_COUNTERS));
    aMemoryAfter=aMemory.WorkingSetSize;



    setAndCalculate(lastRow,   QString::number(aMemoryAfter-aMemoryBefore));
    setAndCalculate(lastRow+1, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();
    aMyList.clear();
    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setAndCalculate(lastRow+2, QString::number(aTimeStamp-aStart));
}

void MainWindow::on_startButton_clicked()
{
    ui->testsTableWidget->setSortingEnabled(false);
    ui->testsTableWidget->clearContents();
    ui->testsTableWidget->setRowCount(0);

    ui->progressBar->setMaximum(17*3*3);
    ui->progressBar->setValue(0);

    testList<bool>("bool");
    testList<char>("char");
    testList<signed char>("signed char");
    testList<uchar>("uchar");
    testList<short>("short");
    testList<ushort>("ushort");
    testList<int>("int");
    testList<uint>("uint");
    testList<long>("long");
    testList<ulong>("ulong");
    testList<qint64>("qint64");
    testList<quint64>("quint64");
    testList<float>("float");
    testList<double>("double");
    testList<long double>("long double");
    testList<sStruct>("sStruct");
    //testList<QString>("QString");

    ui->testsTableWidget->setSortingEnabled(true);
    ui->testsTableWidget->sortByColumn(0, Qt::AscendingOrder);

    ui->progressBar->setValue(0);
}
