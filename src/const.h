#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QStringList>
#include <QColor>
#include <QDir>
//#include "qextserialport.h"
#include "qserialport.h"

static bool useEventDriven = true;

static QString APPName = "QST";
static QString DEF_CONFIG_PATH = QDir( QDir::homePath() +
                                QString("/.config/") + APPName +
                                QString("/")).path();
static QString DEF_HistoryFileName =
        QDir(DEF_CONFIG_PATH).filePath("inputHistory.txt");
static QString DEF_LogFileName =
        QDir( QDir::homePath()).filePath(APPName + "_log.txt");
static QString select_logfilter = ("Text (*.TXT *.txt)");
const QString logfilter = "Text (*.TXT *.txt);; All files (*.*)";

/*#include "qextserialport.h"
static int DEF_baudRates[] = { BAUD2400, BAUD4800, BAUD9600,
                               BAUD19200, BAUD38400,
                               BAUD57600, BAUD115200};
*/
static int DEF_baudRates[] = { QSerialPort::Baud2400, QSerialPort::Baud4800,
                               QSerialPort::Baud9600,
                               QSerialPort::Baud19200, QSerialPort::Baud38400,
                               QSerialPort::Baud57600, QSerialPort::Baud115200};

const int Max_BaudRates = sizeof(DEF_baudRates)/sizeof(DEF_baudRates[0]);

//pre-define theme
enum consoleThemes {
    light=0,
    dark,
    custom
};
static QColor DEF_fontColors[] = { Qt::black, Qt::green};
const int Max_fontColors = sizeof(DEF_fontColors)/sizeof(DEF_fontColors[0]);
static QColor DEF_bgColors[] = { Qt::white, Qt::black};
const int Max_bgColors = sizeof(DEF_bgColors)/sizeof(DEF_bgColors[0]);

//default font color
//QStringList DEF_fontColor =(QStringList() << Qt::black << Qt::white);
//QStringList DEF_bgColor =(QStringList() << Qt::white << Qt::black);
//DEF_baudRates;


#endif // CONST_H
