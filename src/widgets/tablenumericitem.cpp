#include "tablenumericitem.h"

TableNumericItem::TableNumericItem(int type) :
        QTableWidgetItem(type)
{
}

TableNumericItem::TableNumericItem(const QString &text, int type) :
        QTableWidgetItem(text, type)
{
}

TableNumericItem::TableNumericItem(const QIcon &icon, const QString &text, int type) :
        QTableWidgetItem(icon, text, type)
{
}

TableNumericItem::TableNumericItem(const QTableWidgetItem &other) :
        QTableWidgetItem(other)
{
}

bool TableNumericItem::operator<(const QTableWidgetItem &other) const
{
    bool ok1;

    double num1=text().toDouble(&ok1);

    if (ok1)
    {
        bool ok2;

        double num2=other.text().toDouble(&ok2);

        if (ok2)
        {
            return num1<num2;
        }
        else
        {
            return QTableWidgetItem::operator <(other);
        }
    }
    else
    {
        return QTableWidgetItem::operator <(other);
    }
}
