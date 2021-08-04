#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QDebug>
#include <QDesktopWidget>
#include <QRect>
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    que=new queue<int>();
    server=new TcpServer();
    listenThread=new ListenThread;
    reThread=new ReceiveThread;
    timer=new QTimer(this);
    dealKeyTimer=new QTimer();
    screen=QApplication::primaryScreen();
    setWindowIcon(QIcon(":/picture/tb.ico"));
    ui->startBtn->setEnabled(false);
    connect(ui->savePathBtn,&QPushButton::clicked,[=](){
        savePath=QFileDialog::getExistingDirectory(this, "选择目录", QDir::currentPath(), QFileDialog::ShowDirsOnly);
        ui->savePathLineEdit->setText(savePath);
        if(savePath==""){
            ui->startBtn->setEnabled(false);
           QMessageBox::critical(this,"错误","路径不能为空","确定");
        }else{
            server->setFilePath(qstr2str(savePath.toUtf8()));
            ui->startBtn->setEnabled(true);
        }
    });
    connect(ui->startBtn,&QPushButton::clicked,[=](){
       ui->startBtn->setEnabled(false);
       listenThread->setServerSocket(server);
       listenThread->start();
    });
    connect(listenThread,&ListenThread::acceptSocket,this,&MainWindow::deal_acceptSocket);
    connect(timer,&QTimer::timeout,[=](){
        image =  screen->grabWindow(0);//抓屏
        image = image.scaled(1600, 800, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        newpixmap = image;
        if(oldpixmap.toImage()!=newpixmap.toImage()){
            oldpixmap=newpixmap;
            QByteArray bytes;
            //qDebug()<<bytes.length();
            QBuffer buffer(&bytes);
            buffer.open(QIODevice::ReadWrite);
            image.save(&buffer,"jpg",50);
            server->send_image(image.width(),image.height(),bytes,bytes.toBase64().length());
        }
    });
    connect(this,&MainWindow::destroyed,this,&MainWindow::stopThread);
    connect(ui->exitBtn,&QPushButton::clicked,this,[=](){
        stopThread();
        exit(0);
    });

    connect(server,&TcpServer::receive_mouse,this,&MainWindow::deal_mouse);
    connect(server,&TcpServer::receive_keyBoard,this,&MainWindow::deal_keyBoard);
    connect(dealKeyTimer,&QTimer::timeout,this,[=](){
        if(!que->empty()){
            int key=que->front();
            ::keybd_event(key,0,0,0);
                Sleep(10);
            ::keybd_event(key,0,KEYEVENTF_KEYUP,0);
            que->pop();
        }
    });
    connect(server,&TcpServer::requestFile,this,&MainWindow::deal_requestFile);

}

MainWindow::~MainWindow()
{
    if(reThread){
        stopThread();
        delete reThread;
    }
    if(server){
        server->CLEAR();
    }
    if(que){
        delete que;
    }
    if(timer){
        delete timer;
    }
    if(dealKeyTimer){
        delete dealKeyTimer;
    }
    if(downloadView){
        delete downloadView;
    }

    delete ui;
}
std::string MainWindow::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}
void MainWindow::deal_acceptSocket(TcpServer*server)
{
    this->server=server;
    timer->start(50);
    reThread->setTcpServer(server);
    reThread->start();
}
void MainWindow::stopThread()
{

    qDebug()<<"结束线程";
    listenThread->quit();
    reThread->quit();
}

void MainWindow::deal_mouse(double dx,double dy,int type)
{
    QRect screenRect =QApplication::desktop()->screenGeometry();
    double x=dx*screenRect.width();
    double y=dy*screenRect.height();
    switch(type){
    case MOUSEEVENTF_LEFTDOWN:
        mouse_event(MOUSEEVENTF_LEFTDOWN,x,y,0,0);
        break;
    case MOUSEEVENTF_RIGHTDOWN:
        mouse_event(MOUSEEVENTF_RIGHTDOWN,x,y,0,0);
        break;
    case MOUSEEVENTF_LEFTUP:
        mouse_event(MOUSEEVENTF_LEFTUP,x,y,0,0);
        break;
    case MOUSEEVENTF_RIGHTUP:
        mouse_event(MOUSEEVENTF_RIGHTUP,x,y,0,0);
        break;
    case MOUSEEVENTF_WHEEL:
        mouse_event(MOUSEEVENTF_WHEEL,x,y,120,0);
        break;
    case MOUSEEVENTF_WHEEL+1:
        mouse_event(MOUSEEVENTF_WHEEL,x,y,-120,0);
        break;
    case MOUSEEVENTF_MOVE:
        mouse_event(MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE,dx*65535,dy*65535,0,0);
        break;
    }

}

void MainWindow::deal_keyBoard(int key)
{
    que->push(key);
    if(!dealKeyTimer->isActive()){
        dealKeyTimer->start(20);
    }

}

void MainWindow::deal_requestFile()
{
    downloadView=new DownloadView;
    downloadView->setTcpServer(server);
    downloadView->show();
}


