
TEMPLATE = app
TARGET = qst


include(singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication
include(qextserialport/src/qextserialport.pri)

# CONFIG += debug

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
    consoleeditor.h
    
FORMS += mainwindow.ui \
    aboutdialog.ui \
    commdialog.ui \
    optiondialog.ui
    
OTHER_FILES += 

RESOURCES += qst.qrc

VERSION = 1.0.2
unix:DEFINES += _TTY_POSIX_
win32:{
DEFINES += _TTY_WIN_
RC_ICONS += images/qst.ico

QMAKE_TARGET_COMPANY = coolshou
QMAKE_TARGET_PRODUCT = QST
QMAKE_TARGET_DESCRIPTION = Qt Serial Terminal
QMAKE_TARGET_COPYRIGHT = GPLv2

}
DEFINES += VERSION=\\\"$${VERSION}\\\"
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
# Package information (used for qbuild packages)
pkg=[
    name=qst
    desc="Qt Serial Term"
    license=GPL
    version=$VERSION-1
    maintainer="coolshou"
]
