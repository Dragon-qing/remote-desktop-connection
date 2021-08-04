#include "messagelog.h"
#include "ui_messagelog.h"

#include <QRect>
#include <QScreen>
#include <QDebug>
#include <QTimer>
#include <QIcon>
MessageLog::MessageLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageLog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/picture/tb.ico"));
    timer=new QTimer(this);
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect screenRect =  screen->availableVirtualGeometry();
    move(screenRect.width()-this->width()-1,screenRect.height()-this->height()-38);
    connect(timer,&QTimer::timeout,this,[=](){
        this->close();
    });
}

MessageLog::~MessageLog()
{
    delete ui;
    if(timer){
        delete timer;
    }
}
void MessageLog::setLogText(QString s)
{
    ui->label->setText(s);
}
void MessageLog::startTimer()
{
    timer->start(3000);
}
