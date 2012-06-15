#ifndef TABLENUMERICITEM_H
#define TABLENUMERICITEM_H

#include <QTableWidgetItem>

class TableNumericItem : public QTableWidgetItem
{
public:
    TableNumericItem(int type = Type);
    TableNumericItem(const QString &text, int type = Type);
    TableNumericItem(const QIcon &icon, const QString &text, int type = Type);
    TableNumericItem(const QTableWidgetItem &other);

    bool operator<(const QTableWidgetItem &other) const;
};

#endif // TABLENUMERICITEM_H
