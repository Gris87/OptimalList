#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->testsTableWidget->setColumnWidth(0, 400);
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
    qint64 aStart;
    qint64 aTimeStamp;
    int aElemCount=10000000;
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



    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 0, new QTableWidgetItem("Time to fill QList with "+QString::number(aElemCount)+" \""+aElementName+"\" elements"));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 1, new QTableWidgetItem(QString::number(aTimeStamp-aStart)+" ms"));

    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 0, new QTableWidgetItem("Memory usage while QList filled with "+QString::number(aElemCount)+" \""+aElementName+"\" elements"));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(aMemoryAfter-aMemoryBefore)));



    ui->testsTableWidget->scrollToBottom();

    ui->progressBar->setValue(ui->progressBar->value()+1);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    aQtList.clear();

    //------------------------------------------------------------

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

    ui->testsTableWidget->setRowCount(ui->testsTableWidget->rowCount()+2);



    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 0, new QTableWidgetItem("Time to fill OptimalList with "+QString::number(aElemCount)+" \""+aElementName+"\" elements"));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-2, 1, new QTableWidgetItem(QString::number(aTimeStamp-aStart)+" ms"));

    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 0, new QTableWidgetItem("Memory usage while OptimalList filled with "+QString::number(aElemCount)+" \""+aElementName+"\" elements"));
    ui->testsTableWidget->setItem(ui->testsTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(aMemoryAfter-aMemoryBefore)));



    if (ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-4, 1)->text().left(ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-4, 1)->text().length()-3).toInt()>aTimeStamp-aStart)
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 1)->setTextColor(QColor(0, 128, 0));
    }
    else
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 1)->setTextColor(QColor(128, 0, 0));
    }

    if (ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-3, 1)->text().toInt()>aMemoryAfter-aMemoryBefore)
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 1)->setTextColor(QColor(0, 128, 0));
    }
    else
    {
        ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 1)->setTextColor(QColor(128, 0, 0));
    }

    ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-2, 1)->setFont(QFont("Arial", 12, QFont::Bold));
    ui->testsTableWidget->item(ui->testsTableWidget->rowCount()-1, 1)->setFont(QFont("Arial", 12, QFont::Bold));



    ui->testsTableWidget->scrollToBottom();

    ui->progressBar->setValue(ui->progressBar->value()+1);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    aMyList.clear();
}

void MainWindow::on_startButton_clicked()
{
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

    ui->progressBar->setValue(0);
}
