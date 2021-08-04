#ifndef UPLOADVIEW_H
#define UPLOADVIEW_H

#include "tcpclient.h"
#include "uploadthread.h"
#include <QWidget>

namespace Ui {
class UploadView;
}

class UploadView : public QWidget
{
    Q_OBJECT

public:
    explicit UploadView(QWidget *parent = nullptr);
    ~UploadView();
    void stopThread();
    string qstr2str(const QString qstr);
    void dealChange(int i);
    void setTcpClient(TcpClient*tcp);
private:
    Ui::UploadView *ui;
    TcpClient * tcpClient;
    QString filePath;
    UploadThread *uploadThread;
};

#endif // UPLOADVIEW_H
