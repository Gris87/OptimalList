#ifndef COPYABLETABLE_H
#define COPYABLETABLE_H

#include <QTableWidget>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>

class CopyableTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit CopyableTable(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void commitData(QWidget *);
};

#endif // COPYABLETABLE_H
