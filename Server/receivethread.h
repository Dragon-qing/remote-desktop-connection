#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include "tcpserver.h"

#include <QObject>
#include <QThread>
#include <QTimer>
class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(QObject *parent = nullptr);
    ~ReceiveThread();
    void setTcpServer(TcpServer* ser);
protected:
    void run();
signals:

private:
    QTimer* timer;
    TcpServer *server;
};

#endif // RECEIVETHREAD_H
