#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    const char *qt_version = qVersion(); //runtime Qt version
    ui->setupUi(this);
    ui->label_version->setText("Ver: " + QString(VERSION));
    ui->label_QtVersion->setText("Using <a href=\"https://www.qt.io/\">Qt</a> " +
                                 QString(QT_VERSION_STR) +
                                 " (Runtime: "+ QString(qt_version) + ")");
}

aboutDialog::~aboutDialog()
{
    delete ui;
}
