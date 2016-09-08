#include "commdialog.h"
#include "ui_commdialog.h"

commDialog::commDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::commDialog)
{
    ui->setupUi(this);
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
