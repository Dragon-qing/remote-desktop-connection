#ifndef RECVIMAGETHREAD_H
#define RECVIMAGETHREAD_H

#include "tcpclient.h"

#include <QObject>
#include <QThread>
class RecvImageThread : public QThread
{
    Q_OBJECT
public:
    explicit RecvImageThread(QObject *parent = nullptr);
    void setTcpClient(TcpClient* tcp);
protected:
    void run();
signals:

private:
    TcpClient* tcpClient;
};

#endif // RECVIMAGETHREAD_H
