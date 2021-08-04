QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += /utf-8
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    downloadwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    messagelog.cpp \
    mymainwindow.cpp \
    myqwidget.cpp \
    recvimagethread.cpp \
    tcpclient.cpp \
    uploadthread.cpp \
    uploadview.cpp

HEADERS += \
    downloadwindow.h \
    mainwindow.h \
    messagelog.h \
    mymainwindow.h \
    myqwidget.h \
    recvimagethread.h \
    tcpclient.h \
    uploadthread.h \
    uploadview.h

FORMS += \
    downloadwindow.ui \
    mainwindow.ui \
    messagelog.ui \
    mymainwindow.ui \
    myqwidget.ui \
    uploadview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    exeico.rc \
    picture/picture1.jpeg \
    picture/picture2.jpeg \
    picture/picture3.jpeg \
    picture/picture4.jpeg

RESOURCES += \
    res.qrc
RC_FILE += exeico.rc

LIBS +=User32.LIB
