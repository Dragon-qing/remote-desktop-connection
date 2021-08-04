QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += /utf-8
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    downloadthread.cpp \
    downloadview.cpp \
    listenthread.cpp \
    main.cpp \
    mainwindow.cpp \
    receivethread.cpp \
    tcpserver.cpp

HEADERS += \
    downloadthread.h \
    downloadview.h \
    listenthread.h \
    mainwindow.h \
    receivethread.h \
    tcpserver.h

FORMS += \
    downloadview.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    exeico.rc

RC_FILE += exeico.rc

RESOURCES += \
    res.qrc
