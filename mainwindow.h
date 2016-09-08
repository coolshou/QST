#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"
#include <QTimer>
#include <QFile>
#include "ui_mainwindow.h"

#include "optiondialog.h"
#include "commdialog.h"
#include "aboutdialog.h"
#include "led.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow,
        private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    //~MainWindow();
    QFile *inputHistoryFile;

public slots:
    void startStopComm(void);
    void aboutQt(void);


private slots:
    void configComm(void);
    void pollSerial(void);
    void onDataAvailable(void);
    void onReadFinished(void);
    void onDsrChanged(bool status);
    void helpAbout(void);
    void configOption(void);
    void sendFile(void);
    void saveScreen(void);
    void startLogging(void);
    void endLogging(void);
    void readSetting(void);
    void saveSetting(void);
    void saveOptionSetting(void);
    void applyOptionSetting(void);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    QextSerialPort *port;
    aboutDialog *aboutDlg;
    commDialog *commDlg;
    optionDialog *optionDlg;

    int baudRates[6];
    QStringList baudRateStrings;
    QTimer timer;
    void updateStatusBar(void);
    QWidgetList sbList;

    // Settings from config file
    int baudNdx;
    bool hwFlow;
    bool openAtStart;
    QString deviceName;
    // Settings for console theme
    int themeIdx;
    int fontColorIdx;
    int bgColorIdx;
    bool wordWrap;
    bool autoScroll;
    bool inputHistory;
    QString inputHistoryFileName;



    QLed *txLed;  // Green left LED
    QLed *rxLed;  // Red right LED
    QFile *logFile;
};

#endif // MAINWINDOW_H
