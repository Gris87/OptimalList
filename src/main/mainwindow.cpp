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

template <typename T>
void MainWindow::testList(const QString aElementName)
{
    OptimalList<T> aMyList;
    QList<T> aQtList;
    T aValue;

    //int aElemCount=30000000; // 1GB RAM free required
    int aElemCount=3000000;

    int lastRow=ui->testsTableWidget->rowCount();

    qint64 aStart;
    qint64 aTimeStamp;

    qint64 aMemoryBefore;
    qint64 aMemoryAfter;
    HANDLE aProcess=GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS aMemory;

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



    ui->testsTableWidget->setRowCount(ui->testsTableWidget->rowCount()+2);

    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 0, new TableNumericItem(QString::number(ui->testsTableWidget->rowCount()-1)));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 0, new TableNumericItem(QString::number(ui->testsTableWidget->rowCount())));

    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Memory, bytes ("+QString::number(sizeof(T))+" bytes per element)"));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 2, new TableNumericItem(QString::number(aMemoryAfter-aMemoryBefore)));

    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for appending, ms"));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 2, new TableNumericItem(QString::number(aTimeStamp-aStart)));



    ui->testsTableWidget->scrollToBottom();

    ui->progressBar->setValue(ui->progressBar->value()+1);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    //------------------------------------------------------------

    aStart=QDateTime::currentMSecsSinceEpoch();

    aQtList.clear();

    aTimeStamp=QDateTime::currentMSecsSinceEpoch();

    ui->testsTableWidget->setRowCount(ui->testsTableWidget->rowCount()+1);

    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 0, new TableNumericItem(QString::number(ui->testsTableWidget->rowCount())));

    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(aElemCount)+" \""+aElementName+"\": Time for clearing, ms"));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 2, new TableNumericItem(QString::number(aTimeStamp-aStart)));



    ui->testsTableWidget->scrollToBottom();

    ui->progressBar->setValue(ui->progressBar->value()+1);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

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



    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 3, new TableNumericItem(QString::number(aMemoryAfter-aMemoryBefore)));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 3, new TableNumericItem(QString::number(aTimeStamp-aStart)));

    if (aMemoryAfter-aMemoryBefore!=0)
    {
        ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 4, new TableNumericItem(QString::number(ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 2)->text().toInt()/((float)(aMemoryAfter-aMemoryBefore)), 'f', 3)));
    }

    if (aTimeStamp-aStart!=0)
    {
        ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 4, new TableNumericItem(QString::number(ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 2)->text().toInt()/((float)(aTimeStamp-aStart)), 'f', 3)));
    }



    if (ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 2)->text().toInt()>aMemoryAfter-aMemoryBefore)
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 3)->setTextColor(QColor(128, 128, 255));
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 4)->setTextColor(QColor(128, 128, 255));
    }
    else
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 3)->setTextColor(QColor(255, 128, 128));
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 4)->setTextColor(QColor(255, 128, 128));
    }

    if (ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 2)->text().toInt()>aTimeStamp-aStart)
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 3)->setTextColor(QColor(0, 128, 0));
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 4)->setTextColor(QColor(0, 128, 0));
    }
    else
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 3)->setTextColor(QColor(128, 0, 0));
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 4)->setTextColor(QColor(128, 0, 0));
    }



    ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 3)->setFont(QFont("Arial", 12, QFont::Bold));
    ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 4)->setFont(QFont("Arial", 12, QFont::Bold));
    ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 3)->setFont(QFont("Arial", 12, QFont::Bold));
    ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 4)->setFont(QFont("Arial", 12, QFont::Bold));



    ui->progressBar->setValue(ui->progressBar->value()+1);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    aMyList.clear();
}

void MainWindow::on_startButton_clicked()
{
    ui->testsTableWidget->setSortingEnabled(false);
    ui->testsTableWidget->clearContents();
    ui->testsTableWidget->setRowCount(0);

    ui->progressBar->setMaximum(17*2);
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
