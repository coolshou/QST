#include "optiondialog.h"
#include "ui_optiondialog.h"
#include "const.h"

optionDialog::optionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionDialog)
{
    ui->setupUi(this);
    connect(ui->cbTheme, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeThemeCombobox(int)));
    connect(ui->cbInputHistory, SIGNAL(stateChanged(int)),
            this, SLOT(onInputHistoryChanged(int)));
    connect(ui->pbSelectHistory, SIGNAL(clicked()),
            this, SLOT(selectInputHistory()));
}

optionDialog::~optionDialog()
{
    delete ui;
}

void optionDialog::setFontColorItemData(int index, const QVariant &value, int role)
{
    ui->cbFontColor->setItemData(index, value, role);
}

void optionDialog::setFontColorDisabled(bool disable)
{
    ui->cbFontColor->setDisabled(disable);
}

void optionDialog::setFontColorCurrentIndex(int index)
{
    ui->cbFontColor->setCurrentIndex(index);
}

int optionDialog::getFontColorCurrentIndex(void)
{
    return ui->cbFontColor->currentIndex();
}
void optionDialog::setBgColorItemData(int index, const QVariant &value, int role)
{
    ui->cbBgColor->setItemData(index, value, role);
}

void optionDialog::setBgColorDisabled(bool disable)
{
    ui->cbBgColor->setDisabled(disable);
}

void optionDialog::setBgColorCurrentIndex(int index)
{
    ui->cbBgColor->setCurrentIndex(index);
}

int  optionDialog::getBgColorCurrentIndex(void)
{
    return ui->cbBgColor->currentIndex();
}
void optionDialog::setThemeCurrentIndex(int index)
{
    ui->cbTheme->setCurrentIndex(index);
}

int  optionDialog::getThemeCurrentIndex(void)
{
    return ui->cbTheme->currentIndex();
}

void optionDialog::setWordWrapChecked(bool checked)
{
    ui->cbWordWrap->setChecked(checked);
}
bool optionDialog::isWordWrapChecked(void)
{
    return ui->cbWordWrap->isChecked();
}

void optionDialog::setInputHistoryChecked(bool checked)
{
    ui->cbInputHistory->setChecked(checked);
    /*
    Qt::CheckState state;
    if (checked) {
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    ui->cbInputHistory->setCheckState(state);
    */
}
bool optionDialog::isInputHistoryChecked(void)
{
    return ui->cbInputHistory->isChecked();
}
void optionDialog::setInputHistoryFilename(QString filename)
{
    ui->editInputHistory->setText(filename);
}

QString optionDialog::getInputHistoryFilename(void)
{
    return ui->editInputHistory->text();
}

void optionDialog::setAutoScrollChecked(bool checked)
{
    ui->cbAutoScroll->setChecked(checked);
}

bool optionDialog::isAutoScrollChecked(void)
{
    return ui->cbAutoScroll->isChecked();
}


void optionDialog::changeThemeCombobox(int idx)
{
    qDebug() << "idx:" <<idx;
    if (idx < 0) {
        return;
    }
    if (idx > 1) {
        //custom theme
        //TODO
        ui->cbFontColor->setDisabled(false);
        ui->cbBgColor->setDisabled(false);
    } else {

        ui->cbFontColor->setCurrentIndex(idx);
        ui->cbBgColor->setCurrentIndex(idx);
        ui->cbFontColor->setDisabled(true);
        ui->cbBgColor->setDisabled(true);
    }
}
void optionDialog::onInputHistoryChanged(int state)
{
    bool bEnable;
    if (state == Qt::Unchecked){
        bEnable = false;
    } else {
        bEnable = true;
    }
    ui->labelInputHistory->setEnabled(bEnable);
    ui->editInputHistory->setEnabled(bEnable);
    ui->pbSelectHistory->setEnabled(bEnable);
}
void optionDialog::selectInputHistory(void)
{
    QStringList fileNames;
    QStringList mimeTypeFilters;
    //"image/jpeg" // will show "JPEG image (*.jpeg *.jpg *.jpe)
    mimeTypeFilters << "text/plain"
                << "application/octet-stream"; // will show "All files (*)"

    QFileDialog dialog(this);
    dialog.setWindowTitle("select input history file");
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.setDefaultSuffix("txt");
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        ui->editInputHistory->setText(fileNames.at(0));
    }
}
void optionDialog::done(int r)
{
    if(QDialog::Accepted == r)  // ok was pressed
    {
        if(ui->cbInputHistory->isChecked())   // validate the data somehow
        {
            //must specify InputHistoryFilename
            qDebug() << "edit:" << ui->editInputHistory->text() << ":" << ui->editInputHistory->text().size();
            if (ui->editInputHistory->text().trimmed().isEmpty()) {
                qDebug() << "must specify InputHistoryFilename";
                ui->editInputHistory->setFocus();
            }
            //QDialog::done(r);
            //return;
        } else {
            QDialog::done(r);
        }
    }
    else    // cancel, close or exc was pressed
    {
        QDialog::done(r);
        return;
    }
}
