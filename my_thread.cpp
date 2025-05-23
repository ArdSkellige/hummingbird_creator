#include "my_thread.h"

#include <QWidget>

Worker_t::Worker_t(QString _fileName, QString _fileMode, uint8_t _mask, bool _isCheckedModify, bool _isCheckedDelete, QMap<QString, uint8_t>* _mapFileNamesP)
    : fileName(_fileName), fileMode(_fileMode), mask(_mask), isCheckedModify(_isCheckedModify), isCheckedDelete(_isCheckedDelete)
{
    mapFileNamesP = _mapFileNamesP;
}

Worker_t::~Worker_t()
{
    mapFileNamesP = nullptr;
}

void Worker_t::slotModifyFile()
{
    qDebug() << "THREAD 2";
    QByteArray bAr;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite))
    {
        bAr = file.readAll();
        size_t size = bAr.size();
        uint16_t modifyProgress = 0;
        for(size_t i = 0; i < size; i++)
        {
            bAr[i] = bAr[i] ^ mask;
            modifyProgress = i / static_cast<double>(size) * 100;
            emit signalSendProgress(modifyProgress);
        }

        modifyProgress = UINT16_MAX;
        emit signalSendProgress(modifyProgress);
        // timerResetProgressP->start();
        emit signalResetProgress();

        if(isCheckedModify)
        {
            file.close();

            QString fileName = file.fileName();
            auto iter = mapFileNamesP->find(fileName);
            if(iter != mapFileNamesP->end()) // such file already exists
            {
                iter.value() = iter.value() + 1;
            }
            else
            {
                mapFileNamesP->insert(fileName, 2);
                iter = mapFileNamesP->find(fileName);
                qDebug() << "iter.value() = " << iter.value();
            }

            for(size_t i = 0; i < 4; i++)
            {
                fileName.removeLast();
            }
            fileName.append(QString::number(iter.value()) + fileMode);
            file.setFileName(fileName);
            file.open(QIODevice::ReadWrite);
        }

        if(isCheckedDelete)
        {
            file.remove();
            QFile fileNew(QFileDialog::getSaveFileName(this, "Name file", "test", "*.txt;; *.bin")); // save changed byteAr in file
            if(fileNew.open(QIODevice::ReadWrite))
            {
                fileNew.resize(0);
                fileNew.write(bAr);
            }
            fileNew.close();
        }
        else
        {
            file.resize(0);
            file.write(bAr);
            file.close();

            // btnModifyFileP->setStyleSheet("background-color: YellowGreen");
            // timerModifyStatusP->start();
            emit signalIsSuccess(true);
        }
    }
    else
    {
        // btnModifyFileP->setStyleSheet("background-color: Crimson");
        // timerModifyStatusP->start();
        emit signalIsSuccess(false);
    }
}
