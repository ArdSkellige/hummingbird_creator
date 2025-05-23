#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <QFile>
#include <QFileDialog>
#include <QThread>

class Worker_t : public QWidget
{
    Q_OBJECT
private:
    QString fileName;
    QString fileMode;
    QMap<QString, uint8_t>* mapFileNamesP = nullptr;
    uint8_t mask = 0;
    bool isCheckedModify;
    bool isCheckedDelete;
public:
    Worker_t(QString _fileName, QString _fileMode, uint8_t _mask, bool _isCheckedModify, bool _isCheckedDelete, QMap<QString, uint8_t>* _mapFileNamesP);
    ~Worker_t();

signals:
    void signalSendProgress(uint16_t);
    void signalResetProgress();
    void signalIsSuccess(bool);
public slots:
    void slotModifyFile();
};

#endif // MY_THREAD_H
