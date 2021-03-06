#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QThread>

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#else
#include <QFile>
#endif

#include "../optimallist/optimallist.h"
#include "../widgets/tablenumericitem.h"
#include "../widgets/copyabletable.h"
#include "types.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mLastTabIndex=0;

    for (int i=0; i<6; ++i)
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
        aTable->setColumnWidth(2, 135);
        aTable->setColumnWidth(3, 135);
        aTable->setColumnWidth(4, 75);

        ui->testsTabWidget->addTab(aTable, "");
    }

    ui->testsTabWidget->setTabText(0, "Memory usage");
    ui->testsTabWidget->setTabText(1, "Time for appending");
    ui->testsTabWidget->setTabText(2, "Time for clearing");
    ui->testsTabWidget->setTabText(3, "Time for inserting");
    ui->testsTabWidget->setTabText(4, "Time for cloning");
    ui->testsTabWidget->setTabText(5, "Time for removing");

    mRandomSeed=QDateTime::currentMSecsSinceEpoch();
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
    double aRate;

    if (aValue2==0)
    {
        if (aValue1==0)
        {
            aRate=1;
        }
        else
        {
            aRate=aValue1;
        }
    }
    else
    {
        aRate=aValue1/aValue2;
    }

    setItemAndScroll(aTable, row, 4, QString::number(aRate, 'f', 3));

    aTable->item(row, 3)->setFont(QFont("Arial", 12, QFont::Bold));
    aTable->item(row, 4)->setFont(QFont("Arial", 12, QFont::Bold));

    if (qAbs(aRate-1)<0.100)
    {
        aTable->item(row, 3)->setTextColor(QColor(128, 128, 255));
        aTable->item(row, 4)->setTextColor(QColor(128, 128, 255));
    }
    else
    if (aValue2<=aValue1)
    {
        aTable->item(row, 3)->setTextColor(QColor(96, 190, 96));
        aTable->item(row, 4)->setTextColor(QColor(96, 190, 96));
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

qint64 memoryUsage()
{
#ifdef Q_OS_WIN
    HANDLE aProcess=GetCurrentProcess();

    PROCESS_MEMORY_COUNTERS aMemory;
    aMemory.cb=sizeof(PROCESS_MEMORY_COUNTERS);
    GetProcessMemoryInfo(aProcess, &aMemory, aMemory.cb);

    return aMemory.PagefileUsage;
#else
    QFile aFile("/proc/self/status");
    aFile.open(QIODevice::ReadOnly);

    QString aStat;

    do
    {
        QString aLine=QString::fromUtf8(aFile.readLine());

        if (aLine.startsWith("VmData:"))
        {
            aStat=aLine.mid(7).trimmed();
            aStat.remove(aStat.length()-3, 3);
            break;
        }

        if (aLine=="")
        {
            break;
        }
    } while (true);

    aFile.close();

    return aStat.toLongLong()*1024;
#endif
}

qint64 memoryDiff(const qint64 aMemoryBefore)
{
    int aAttempt=0;

    while (aAttempt<10)
    {
        ++aAttempt;

        qint64 aMemoryUsed=memoryUsage();

        if (aMemoryUsed>aMemoryBefore)
        {
            return aMemoryUsed-aMemoryBefore;
        }

        QThread::currentThread()->msleep(100);
    }

    return 0;
}

template <typename T>
void MainWindow::testList(const QString aElementName)
{
    T aValue;

//  int aElemCount=15000000; // 1GB RAM free required
    int aElemCount=15000000;

    int lastRow=((CopyableTable*)ui->testsTabWidget->widget(0))->rowCount();

    qint64 aStart;
    qint64 aTimeStamp;

    qint64 aMemoryUsed;

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

    //============================================================
    //                           QT LIST
    //============================================================
    {
        QList<T> aQtList;

        aMemoryUsed=memoryUsage();
        aStart=QDateTime::currentMSecsSinceEpoch();

        for (int i=0; i<aElemCount; ++i)
        {
            aQtList.append(aValue);
        }

        aTimeStamp=QDateTime::currentMSecsSinceEpoch();
        aMemoryUsed=memoryDiff(aMemoryUsed);



        setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(0)), lastRow, 2, QString::number(aMemoryUsed));
        setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(1)), lastRow, 2, QString::number(aTimeStamp-aStart));

        //------------------------------------------------------------

        aStart=QDateTime::currentMSecsSinceEpoch();
        aQtList.clear();
        aTimeStamp=QDateTime::currentMSecsSinceEpoch();

        setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(2)), lastRow, 2, QString::number(aTimeStamp-aStart));

        //------------------------------------------------------------

        srand(mRandomSeed);

        aStart=QDateTime::currentMSecsSinceEpoch();

        for (int i=0; i<aElemCount/100; ++i)
        {
            aQtList.insert(rand() % (aQtList.length()+1), aValue);
        }

        aTimeStamp=QDateTime::currentMSecsSinceEpoch();

        setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(3)), lastRow, 2, QString::number((aTimeStamp-aStart)*100));

        //------------------------------------------------------------

        aStart=QDateTime::currentMSecsSinceEpoch();
        QList<T> aTempList=aQtList;
        aTempList.detach();
        aTimeStamp=QDateTime::currentMSecsSinceEpoch();

        aTempList.clear();

        setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(4)), lastRow, 2, QString::number(aTimeStamp-aStart));

        //------------------------------------------------------------

        srand(mRandomSeed);

        aStart=QDateTime::currentMSecsSinceEpoch();

        while (aQtList.length()>0)
        {
            aQtList.removeAt(rand() % aQtList.length());
        }

        aTimeStamp=QDateTime::currentMSecsSinceEpoch();

        setItemAndScroll(((CopyableTable*)ui->testsTabWidget->widget(5)), lastRow, 2, QString::number((aTimeStamp-aStart)*100));
    }

    //============================================================
    //                        OPTIMAL LIST
    //============================================================
    {
        OptimalList<T> aMyList;

        aMemoryUsed=memoryUsage();
        aStart=QDateTime::currentMSecsSinceEpoch();

        for (int i=0; i<aElemCount; ++i)
        {
            aMyList.append(aValue);
        }

        aTimeStamp=QDateTime::currentMSecsSinceEpoch();
        aMemoryUsed=memoryDiff(aMemoryUsed);



        setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(0)), lastRow, QString::number(aMemoryUsed));
        setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(1)), lastRow, QString::number(aTimeStamp-aStart));

        //------------------------------------------------------------

        aStart=QDateTime::currentMSecsSinceEpoch();
        aMyList.clear();
        aTimeStamp=QDateTime::currentMSecsSinceEpoch();

        setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(2)), lastRow, QString::number(aTimeStamp-aStart));

        //------------------------------------------------------------

        srand(mRandomSeed);

        aStart=QDateTime::currentMSecsSinceEpoch();

        for (int i=0; i<aElemCount/100; ++i)
        {
            aMyList.insert(rand() % (aMyList.length()+1), aValue);
        }

        aTimeStamp=QDateTime::currentMSecsSinceEpoch();



        setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(3)), lastRow, QString::number((aTimeStamp-aStart)*100));

        //------------------------------------------------------------

        aStart=QDateTime::currentMSecsSinceEpoch();
        OptimalList<T> aTempList2=aMyList;
        aTimeStamp=QDateTime::currentMSecsSinceEpoch();

        aTempList2.clear();

        setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(4)), lastRow, QString::number(aTimeStamp-aStart));

        //------------------------------------------------------------

        srand(mRandomSeed);

        aStart=QDateTime::currentMSecsSinceEpoch();

        while (aMyList.length()>0)
        {
            aMyList.removeAt(rand() % aMyList.length());
        }

        aTimeStamp=QDateTime::currentMSecsSinceEpoch();

        setAndCalculate(((CopyableTable*)ui->testsTabWidget->widget(5)), lastRow, QString::number((aTimeStamp-aStart)*100));
    }
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);

    ui->testsTabWidget->setCurrentIndex(0);
    ui->testsTabWidget->blockSignals(true);

    for (int i=0; i<ui->testsTabWidget->count(); ++i)
    {
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setSortingEnabled(false);
        ((CopyableTable*)ui->testsTabWidget->widget(i))->clearContents();
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setRowCount(0);
    }

    ui->progressBar->setMaximum(21*3*ui->testsTabWidget->count());
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
    testList<sTinyStruct>("sTinyStruct");
    testList<sSmallStruct>("sSmallStruct");
    testList<sMediumStruct>("sMediumStruct");
    testList<sLargeStruct>("sLargeStruct");
    testList<sHugeStruct>("sHugeStruct");
    testList<QString>("QString");

    for (int i=0; i<ui->testsTabWidget->count(); ++i)
    {
        ((CopyableTable*)ui->testsTabWidget->widget(i))->setSortingEnabled(true);
        ((CopyableTable*)ui->testsTabWidget->widget(i))->sortByColumn(0, Qt::AscendingOrder);
    }

    ui->testsTabWidget->setCurrentIndex(0);
    mLastTabIndex=0;

    ui->testsTabWidget->blockSignals(false);

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
