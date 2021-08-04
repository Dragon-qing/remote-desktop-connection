#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QObject>
#include <QThread>
#include "tcpserver.h"
class DownloadThread : public QThread
{
    Q_OBJECT
public:
    explicit DownloadThread(QObject *parent = nullptr);
    std::string qstr2str(const QString qstr);
    void setFileAbsPath(QString str);
    void setTcpServer(TcpServer*server){this->tcpServer=server;}
protected:
    void run();

signals:
    void isDone();//处理完成信号
    void change(int);
private:
    QString fileAbsPath;
    TcpServer *tcpServer;

};

#endif // DOWNLOADTHREAD_H
