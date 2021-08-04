#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QObject>
#include <QThread>
#include "tcpserver.h"
class ListenThread : public QThread
{
    Q_OBJECT
public:
    explicit ListenThread(QObject *parent = nullptr);
    void setServerSocket(TcpServer*ser);

signals:
    void acceptSocket(TcpServer*);
protected:
    void run();
private:
    TcpServer *server;
};

#endif // LISTENTHREAD_H
