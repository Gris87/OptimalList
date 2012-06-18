#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mLastTabIndex=0;

    for (int i=0; i<6; i++)
    {
        CopyableTable *aTable=new CopyableTable(this);

        aTable->setColumnCount(5);

        aTable->setHorizontalHeaderItem(0, new QTableWidgetItem("N"));
        aTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
        aTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Value (QList)"));
        aTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Value (OptimalList)"));
        aTable->setHorizontalHeaderItem(4, new QTableWidgetItem("Rate"));

        aTable->setColumnWidth(0, 25);
        aTable->setColumnWidth(1, 400);
        aTable->setColumnWidth(2, 125);
        aTable->setColumnWidth(3, 125);
        aTable->setColumnWidth(4, 75);

        ui->testsTabWidget->addTab(aTable, "");
    }

    ui->testsTabWidget->setTabText(0, "Memory usage");
    ui->testsTabWidget->setTabText(1, "Time for appending");
    ui->testsTabWidget->setTabText(2, "Time for clearing");
    ui->testsTabWidget->setTabText(3, "Time for inserting");
    ui->testsTabWidget->setTabText(4, "Time for cloning");
    ui->testsTabWidget->setTabText(5, "Time for removing");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setItemAndScroll(CopyableTable *aTable, int row, int column, QString aText)
{
    ui->testsTabWidget->setCurrentWidget(aTable);

    TableNumericItem *aItem=new TableNumericItem(aText);

    aTable->setItem(row, column, aItem);
    aTable->scrollToItem(aItem);

    ui->progressBar->setValue(ui->progressBar->value()+1);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MainWindow::calculateRate(CopyableTable *aTable, int row)
{
    double aValue1=aTable->item(row, 2)->text().toDouble();
    double aValue2=aTable->item(row, 3)->text().toDouble();

    if (aValue2==0)
    {
        if (aValue1==0)
        {
            setItemAndScroll(aTable, row, 4, QString::number(1, 'f', 3));
        }
        else
        {
            setItemAndScroll(aTable, row, 4, QString::number(aValue1, 'f', 3));
        }
    }
    else
    {
        setItemAndScroll(aTable, row, 4, QString::number(aValue1/aValue2, 'f', 3));
    }

    aTable->item(row, 3)->setFont(QFont("Arial", 12, QFont::Bold));
    aTable->item(row, 4)->setFont(QFont("Arial", 12, QFont::Bold));

    if (aValue2<=aValue1)
    {
        aTable->item(row, 3)->setTextColor(QColor(128, 128, 255));
        aTable->item(row, 4)->setTextColor(QColor(128, 128, 255));
    }
    else
    {
        aTable->item(row, 3)->setTextColor(QColor(255, 128, 128));
        aTable->item(row, 4)->setTextColor(QColor(255, 128, 128));
    }

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MainWindow::setAndCalculate(CopyableTable *aTable, int row, QString aText)
{
    setItemAndScroll(aTable, row, 3, aText);
    calculateRate(aTable, row);
}

template <typename T>
void MainWindow::testList(const QString aElementName)
{
    OptimalList<T> aMyList;
    QList<T> aQtList;
    T aValue;

//  int aElemCount=15000000; // 1GB RAM free required
    int aElemCount=1500000;

    int lastRow=((CopyableTable*)ui->testsTabWidget->widget(0))->rowCount();

    qint64 aStart;
    qint64 aTimeStamp;

    qint64 aMemoryBefore;
    qint64 aMemoryAfter;
    HANDLE aProcess=GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS aMemory;

    //------------------------------------------------------------

    for (int i=0; i<ui->testsTabWidget->count(); ++i)
    {
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setRowCount(lastRow+1);
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setItem(lastRow, 0, new TableNumericItem(QString::number(lastRow+1)));
    }

    ((CopyableTable*)ui->testsTabWidget->widget(0))->setItem(lastRow, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Memory, bytes ("+QString::number(sizeof(T))+" bytes per element)"));
    ((CopyableTable*)ui->testsTabWidget->widget(1))->setItem(lastRow, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for appending, ms"));
    ((CopyableTable*)ui->testsTabWidget->widget(2))->setItem(lastRow, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for clearing, ms"));
    ((CopyableTable*)ui->testsTabWidget->widget(3))->setItem(lastRow, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for inserting, ms"));
    ((CopyableTable*)ui->testsTabWidget->widget(4))->setItem(lastRow, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for cloning, ms"));
    ((CopyableTable*)ui->testsTabWidget->widget(5))->setItem(lastRow, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for removing, ms"));

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



    setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(0)), lastRow, 2, QString::number(aMemoryAfter-aMemoryBefore));
    setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(1)), lastRow, 2, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();
    aQtList.clear();
    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(2)), lastRow, 2, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();

    for (int i=0; i<aElemCount; ++i)
    {
        aQtList.insert(0, aValue);
    }

    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(3)), lastRow, 2, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();
    //useAsArgument<T>(aQtList);
    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(4)), lastRow, 2, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();

    while (aQtList.length()>0)
    {
        aQtList.removeAt(0);
    }

    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(5)), lastRow, 2, QString::number(aTimeStamp-aStart));

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



    setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(0)), lastRow, QString::number(aMemoryAfter-aMemoryBefore));
    setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(1)), lastRow, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();
    aMyList.clear();
    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(2)), lastRow, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();

    for (int i=0; i<aElemCount; ++i)
    {
        aMyList.append(aValue);
    }

    aTimeStamp=QDateTime::currentMSecsSinceEpoch();



    setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(3)), lastRow, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();
    //useAsArgument<T>(aMyList);
    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(4)), lastRow, QString::number(aTimeStamp-aStart));

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();

    while (aMyList.length()>0)
    {
        aMyList.removeAt(0);
    }

    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(5)), lastRow, QString::number(aTimeStamp-aStart));
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);

    ui->testsTabWidget->setCurrentIndex(0);

    disconnect(ui->testsTabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_testsTabWidget_currentChanged(int)));

    for (int i=0; i<ui->testsTabWidget->count(); ++i)
    {
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setSortingEnabled(false);
        ((CopyableTable*)ui->testsTabWidget->widget(i))->clearContents();
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setRowCount(0);
    }

    ui->progressBar->setMaximum(17*3*ui->testsTabWidget->count());
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

    for (int i=0; i<ui->testsTabWidget->count(); ++i)
    {
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setSortingEnabled(true);
        ((CopyableTable*)ui->testsTabWidget->widget(i))->sortByColumn(0, Qt::AscendingOrder);
    }

    ui->testsTabWidget->setCurrentIndex(0);
    mLastTabIndex=0;

    connect(ui->testsTabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_testsTabWidget_currentChanged(int)));

    ui->progressBar->setValue(0);

    ui->startButton->setEnabled(true);
}

void MainWindow::on_testsTabWidget_currentChanged(int index)
{
    CopyableTable *aTable1=(CopyableTable*)ui->testsTabWidget->widget(mLastTabIndex);
    CopyableTable *aTable2=(CopyableTable*)ui->testsTabWidget->widget(index);

    for (int i=0; i<aTable1->columnCount(); ++i)
    {
        aTable2->setColumnWidth(i, aTable1->columnWidth(i));
    }

    aTable2->sortByColumn(aTable1->horizontalHeader()->sortIndicatorSection(), aTable1->horizontalHeader()->sortIndicatorOrder());

    mLastTabIndex=index;
}
