
include(./qextserialport/src/qextserialport.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets 
}

TARGET = qst
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    led.cpp
    
HEADERS += mainwindow.h \
    led.h
    
FORMS += mainwindow.ui \
    config.ui \
    about.ui
    
unix:DEFINES += _TTY_POSIX_
win32:DEFINES += _TTY_WIN_
CONFIG += debug
OTHER_FILES += 

RESOURCES += qst.qrc
