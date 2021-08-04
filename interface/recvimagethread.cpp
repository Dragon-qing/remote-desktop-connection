#include "recvimagethread.h"

RecvImageThread::RecvImageThread(QObject *parent) : QThread(parent)
{

}
void RecvImageThread::run()
{
    tcpClient->receiveMessage();
}
void RecvImageThread::setTcpClient(TcpClient* tcp)
{
    this->tcpClient=tcp;
}
