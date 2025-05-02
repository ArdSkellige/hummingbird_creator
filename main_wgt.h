#ifndef MAIN_WGT_H
#define MAIN_WGT_H

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QMap>
#include <QPushButton>
#include <QProgressBar>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "mylineedit.h"

class Main_Wgt : public QWidget
{
    Q_OBJECT
private:
    uint8_t mask = 0;
    QMap<QString, uint8_t> mapFileNames;
    QTimer *timerFindFileP = new QTimer(this);
    QTimer *timerModifyStatusP = new QTimer(this);
    QTimer *timerModifyProgressP = new QTimer(this);
    QTimer *timerResetProgressP = new QTimer(this);
    uint16_t modifyProgress = 0;
protected:
    // layout 1:
    QLabel* lblMaskP;
    QComboBox* cmbboxFileMaskP;
    // layout 2:
    QLabel* lblDeleteFileP;
    QCheckBox* cbxDeleteFileP;
    QLabel* lblModifyFileNameP;
    QCheckBox* cbxModifyFileNameP;
    QLabel* lblStartTimerP;
    QCheckBox* cbxStartTimerP;
    // layout 3:
    QLabel* lblMaskTitleP;
    QLineEdit* ledMaskValueP;
    QPushButton* btnConfirmMaskP;
    // layout 4:
    QLabel* lblChooseFileP;
    MyLineEdit* myLineEditP;
    QPushButton* btnModifyFileP;
    // layout 5:
    QProgressBar* progressBarP;

public:
    Main_Wgt(QWidget* parent = nullptr);
    ~Main_Wgt();

    QLabel* createQLabel(const char *str = "");
    QComboBox* createQComboBox();
    QPushButton* createPushButton(const char* str = "");
    QLineEdit* createQLineEdit();

public slots:
    void slotSetFilePath(QString path);
    void slotCheckRange(QString id);
    void slotWriteMask();
    void slotModifyFile();
    void slotTimerControl();
    void slotColorFile();
    void slotProgress();
    void slotResetProgressBar();
};
#endif // MAIN_WGT_H
