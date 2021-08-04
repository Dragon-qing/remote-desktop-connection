#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include "tcpclient.h"

#include "messagelog.h"
#include <QWidget>

namespace Ui {
class DownloadWindow;
}

class DownloadWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadWindow(QWidget *parent = nullptr);
    void setTcpClient(TcpClient *tcp){this->tcpClient=tcp;}
    ~DownloadWindow();

private:
    Ui::DownloadWindow *ui;
    std::string qstr2str(const QString qstr);
    TcpClient *tcpClient;
    MessageLog *messagelog;
};

#endif // DOWNLOADWINDOW_H
