#ifndef COMMDIALOG_H
#define COMMDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QFileDialog>

namespace Ui {
class commDialog;
}

class commDialog : public QDialog
{
    Q_OBJECT

public:
    explicit commDialog(QWidget *parent = 0);
    ~commDialog();
    void setHwFlow(bool checked);
    bool getHwFlow(void);
    void setOpenStart(bool checked);
    bool getOpenStart(void);
    void setBaudNdx(int idx);
    int  getBaudNdx(void);
    void additemListPort(const QString &text, const QVariant &userData );
    void setCurrentPortNdx(int idx);
    int getCurrentPortNdx(void);
    QString getPortName(int idx);
    int finditemListPort(QString text);

    void setLogFileEnable(bool checked);
    bool getLogFileEnable(void);
    void setLogFileName(QString filename);
    QString getLogFileName(void);
    void setAddTimeStemp(bool checked);
    bool getAddTimeStemp(void);
    void setSplitFile(bool checked);
    bool getSplitFile(void);
    void setSplitFileSize(int iSize);
    int getSplitFileSize(void);

private slots:
    void onLogFileStateChanged(int state);
    void selectLogFileName(void);

private:
    Ui::commDialog *ui;
};

#endif // COMMDIALOG_H
