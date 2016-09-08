#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QStringList>
#include <QColor>
#include <QDir>

static bool useEventDriven = true;

static QString APPName = "QST";
static QString DEF_CONFIG_PATH = QDir( QDir::homePath() +
                                QString("/.config/") + APPName + QString("/")).path();
static QString DEF_HistoryFileName = QDir(DEF_CONFIG_PATH).filePath("inputHistory.txt");

static QString select_logfilter = ("Text (*.TXT *.txt)");
const QString logfilter = "Text (*.TXT *.txt);; All files (*.*)";

//pre-define theme
enum consoleThemes {
    light=0,
    dark,
    custom
};

//default font color
//QStringList DEF_fontColor =(QStringList() << Qt::black << Qt::white);
//QStringList DEF_bgColor =(QStringList() << Qt::white << Qt::black);
//DEF_baudRates;


#endif // CONST_H
