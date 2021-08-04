#include "listenthread.h"

ListenThread::ListenThread(QObject *parent) : QThread(parent)
{

}

void ListenThread::setServerSocket(TcpServer *ser)
{
    this->server=ser;
}


void ListenThread::run()
{
    server->startSocket();
     emit acceptSocket(server);

}
