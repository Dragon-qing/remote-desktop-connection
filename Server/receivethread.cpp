#include "receivethread.h"

ReceiveThread::ReceiveThread(QObject *parent) : QThread(parent)
{
    timer=new QTimer(this);

}
void ReceiveThread::run()
{
    server->receive_All();
}
ReceiveThread::~ReceiveThread()
{
    if(timer){
        delete timer;
    }
}

void ReceiveThread::setTcpServer(TcpServer *ser)
{
    this->server=ser;
}
