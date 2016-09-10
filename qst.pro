# Package information (used for qbuild packages)
pkg=[
    name=qst
    desc="Qt Serial Term"
    license=GPL
    version=1.0.1-1
    maintainer="coolshou"
]

TEMPLATE = app
TARGET = qst


include(singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication
include(qextserialport/src/qextserialport.pri)

unix:DEFINES += _TTY_POSIX_
win32:DEFINES += _TTY_WIN_
CONFIG += debug

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets 
}

SOURCES += main.cpp \
    mainwindow.cpp \
    led.cpp \  
    aboutdialog.cpp \
    commdialog.cpp \
    optiondialog.cpp \
    consoleeditor.cpp
    
HEADERS += mainwindow.h \
    led.h \ 
    const.h \
    aboutdialog.h \
    commdialog.h \
    optiondialog.h \
    consoleeditor.h \
    version.h
    
FORMS += mainwindow.ui \
    aboutdialog.ui \
    commdialog.ui \
    optiondialog.ui
    
OTHER_FILES += 

RESOURCES += qst.qrc

# Install the launcher item. The metadata comes from the .desktop file
# and the path here.
desktop=[
    hint=desktop
    files=qst.desktop
    path=/user/shares/applications
#    trtarget=Qt Serial Term
]
# Install some pictures.
pics=[
    hint=pics
    files=images/*
    path=/usr/share/qst/pixmaps
]   
