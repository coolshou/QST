#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>


namespace Ui {
class optionDialog;
}

class optionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit optionDialog(QWidget *parent = 0);
    ~optionDialog();
    void setFontColorItemData(int index, QColor color, const QVariant &value, int role);
    void setFontColorDisabled(bool disable);
    void setFontColorCurrentIndex(int index);
    int  getFontColorCurrentIndex(void);
    void setBgColorItemData(int index, QColor color, const QVariant &value, int role);
    void setBgColorDisabled(bool disable);
    void setBgColorCurrentIndex(int index);
    int  getBgColorCurrentIndex(void);
    void setThemeCurrentIndex(int index);
    int  getThemeCurrentIndex(void);
    void setWordWrapChecked(bool checked);
    bool isWordWrapChecked(void);
    void setInputHistoryChecked(bool checked);
    bool isInputHistoryChecked(void);
    void setInputHistoryFilename(QString filename);
    QString getInputHistoryFilename(void);
    void setAutoScrollChecked(bool checked);
    bool isAutoScrollChecked(void);

private slots:
    void changeThemeCombobox(int idx);
    void onInputHistoryChanged(int state);
    void selectInputHistory(void);
    void done(int r);

private:
    Ui::optionDialog *ui;
    QString colorToString(QColor color);
};

#endif // OPTIONDIALOG_H
