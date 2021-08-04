#ifndef SENDIMAGETHREAD_H
#define SENDIMAGETHREAD_H

#include "tcpserver.h"

#include <QObject>
#include <QThread>
class SendImageThread : public QThread
{
    Q_OBJECT
public:
    explicit SendImageThread(QObject *parent = nullptr);
    void setTcpServer(TcpServer* server){this->tcpServer=server;}
signals:
protected:
    void run();
private:
    TcpServer *tcpServer;
    QPixmap *image;
    QByteArray bytes;
};

#endif // SENDIMAGETHREAD_H
