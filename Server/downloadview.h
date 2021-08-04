#ifndef DOWNLOADVIEW_H
#define DOWNLOADVIEW_H

#include "downloadthread.h"
#include "tcpServer.h"

#include <QWidget>

namespace Ui {
class DownloadView;
}

class DownloadView : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadView(QWidget *parent = nullptr);
    ~DownloadView();
    void stopThread();
    string qstr2str(const QString qstr);
    void dealChange(int i);
    void setTcpServer(TcpServer*tcp);
private:
    Ui::DownloadView *ui;
    DownloadThread* downloadThread;
    TcpServer * tcpServer;
    QString filePath;
};

#endif // DOWNLOADVIEW_H
