#include "main_wgt.h"

#include <QVBoxLayout>

Main_Wgt::Main_Wgt(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Hummingbird");
    this->setMinimumSize(400, 300);

    // create Main layout:
    auto* vblayMainP = new QVBoxLayout(this);

    {// layout 1:
        auto* hblayP = new QHBoxLayout(this);
        lblMaskP = createQLabel("File Mask");
        cmbboxFileMaskP = createQComboBox();

        hblayP->addWidget(lblMaskP);
        hblayP->addWidget(cmbboxFileMaskP);
        vblayMainP->addLayout(hblayP);
    }
    {// layout 2:
        auto* hblayP = new QHBoxLayout(this);
        lblDeleteFileP = createQLabel("Delete file");
        cbxDeleteFileP = new QCheckBox();
        cbxDeleteFileP->setChecked(false);
        lblModifyFileNameP = createQLabel("Modify file Name");
        cbxModifyFileNameP = new QCheckBox();
        cbxModifyFileNameP->setChecked(false);
        lblStartTimerP = createQLabel("Start timer");
        cbxStartTimerP = new QCheckBox();
        cbxStartTimerP->setChecked(false);

        hblayP->addWidget(lblDeleteFileP);
        hblayP->addWidget(cbxDeleteFileP);
        hblayP->addWidget(lblModifyFileNameP);
        hblayP->addWidget(cbxModifyFileNameP);
        hblayP->addWidget(lblStartTimerP);
        hblayP->addWidget(cbxStartTimerP);
        vblayMainP->addLayout(hblayP);
    }
    {// layout 3:
        auto* hblayP = new QHBoxLayout(this);
        lblMaskTitleP = createQLabel("Input Mask");;
        ledMaskValueP = createQLineEdit();
        btnConfirmMaskP = createPushButton("Confirm");

        hblayP->addWidget(lblMaskTitleP);
        hblayP->addWidget(ledMaskValueP);
        hblayP->addWidget(btnConfirmMaskP);
        vblayMainP->addLayout(hblayP);
    }
    {// layout 4:
        auto* hblayP = new QHBoxLayout(this);
        lblChooseFileP = createQLabel("File");
        myLineEditP = new MyLineEdit;
        btnModifyFileP = createPushButton("Modify");

        hblayP->addWidget(lblChooseFileP);
        hblayP->addWidget(myLineEditP);
        hblayP->addWidget(btnModifyFileP);
        vblayMainP->addLayout(hblayP);
    }
    {// layout 5:
        auto* hblayP = new QHBoxLayout(this);
        progressBarP = new QProgressBar;
        progressBarP->setStyleSheet("QProgressBar{border: 1px solid transparent; text-align: center;"
            "color:rgba(235, 235, 230, 1);"
            "border-radius: 1px;"
            "border-width: 2px;"
            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(150, 150, 150, 1), stop:1 rgba(50, 50, 50, 1));}"
            "QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(49, 59, 49, 1), stop: 1 rgba(61, 173, 61, 1));}");

        hblayP->addWidget(progressBarP);
        vblayMainP->addLayout(hblayP);
    }
    vblayMainP->addStretch();

    connect(cmbboxFileMaskP, &QComboBox::activated, myLineEditP, &MyLineEdit::slotFileMode);
    connect(cbxStartTimerP, &QCheckBox::checkStateChanged, this, &Main_Wgt::slotTimerControl);
    connect(myLineEditP, &MyLineEdit::signPath, this, &Main_Wgt::slotSetFilePath);
    connect(ledMaskValueP, &QLineEdit::textChanged, this, &Main_Wgt::slotCheckRange);
    connect(btnConfirmMaskP, &QPushButton::clicked, this, &Main_Wgt::slotWriteMask);
    connect(btnModifyFileP, &QPushButton::clicked, this, &Main_Wgt::slotModifyFile);

    timerFindFileP->setInterval(8000);
    connect(timerFindFileP, &QTimer::timeout, this, &Main_Wgt::slotModifyFile);
    timerModifyStatusP->setInterval(2000);
    connect(timerModifyStatusP, &QTimer::timeout, this, &Main_Wgt::slotColorFile);
    timerModifyProgressP->setInterval(20);
    connect(timerModifyProgressP, &QTimer::timeout, this, &Main_Wgt::slotProgress);
    timerModifyProgressP->setSingleShot(false);
    timerModifyProgressP->start();
    timerResetProgressP->setInterval(2000);
    connect(timerResetProgressP, &QTimer::timeout, this, &Main_Wgt::slotResetProgressBar);

    progressBarP->setMaximum(UINT16_MAX);
}

Main_Wgt::~Main_Wgt()
{
}

QLabel* Main_Wgt::createQLabel(const char *str)
{
    QLabel* labelP = new QLabel(str);
    labelP->setAlignment(Qt::AlignCenter);
    labelP->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    return labelP;
}

QComboBox* Main_Wgt::createQComboBox()
{
    QComboBox* tmpP = new QComboBox;
    tmpP->addItem(".txt");
    tmpP->addItem(".bin");
    return tmpP;
}

QPushButton* Main_Wgt::createPushButton(const char* str)
{
    QPushButton* btnTmpP = new QPushButton(str);
    btnTmpP->setStyleSheet("background-color: Gainsboro");
    return btnTmpP;
}

QLineEdit* Main_Wgt::createQLineEdit()
{
    QLineEdit* tmpLEd = new QLineEdit();
    QIntValidator* valid = new QIntValidator(0, 255);
    tmpLEd->setValidator(valid);
    return tmpLEd;
}

void Main_Wgt::slotSetFilePath(QString path)
{
    if(!path.isEmpty())
    {
        myLineEditP->setText(path);
    }
}

void Main_Wgt::slotCheckRange(QString id)
{
    QVariant variant(id);
    int num = variant.toInt();
    if(num > 255)
    {
        num = 255;
        ledMaskValueP->setText(QString::number(num));
    }
}

void Main_Wgt::slotWriteMask()
{
    mask = ledMaskValueP->text().toUInt();
}

void Main_Wgt::slotModifyFile()
{
    QByteArray bAr;
    QFile file(myLineEditP->text());
    if(file.open(QIODevice::ReadWrite))
    {
        bAr = file.readAll();
        size_t size = bAr.size();
        for(size_t i = 0; i < size; i++)
        {
            bAr[i] = bAr[i] ^ mask;
            modifyProgress = size / 100 * i;
        }
        modifyProgress = UINT16_MAX;
        timerResetProgressP->start();

        if(cbxModifyFileNameP->isChecked())
        {
            file.close();

            QString fileName = file.fileName();
            auto iter = mapFileNames.find(fileName);
            if(iter != mapFileNames.end()) // such file already exists
            {
                iter.value() = iter.value() + 1;
            }
            else
            {
                mapFileNames.insert(fileName, 2);
                iter = mapFileNames.find(fileName);
                qDebug() << "iter.value() = " << iter.value();
            }

            for(size_t i = 0; i < 4; i++)
            {
                fileName.removeLast();
            }
            fileName.append(QString::number(iter.value()) + cmbboxFileMaskP->currentText());
            file.setFileName(fileName);
            file.open(QIODevice::ReadWrite);
        }

        if(cbxDeleteFileP->isChecked())
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

            btnModifyFileP->setStyleSheet("background-color: YellowGreen");
            timerModifyStatusP->start();
        }
    }
    else
    {
        btnModifyFileP->setStyleSheet("background-color: Crimson");
        timerModifyStatusP->start();
    }
}

void Main_Wgt::slotTimerControl()
{
    if(cbxStartTimerP->isChecked())
    {
        timerFindFileP->start();
    }
    else
    {
        timerFindFileP->stop();
    }
}

void Main_Wgt::slotColorFile()
{
    btnModifyFileP->setText("Modify");
    btnModifyFileP->setStyleSheet("background-color: Gainsboro");
    timerModifyStatusP->stop();
}

void Main_Wgt::slotProgress()
{
    progressBarP->setValue(modifyProgress);
}

void Main_Wgt::slotResetProgressBar()
{
    modifyProgress = 0;
    timerResetProgressP->stop();
}
