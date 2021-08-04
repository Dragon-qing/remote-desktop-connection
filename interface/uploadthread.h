#ifndef UPLOADTHREAD_H
#define UPLOADTHREAD_H

#include "tcpclient.h"

#include <QObject>
#include <QThread>
#include <iostream>
class UploadThread : public QThread
{
    Q_OBJECT
public:
    explicit UploadThread(QObject *parent = nullptr);
    std::string qstr2str(const QString qstr);
    void setFileAbsPath(QString str);
    void setTcpClient(TcpClient * client){this->tcpClient=client;}
signals:
    void isDone();//处理完成信号
    void change(int);
protected:
    void run();

private:
    QString fileAbsPath;
    TcpClient *tcpClient;

};

#endif // UPLOADTHREAD_H
