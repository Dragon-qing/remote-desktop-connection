#include "mainwindow.h"
#include"mymainwindow.h"
#include "messagelog.h"
#include <QDebug>
#include <QApplication>
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myMainWindow d;
    d.show();
    return a.exec();
}

