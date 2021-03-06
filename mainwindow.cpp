#include "mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QtDebug>
#include <QPainter>
#include <QTime>
#include <QDesktopWidget>

//#include "qextserialenumerator.h"

MainWindow::MainWindow(QWidget *parent)
//Polling mode?
    : QMainWindow(parent), serial(0), logFile(NULL)
//EventDriven mode?
//    : QMainWindow(parent), serial(QextSerialPort::EventDriven), logFile(NULL)
{
    setupUi(this);
    readSetting();
    applySetting();

    initActionsConnections();

    if (! useEventDriven) {
        //use timer to poll serial data
        connect(&timer, SIGNAL(timeout()), this, SLOT(pollSerial()));
        timer.start(100);
    }

    //textEdit->installEventFilter(this);
    connect(textEdit, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
    applyOptionSetting();
    //textEdit->setFont();
    //textEdit->setBackgroundRole();

    layout()->setSpacing(1);

    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::Panel);
    frame->setFrameShadow(QFrame::Sunken);
    sbList.append(frame);
    frame->setFixedWidth(33);
    txLed = new QLed(frame, QLed::green);
    txLed->setGeometry(1,1,15,15);
    rxLed = new QLed(frame, QLed::red);
    rxLed->setGeometry(16,1,15,15);

    for (int i=0;i<5;i++)
    {
        // Put a label in each box
        QLabel *statusLbl = new QLabel("Hey");
        statusLbl->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        statusLbl->setFrameShape(QFrame::Panel);
        statusLbl->setFrameShadow(QFrame::Sunken);
        sbList.append(statusLbl);
    }
    verticalLayout->setMargin(1);
    updateStatusBar();

    if (openAtStart)
        startStopComm();

}

MainWindow::~MainWindow()
{
    //TODO: delete any dialog?
//    delete aboutDlg;
//    delete commDlg;
//    delete optionDlg;
//    delete inputHistoryDlg;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APPName,
                                        tr("Are you sure to Quit?\n"),
                                        QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            //qDebug() <<"CloseEvent";
            saveSetting();
            saveOptionSetting();

            event->accept();
        }
}

// Grab keypresses meant for edit, send to serial port.
// TODO: special key like "tab" ...
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (serial)
        {
            QString s = keyEvent->text();
            if (s.length())
            {
                //char ch = s.at(0).toAscii();
                char ch = s.at(0).toLatin1();
                qDebug("keyEvent: %x", ch);
                if (serial->putChar(ch)) {
                    txLed->setActive(true);
                    if (inputHistory) {
                        if (ch != 13) {
                            // collect char to a tmp QString
                            sInputHistory.append(QChar(ch));
                        } else {
                            //use Enter (0x0D: CR) as end of a line
                            // Do no record rmpty line
                            if (! sInputHistory.isEmpty()) {
                            if (inputHistoryFile) {
                                //record a line to history file
                                qDebug() << "record:\""<< sInputHistory << "\" len:"
                                         << sInputHistory.length();

                                //QByteArray qba((const char *)sInputHistory.data(), sInputHistory.length());
                                //qDebug()<< "qba:" <<qba;
                                //inputHistoryFile->write(qba + "\n");
                                inputHistoryFile->write(sInputHistory.toLatin1() + "\n");
                                inputHistoryFile->flush();
                                sInputHistory.clear();
                            }
                            }
                        }
                    }

                }
            }
        }
        return true;
    }
    else
        // standard event processing
        return QObject::eventFilter(obj, event);
}

void MainWindow::updateStatusBar(void)
{
    for (int i=0;i<6;i++)
    {
        sbList.at(i)->hide();
        statusBar()->removeWidget(sbList.at(i));
    }

    if (serial)
    {
        for (int i=0;i<6;i++)
        {
            statusBar()->addWidget(sbList.at(i), (i == 5));
            sbList.at(i)->show();
        }
        QLabel *lbl;
        lbl = static_cast<QLabel*> (sbList.at(1));
        QString s = serial->portName();
        lbl->setText(s);
        lbl = static_cast<QLabel*> (sbList.at(2));
        s = baudRateStrings.at(baudNdx);
        lbl->setText(s);
        lbl = static_cast<QLabel*> (sbList.at(3));
        s = QString("N-8-1");
        lbl->setText(s);
        lbl = static_cast<QLabel*> (sbList.at(4));
        s = QString("No Flow Control");
        lbl->setText(s);
        lbl = static_cast<QLabel*> (sbList.at(5));
        s = QString("UNIX CR/LF");
        lbl->setText(s);
        for (int i=1;i<5;i++)
        {
            QLabel *lbl = static_cast<QLabel*>(sbList.at(i));
            lbl->setFixedWidth(lbl->minimumSizeHint().width());
        }
    }
    else
    {
        statusBar()->addWidget(sbList.at(5),100);
        QString status("Serial port closed");
        QLabel *lbl = static_cast<QLabel *>(sbList.at(5));
        lbl->setText(status);
        lbl->show();
    }
}

void MainWindow::startStopComm(void)
{
    if (serial)
    {
        // close it
        delete serial;
        serial = NULL;
    }
    else
    {
        // Start comm
        QString device;
#ifdef _TTY_POSIX_
        device = QString("/dev/");
#endif
        device += deviceName;

        serial = new QSerialPort(this);
        if (useEventDriven) {
            //serial = new QextSerialPort(device, QextSerialPort::EventDriven);
            //serial = new QextSerialPort(device, QextSerialPort::EventDriven);
            //sometimes did not get all data?
            connect(serial, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
            //connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
            //connect(serial, SIGNAL(readChannelFinished()), this, SLOT(onReadFinished()));
            //connect(serial, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        } else {
//            serial = new QextSerialPort(device, QextSerialPort::Polling);
        }
        /*
        serial->setBaudRate((BaudRateType)baudRates[baudNdx]);
        serial->setDataBits(DATA_8);
        serial->setParity(PAR_NONE);
        serial->setStopBits(STOP_1);
        serial->setFlowControl(hwFlow ? FLOW_HARDWARE : FLOW_OFF);
        serial->setTimeout(100);
        */
        serial->setPortName(device);
        serial->setBaudRate(baudRates[baudNdx]);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(hwFlow ? QSerialPort::HardwareControl : QSerialPort::NoFlowControl);

        if (!serial->open(QIODevice::ReadWrite))
        {
            delete serial;
            serial = NULL;
            QString s("Cannot open serial ");
            s += device;
            QMessageBox::critical(this,"Error",s);
            return;
        }
        if (inputHistory){
            //recore input history
            inputHistoryFile = new QFile(inputHistoryFileName);
            if (!inputHistoryFile->open(QIODevice::WriteOnly|QIODevice::Append))
            {
                QString s("Cannot write file: ");
                s += inputHistoryFileName;
                QMessageBox::critical(this,"File write error", s);
                inputHistoryFile = NULL;
                return;
            }

        }
    }
    // Update status bar
    updateStatusBar();
}



void MainWindow::configComm(void)
{
    QString selectedDevice;
    commDlg = new commDialog(this) ;

    // Load settings
    commDlg->setHwFlow(hwFlow);
    commDlg->setOpenStart(openAtStart);
    commDlg->setBaudNdx(baudNdx);

    // Find serial ports available
#ifdef _TTY_POSIX_
    QDir dir("/dev");
    QStringList filterList("ttyS*");
    filterList += QStringList("ttyUSB*");
    filterList += QStringList("ttyACM*");
    dir.setNameFilters(filterList);
    QStringList devices = dir.entryList(QDir::System);
#elif defined(_TTY_WIN_)
    QList< QextPortInfo > list = QextSerialEnumerator::getPorts();
    QStringList devices;
    for (int i=0;i<list.count();i++)
    {
        QString s = list.at(i).portName;
        char t[20];
        for(int j=0;j<s.length();j++)
        {
            if (!isprint((int)s.at(j).toLatin1()))
            {
                s.resize(j);
                break;
            }
            else
                t[j] = s.at(j).toLatin1();
        }
        devices.append(s);
    }
#endif

    for(int i=0;i<devices.count();i++)
    {
        if (!devices.at(i).contains("ttys")) {
            commDlg->additemListPort(devices.at(i), devices.at(i));
        }
        if (devices.at(i) == deviceName)
            selectedDevice = devices.at(i);
    }
    if (!selectedDevice.isEmpty())
    {
        qDebug() << "selectedDevice: " << selectedDevice;
        int index = commDlg->finditemListPort(selectedDevice);
        qDebug() << "index: " << index;
        if ( index != -1 ) { // -1 for not found
            commDlg->setCurrentPortNdx(index);
        }
    }
    //logfile
    commDlg->setLogFileEnable(LogFile);
    commDlg->setLogFileName(LogFileName);
    commDlg->setAddTimeStemp(AddTimeStemp);
    commDlg->setSplitFile(SplitFile);
    commDlg->setSplitFileSize(SplitFileSize);

    commDlg->exec();

    if (commDlg->result() == QDialog::Accepted)
    {
        // Open serial port
        if (serial)
        {
            delete serial;
            serial = NULL;
        }
        // make change back to variable
        hwFlow = commDlg->getHwFlow();
        baudNdx = commDlg->getBaudNdx();
        deviceName = commDlg->getPortName(commDlg->getCurrentPortNdx());
        openAtStart = commDlg->getOpenStart();
        //logfile
        LogFile = commDlg->getLogFileEnable();
        LogFileName = commDlg->getLogFileName();
        AddTimeStemp = commDlg->getAddTimeStemp();
        SplitFile = commDlg->getSplitFile();
        SplitFileSize = commDlg->getSplitFileSize();

        saveSetting();
        startStopComm();
    }
}

//read serial data at once
void MainWindow::readData(void)
{
    rxLed->setActive(true);
    //TODO: how to handle backspace with readAll??
    QByteArray data = serial->readAll();
    //TODO: timestemp
    textEdit->putData(data);
    //TODO: logFile
}

//read serial data by bytesAvailable
void MainWindow::onDataAvailable(void)
{
    txLed->setActive(false);

    int avail = serial->bytesAvailable();
    if( avail > 0 ) {
        rxLed->setActive(true);
        QByteArray bytes;
        bytes.resize(avail);
        int read = serial->read(bytes.data(), bytes.size());
        // qDebug() << "bytes read:" << bytes.size();

        if( read > 0 ) {
            if (logFile)
            {
                //TODO: add timestamp at the begin of line
                if (AddTimeStemp) {
                    QString sNow = QTime::currentTime().toString("yyyy/MM/dd_hh:mm:ss.zzz")+ " ";
                    logFile->write(sNow.toLatin1() + bytes);
                } else {
                    logFile->write(bytes);
                }
                logFile->flush();
            }
            bytes.replace("\r", ""); //TODO: why remove \r ?
            //qDebug()<<"bytes:"<<bytes;
            // TODO: how about \n  ?
            if (bytes.contains(8))
            {
                // Must parse backspace commands manually
                for (int i=0;i<bytes.count();i++)
                {
                    char ch = bytes.at(i);
                    if (ch == 8)
                    {
                        // Backspace
                        QString s = textEdit->toPlainText();
                        //s.chop(1); //Removes 1 characters from the end of the string.
                        //qDebug()<<"size QByteArray:"<<sizeof(QByteArray);
                        //qDebug()<<"size char:"<<sizeof(ch);
                        s.chop(sizeof(ch));
                        textEdit->setPlainText(s);
                    }
                    else
                    {
                        //TODO: tab char
                        // Add char to edit
                        QString s(ch);
                        textEdit->insertPlainText(s);
                    }
                    textEdit->moveCursor(QTextCursor::End);
                }
            }
            else
            {
                //textEdit->moveCursor(QTextCursor::End);
                //textEdit->insertPlainText(bytes);
                textEdit->putData(bytes);
            }
/*
            if (autoScroll) {
                textEdit->ensureCursorVisible();
            }
*/
            rxLed->setActive(false);

        }
    }
    //rxLed->setActive(false);
}

void MainWindow::onReadFinished(void)
{
    qDebug() << "onReadFinished";
    rxLed->setActive(false);
}

void MainWindow::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}

void MainWindow::pollSerial(void)
{
    // Check for serial chars

    rxLed->setActive(false);
    txLed->setActive(false);

    if (!serial) return;
    
    QByteArray bytes = serial->readAll();

    if (bytes.isEmpty()) return;
    if (logFile)
    {
        logFile->write(bytes);
        logFile->flush();
    }
    bytes.replace("\r", "");
    if (bytes.contains(8))
    {
        // Must parse backspace commands manually
        for (int i=0;i<bytes.count();i++)
        {
            char ch = bytes.at(i);
            if (ch == 8)
            {
                // Backspace
                QString s = textEdit->toPlainText();
                s.chop(1);
                textEdit->setPlainText(s);
            }
            else
            {
                // Add char to edit
                QString s(ch);
                textEdit->insertPlainText(s);
            }
            textEdit->moveCursor(QTextCursor::End);
        }
    }
    else
    {
        textEdit->moveCursor(QTextCursor::End);
        textEdit->insertPlainText(bytes);
    }
    if (autoScroll) {
        textEdit->ensureCursorVisible();
    }
    rxLed->setActive(true);
}

void MainWindow::sendFile(void)
{
    if (!serial) return;

    QString name = QFileDialog::getOpenFileName(this,
                                                "Open File",
                                                QDir::homePath(),
                                                "All Files (*)");
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly))
    {
        QString s("Cannot read file ");
        s += name;
        QMessageBox::critical(this,"File read error", s);
        return;
    }

    QByteArray bytes = file.readAll();
    serial->write(bytes);
    file.close();
}

void MainWindow::helpAbout(void)
{
    aboutDlg = new aboutDialog(this);
    //about->show();
    aboutDlg->exec();
/*
    QDialog dlg;
    Ui_aboutDialog dlg_ui;
    dlg_ui.setupUi(&dlg);

    dlg.exec();
*/
}

void MainWindow::configOption(void)
{
    optionDlg = new optionDialog(this);
    //setting
    optionDlg->setThemeCurrentIndex(themeIdx);
    //optionDlg->setFontColorCurrentIndex(fontColorIdx);
    //optionDlg->setBgColorCurrentIndex(bgColorIdx);
    optionDlg->setWordWrapChecked(wordWrap);
    optionDlg->setInputHistoryChecked(inputHistory);
    optionDlg->setInputHistoryFilename(inputHistoryFileName);
    optionDlg->setAutoScrollChecked(autoScroll);

    optionDlg->exec();
    if (optionDlg->result() == QDialog::Accepted) {
        qDebug() << "save option";
        //make change to variable
        themeIdx = optionDlg->getThemeCurrentIndex();
        fontColorIdx = optionDlg->getFontColorCurrentIndex();
        bgColorIdx = optionDlg->getBgColorCurrentIndex();
        wordWrap = optionDlg->isWordWrapChecked();
        autoScroll = optionDlg->isAutoScrollChecked();
        inputHistory = optionDlg->isInputHistoryChecked();
        inputHistoryFileName = optionDlg->getInputHistoryFilename();

        applyOptionSetting();
        saveOptionSetting();
    }
    optionDlg->close();
}


void MainWindow::saveScreen(void)
{
    QString name = QFileDialog::getSaveFileName(this,
                                                "Save Screen",
                                                QDir::homePath(),
                                                "All Files (*)");
    if (name.isEmpty())
        return;
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly))
    {
        QString s("Cannot write file ");
        s += name;
        QMessageBox::critical(this,"File read error", s);
        return;
    }

    file.write(textEdit->toPlainText().toLatin1());

    file.close();
}

void MainWindow::startLogging(void)
{
    if (logFile)
    {
        QMessageBox::critical(this, "Error", "Logging already active");
        return;
    }
/*
    QString name = QFileDialog::getSaveFileName(this,
                                                "Select log file",
                                                QDir::homePath(),
                                                logfilter,
                                                &select_logfilter);
*/
    QString name = LogFileName;
    if (name.length() == 0)
        return;
    logFile = new QFile(name);
    if (!logFile->open(QIODevice::WriteOnly))
    {
        QString s("Cannot write file ");
        s += name;
        QMessageBox::critical(this,"File read error", s);
        logFile = NULL;
        return;
    }
}

void MainWindow::endLogging(void)
{
    if (logFile)
    {
        logFile->close();
        delete logFile;
        logFile = NULL;
    }
}

void MainWindow::aboutQt(void)
{
    QMessageBox::aboutQt(this, "About Qt");
}
void MainWindow::showInputHistory(void)
{
    inputHistoryDlg = new InputHistoryDialog(this);
    inputHistoryDlg->setInputHistoryFileName(inputHistoryFileName);
    inputHistoryDlg->exec();
    if (inputHistoryDlg->result() == QDialog::Accepted) {
        //TODO
    }
}
void MainWindow::readSetting(void)
{
    // Get settings from config file (or, god forbid, registry)
    QSettings settings("QST","QST");
    //window geome
    settings.beginGroup("window");
    QRect screenRect = QApplication::desktop()->screenGeometry();
    QRect defRect = QRect((screenRect.width()-800)/2,
                         (screenRect.height()-600)/2,
                         800, 600);
    mGeometry = settings.value("geometry", defRect).toRect();
    settings.endGroup();

    //support baudRates
    settings.beginGroup("baudRates");
    QStringList baudRateskeys = settings.allKeys();
    //int iMax = sizeof(DEF_baudRates)/sizeof(DEF_baudRates[0]);
    int iRate=0;
    if (baudRateskeys.length()<Max_BaudRates) {
        //when DEF_baudRates changed, update config
        for (iRate=0; iRate<Max_BaudRates; ++iRate) {
            qDebug() << "DEF_baudRates:" << DEF_baudRates[iRate];
            baudRates[iRate] = DEF_baudRates[iRate];
            baudRateStrings.append(QString::number(DEF_baudRates[iRate]));
        }
    } else if (baudRateskeys.length()>0) {
        //qDebug() << baudRateskeys;
        foreach (const QString &baudRateskey, baudRateskeys) {
            baudRates[iRate] = settings.value(baudRateskey, DEF_baudRates[iRate]).toInt();
            //qDebug()<<"baudRates[iRate]:" <<QString::number(baudRates[iRate]);
            baudRateStrings.append(QString::number(baudRates[iRate]));
            iRate = iRate + 1;
        }
    }
    settings.endGroup();

    //comm
    settings.beginGroup("comm");
    baudNdx     = settings.value("baudNdx", 0).toInt();
    hwFlow      = settings.value("hwflow", false).toBool();
    openAtStart = settings.value("openAtStart", false).toBool();
    deviceName  = settings.value("device", "ttyS0").toString();

    LogFile = settings.value("LogFile", false).toBool();
    LogFileName = settings.value("LogFileName", DEF_LogFileName).toString();
    AddTimeStemp = settings.value("AddTimeStemp", true).toBool();
    SplitFile = settings.value("SplitFile", false).toBool();
    SplitFileSize = settings.value("SplitFileSize", 0).toInt();
    settings.endGroup();

    //theme
    settings.beginGroup("theme");
    themeIdx = settings.value("themeIdx", 0).toInt();
    fontColorIdx = settings.value("fontColorIdx", 0).toInt();
    bgColorIdx = settings.value("bgColorIdx", 0).toInt();
    wordWrap = settings.value("wordWrap", true).toBool();
    autoScroll = settings.value("autoScroll", true).toBool();
    inputHistory = settings.value("inputHistory", false).toBool();
    inputHistoryFileName = settings.value("inputHistoryFileName", DEF_HistoryFileName ).toString();
    settings.endGroup();
    //support fontColor
    //support bgColor

}

void MainWindow::saveSetting(void)
{
    // Save settings
    QSettings settings("QST","QST");
    settings.beginGroup("window");
    settings.setValue("geometry", this->geometry());
    settings.endGroup();

    //support baudRates
    settings.beginGroup("baudRates");
    //int iMax = sizeof(baudRates)/sizeof(baudRates[0]);
    //qDebug() << "Max:" << iMax;
    for (int iRate = 0; iRate < Max_BaudRates; ++iRate) {
        //qDebug()<<"baudRates["<<iRate<<"]:" <<baudRates[iRate];
        settings.setValue(QString::number(iRate), baudRates[iRate]);
    }
    settings.endGroup();

    settings.beginGroup("comm");
    settings.setValue("hwflow", hwFlow);
    settings.setValue("openAtStart", openAtStart);
    settings.setValue("baudNdx", baudNdx);
    settings.setValue("device", deviceName);
    settings.setValue("LogFile", LogFile);
    settings.setValue("LogFileName", LogFileName);
    settings.setValue("AddTimeStemp", AddTimeStemp);
    settings.setValue("SplitFile", SplitFile);
    settings.setValue("SplitFileSize", SplitFileSize);
    settings.endGroup();
}

void MainWindow::applySetting(void)
{
    this->setGeometry(mGeometry);
}

void MainWindow::saveOptionSetting(void)
{
    // Save settings
    QSettings settings("QST","QST");
    //theme
    settings.beginGroup("theme");
    settings.setValue("themeIdx", themeIdx);
    settings.setValue("fontColorIdx", fontColorIdx);
    settings.setValue("bgColorIdx", bgColorIdx);
    settings.setValue("wordWrap", wordWrap);
    settings.setValue("autoScroll", autoScroll);
    settings.setValue("inputHistory", inputHistory);
    settings.setValue("inputHistoryFileName", inputHistoryFileName);
    settings.endGroup();

}
void MainWindow::applyOptionSetting(void)
{
    if (wordWrap) {
        textEdit->setWordWrapMode(QTextOption::WordWrap);
    } else {
        textEdit->setWordWrapMode(QTextOption::NoWrap);
    }
    textEdit->setCenterOnScroll(autoScroll);
    textEdit->setAutoScroll(autoScroll);
    //console theme
    textEdit->set_theme(DEF_fontColors[fontColorIdx], DEF_bgColors[bgColorIdx]);
    //TODO: 1. textEdit right clieck paste item
    //      2. ctrl + c
    //      3. ctrl + v
    //      4. up arrow (last input history?)
    //      5. down arrow
    //
}

QStringList MainWindow::getBaudRateStrings(void)
{
    //qDebug() << "getBaudRateStrings" << baudRateStrings;
    return baudRateStrings;
}

void MainWindow::writeData(const QByteArray &data)
{
    if (serial) {
        txLed->setActive(true);
        qDebug() << "writeData" <<data;
        //serial->write(data);
        serial->write(data);
        /*
        const char *ch;

        serial->putChar(ch);
        */
        //const char *ch;
        //ch = data.data();
        if (inputHistory) {
            if (data.indexOf("\r")<0) {
                // collect char to a tmp QString
                sInputHistory.append(data);
            } else {
                //use Enter (0x0D: CR) as end of a line
                // Do no record rmpty line
                if (! sInputHistory.isEmpty()) {
                    if (inputHistoryFile) {
                        //record a line to history file
                        qDebug() << "record:\""<< sInputHistory << "\" len:"
                                 << sInputHistory.length();

                        //QByteArray qba((const char *)sInputHistory.data(), sInputHistory.length());
                        //qDebug()<< "qba:" <<qba;
                        //inputHistoryFile->write(qba + "\n");
                        inputHistoryFile->write(sInputHistory.toLatin1() + "\n");
                        inputHistoryFile->flush();
                        sInputHistory.clear();
                    }
                }
            }
        }
    }
}

void MainWindow::initActionsConnections(void)
{
    connect(actionComm, SIGNAL(triggered()), this, SLOT(configComm()));
    connect(actionOption, SIGNAL(triggered()), this, SLOT(configOption()));
    connect(actionStart_Stop_Comm, SIGNAL(triggered()), this, SLOT(startStopComm()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
    connect(actionSend, SIGNAL(triggered()), this, SLOT(sendFile()));
    connect(actionSave_Screen, SIGNAL(triggered()), this, SLOT(saveScreen()));
    connect(actionBegin_Logging, SIGNAL(triggered()), this, SLOT(startLogging()));
    connect(actionEnd_Logging, SIGNAL(triggered()), this, SLOT(endLogging()));
    connect(actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    connect(actionHistory, SIGNAL(triggered()), this, SLOT(showInputHistory()));
}
