#include "inputhistorydialog.h"
#include "ui_inputhistorydialog.h"

InputHistoryDialog::InputHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputHistoryDialog)
{
    ui->setupUi(this);
    if (! inputHistoryFileName.isEmpty()) {

    }
    //monitor file change?
    //ui->teInputHistory->
}

InputHistoryDialog::~InputHistoryDialog()
{
    delete ui;
}
void InputHistoryDialog::setInputHistoryFileName(QString filename)
{
    inputHistoryFileName = filename;
}

QString InputHistoryDialog::getInputHistoryFileName(void)
{
    return inputHistoryFileName;
}
