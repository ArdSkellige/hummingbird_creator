#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QFileDialog>
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
private:
    size_t fileMode = 0;
public:
    MyLineEdit(const QString &contents = "", QWidget *parent = nullptr) : QLineEdit(contents, parent) {}

    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

public slots:
    void slotFileMode(int mode);
signals:
    void signPath(QString path);
};

#endif // MYLINEEDIT_H
