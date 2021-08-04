#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "downloadwindow.h"
#include "recvimagethread.h"
#include "tcpclient.h"
#include "uploadview.h"

#include <QMainWindow>
#include <myqwidget.h>
#include <QLabel>
#include <Windows.h>
#pragma comment(lib,"user32.lib")
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool isConnect,TcpClient*tcpClient,QWidget *parent = nullptr);
    ~MainWindow();
    void setLED(QLabel* label, int size,bool isConnect);
    void handler_receive_image(QPixmap*pix);
public:
    void stopThread();
    void keyPressEvent(QKeyEvent *ev);
    bool eventFilter(QObject *obj,QEvent *event);
private:
    Ui::MainWindow *ui;
    void initWindow();
    QPixmap image;
    bool isConnect;
    bool isDisplayBtn=true;
    bool isControl=false;
    QScreen*screen;
    RecvImageThread *imageThread;
    TcpClient *tcpClient;
    UploadView *upLoadView;
    DownloadWindow *downloadWindow;
};
#endif // MAINWINDOW_H
