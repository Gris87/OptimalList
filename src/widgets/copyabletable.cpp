#include "copyabletable.h"

CopyableTable::CopyableTable(QWidget *parent) :
        QTableWidget(parent)
{
}

void CopyableTable::keyPressEvent(QKeyEvent *event)
{
    if (event==QKeySequence::Copy)
    {
        QList<QTableWidgetSelectionRange> aRanges=selectedRanges();

        if (aRanges.length()>0)
        {
            int aLeft=aRanges.at(0).leftColumn();
            int aTop=aRanges.at(0).topRow();
            int aRight=aRanges.at(0).rightColumn();
            int aBottom=aRanges.at(0).bottomRow();

            QString toClipBoard="";

            for (int i=aTop; i<=aBottom; i++)
            {
                for (int j=aLeft; j<=aRight; j++)
                {
                    if (!isColumnHidden(j))
                    {
                        toClipBoard.append(item(i,j)->text());

                        toClipBoard.append("\t");
                    }
                }

                if (toClipBoard.endsWith("\t"))
                {
                    toClipBoard.remove(toClipBoard.length()-1, 1);
                }

                if (i<aBottom)
                {
                    toClipBoard.append("\n");
                }
            }

            while (toClipBoard.endsWith("\n"))
            {
                toClipBoard.remove(toClipBoard.length()-1, 1);
            }

            QApplication::clipboard()->setText(toClipBoard);
        }
    }
    else
    {
        QTableWidget::keyPressEvent(event);
    }
}

void CopyableTable::commitData(QWidget *)
{
    // Nothing
}
