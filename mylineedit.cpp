#include "mylineedit.h"

#include <QStandardPaths>

void MyLineEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString path;
    if(text().isEmpty())
    {
        path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    else
    {
        path = text();
    }

    if(fileMode)
    {
        emit signPath(QFileDialog::getOpenFileName(this, "Select file", path, "*.json"));
    }
    else
    {
        emit signPath(QFileDialog::getOpenFileName(this, "Select file", path, "*.txt"));
    }
}

void MyLineEdit::slotFileMode(int mode)
{
    fileMode = mode;
}
