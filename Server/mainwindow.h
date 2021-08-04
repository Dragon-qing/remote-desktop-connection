#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "receivethread.h"
#include "tcpserver.h"
#include "listenthread.h"
#include "downloadview.h"
#include <queue>
#include <Windows.h>
#pragma comment(lib,"user32.lib")
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string qstr2str(const QString qstr);
    void deal_acceptSocket(TcpServer*server);
public:
    void stopThread();
    void deal_mouse(double dx,double dy ,int type);
    void deal_keyBoard(int key);
    void deal_requestFile();
    //void deal_receive_file(FileHader &file,char* data,int length);
private:
    Ui::MainWindow *ui;
    QString savePath;
    ReceiveThread *reThread;
    ListenThread* listenThread;
    TcpServer* server;
    QTimer *timer;
    QTimer *dealKeyTimer;
    QScreen*screen;
    QPixmap image;
    QPixmap newpixmap;
    QPixmap oldpixmap;
    queue<int>*que;
    DownloadView *downloadView;
//    queue<char*> *quefileData;
//    queue <FileHader>*quefileHader;
//    queue <int>*quefileLength;
//    QTimer *dealFileTimer;
};
#endif // MAINWINDOW_H
