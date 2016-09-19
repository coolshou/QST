#include "commdialog.h"
#include "ui_commdialog.h"
#include "mainwindow.h"

commDialog::commDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::commDialog)
{
    ui->setupUi(this);
    //ui's baudRate depends on getBaudRateStrings()
    ui->cbSpeed->clear();
    //int i;
    QStringList list = MainWindow(parent).getBaudRateStrings();
    QString baudRate;
    foreach (baudRate, list) {
        //qDebug() << "commDialog:" << baudRate;
        ui->cbSpeed->addItem(baudRate);
    }

    connect(ui->cbLogFile,SIGNAL(stateChanged(int)),
            this, SLOT(onLogFileStateChanged(int)));
    connect(ui->btnSelectLogFile,SIGNAL(clicked(bool)),
            this, SLOT(selectLogFileName()));
}

commDialog::~commDialog()
{
    delete ui;
}

void commDialog::setHwFlow(bool checked)
{
    ui->cbHwFlow->setChecked(checked);
}

bool commDialog::getHwFlow()
{
    return ui->cbHwFlow->isChecked();
}
void commDialog::setOpenStart(bool checked)
{
    ui->cbOpenStart->setChecked(checked);
}

bool commDialog::getOpenStart(void)
{
    return ui->cbOpenStart->isChecked();
}

void commDialog::setBaudNdx(int idx)
{
    ui->cbSpeed->setCurrentIndex(idx);
}

int  commDialog::getBaudNdx(void)
{
    return ui->cbSpeed->currentIndex();
}
void commDialog::additemListPort(const QString &text, const QVariant &userData)
{
    ui->cbListPorts->addItem(text, userData);
}

void commDialog::setCurrentPortNdx(int idx)
{
    ui->cbListPorts->setCurrentIndex(idx);
}

int commDialog::getCurrentPortNdx(void)
{
    return ui->cbListPorts->currentIndex();
}
QString commDialog::getPortName(int idx)
{
    return ui->cbListPorts->itemData(idx).toString();
}

int commDialog::finditemListPort(QString text)
{
    return ui->cbListPorts->findData(text);
}
void commDialog::setLogFileEnable(bool checked)
{
    ui->cbLogFile->setChecked(checked);
}

bool commDialog::getLogFileEnable(void)
{
    return ui->cbLogFile->isChecked();
}

void commDialog::setLogFileName(QString filename)
{
    ui->editLogFile->setText(filename);
}
QString commDialog::getLogFileName(void)
{
    return ui->editLogFile->text();
}

void commDialog::setAddTimeStemp(bool checked)
{
    ui->cbAddTimeStemp->setChecked(checked);
}

bool commDialog::getAddTimeStemp(void)
{
    return ui->cbAddTimeStemp->isChecked();
}

void commDialog::setSplitFile(bool checked)
{
    ui->cbSplitFile->setChecked(checked);
}
bool commDialog::getSplitFile(void)
{
    return ui->cbSplitFile->isChecked();
}

void commDialog::setSplitFileSize(int iSize)
{
    ui->sbSplitFileSize->setValue(iSize);
}

int commDialog::getSplitFileSize(void)
{
    return ui->sbSplitFileSize->value();
}


void  commDialog::onLogFileStateChanged(int state)
{
    bool bEnable;
    if (state == Qt::Unchecked){
        bEnable = false;
    } else {
        bEnable = true;
    }
    ui->editLogFile->setEnabled(bEnable);
    ui->btnSelectLogFile->setEnabled(bEnable);
    ui->cbAddTimeStemp->setEnabled(bEnable);
    ui->cbSplitFile->setEnabled(bEnable);
    ui->sbSplitFileSize->setEnabled(bEnable);
    ui->labelFileSize->setEnabled(bEnable);
}
void commDialog::selectLogFileName(void)
{
    QStringList fileNames;
    QStringList mimeTypeFilters;
    mimeTypeFilters << "text/plain"
                << "application/octet-stream"; // will show "All files (*)"

    QFileDialog dialog(this);
    dialog.setWindowTitle("Select log file");
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.setDefaultSuffix("txt");
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        ui->editLogFile->setText(fileNames.at(0));
    }
}
