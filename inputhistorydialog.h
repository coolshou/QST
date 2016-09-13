#ifndef INPUTHISTORYDIALOG_H
#define INPUTHISTORYDIALOG_H

#include <QDialog>
#include <QFile>

namespace Ui {
class InputHistoryDialog;
}

class InputHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputHistoryDialog(QWidget *parent = 0);
    ~InputHistoryDialog();
    void setInputHistoryFileName(QString filename);
    QString getInputHistoryFileName(void);

private:
    Ui::InputHistoryDialog *ui;
    QString inputHistoryFileName;
};

#endif // INPUTHISTORYDIALOG_H
