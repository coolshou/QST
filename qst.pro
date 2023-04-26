
TEMPLATE = app
TARGET = qst


include(singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication
# include(qextserialport/src/qextserialport.pri)
include(qtserialport/src/serialport/serialport-lib.pri)

# CONFIG += debug

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets
    QT       += serialport
}

INCLUDEPATH += \
   src

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/led.cpp \
    src/aboutdialog.cpp \
    src/commdialog.cpp \
    src/optiondialog.cpp \
    src/consoleeditor.cpp \
    src/inputhistorydialog.cpp

HEADERS += \
    src/mainwindow.h \
    src/led.h \
    src/const.h \
    src/aboutdialog.h \
    src/commdialog.h \
    src/optiondialog.h \
    src/consoleeditor.h \
    src/inputhistorydialog.h

FORMS += \
    src/mainwindow.ui \
    src/aboutdialog.ui \
    src/commdialog.ui \
    src/optiondialog.ui \
    src/inputhistorydialog.ui

OTHER_FILES +=

RESOURCES += qst.qrc

VERSION = 1.1.0
unix:{
    DEFINES += _TTY_POSIX_
    #for serialport-lib.pri not found private/qringbuffer_p.h issue
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtCore/$${QT_VERSION}/QtCore/
    #INCLUDEPATH += $${QMAKE_INCDIR}/QtCore/$${QT_VERSION}/QtCore/

}

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
