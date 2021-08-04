 #include "ui_mainwindow.h"
#include "mainwindow.h"
#include"myqwidget.h"
#include<QToolBar>
#include<QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QScreen>
#include <QApplication>
#include<QIcon>
#include<QWidget>
#include<QTimer>
#include<QDebug>
#include <QBuffer>
#include <QMouseEvent>
#include <QEvent>
#include <QCursor>

MainWindow::MainWindow(bool isConnect,TcpClient*tcpClient,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->grabKeyboard();
    ui->DeskTop->installEventFilter(this);
    imageThread=new RecvImageThread();
    this->isConnect=isConnect;
    this->tcpClient=tcpClient;
    screen=QApplication::primaryScreen();
    initWindow();

    //开启图片接受线程
    imageThread->setTcpClient(tcpClient);
    imageThread->start();

    connect(ui->actionopen,&QAction::triggered,this,[=](){
        ui->widget->show();
    });
    connect(ui->actionclose,&QAction::triggered,this,[=](){
        ui->widget->hide();
    });

    connect(tcpClient,&TcpClient::receive_image,this,&MainWindow::handler_receive_image);
    connect(this, &MainWindow::destroyed, this, &MainWindow::stopThread);
    connect(ui->widget->getDisplayBtn(),&QPushButton::clicked,[=](){
        if(isDisplayBtn){
            ui->DeskTop->hide();
            isDisplayBtn=false;
        }else{
            ui->DeskTop->show();
            isDisplayBtn=true;
        }
    });
    connect(ui->widget->getControlBtn(),&QPushButton::clicked,this,[=](){
        if(isControl){
            isControl=false;
        }else{
            isControl=true;
        }
    });

    connect(ui->widget->getUploadBtn(),&QPushButton::clicked,this,[=](){
        upLoadView=new UploadView();
        upLoadView->setTcpClient(tcpClient);
        upLoadView->show();

    });
    connect(ui->widget->getDownloadBtn(),&QPushButton::clicked,this,[=](){
        downloadWindow=new DownloadWindow();
        downloadWindow->setTcpClient(tcpClient);
        downloadWindow->show();
        tcpClient->send_file("",3,nullptr,0,0);

    });
    connect(ui->widget->getExitBtn(),&QPushButton::clicked,this,[=](){
        stopThread();
        exit(0);
    });
}
MainWindow::~MainWindow()
{
    if(tcpClient){
        tcpClient->CLEAR();
        delete tcpClient;
    }
    if(screen){
        delete screen;
    }
    if(imageThread){
        delete imageThread;
    }
    if(upLoadView){
        delete upLoadView;
    }
    if(downloadWindow){
        delete downloadWindow;
    }
    delete ui;
}
//初始化界面
void MainWindow::initWindow()
{
    setWindowIcon(QIcon(":/picture/tb.ico"));
    this->resize(1650,1100);
    this->setWindowTitle(QString::fromUtf16(u"桌面远程协助"));
    QToolBar * toolBar1 = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea,toolBar1);
    toolBar1->setAllowedAreas(Qt::TopToolBarArea);
    toolBar1->setMovable(false);
    toolBar1->addWidget(ui->led1);
    toolBar1->addWidget(ui->w_text1);
    toolBar1->addWidget(ui->w_text);//桌面状态指示
    toolBar1->addWidget(ui->w_text2);
    toolBar1->addWidget(ui->w_text3);//被连接端IP地址显示
    setLED(ui->led1, 20,isConnect);
}
//圆形指示灯，复制的csdn上的，可以根据不同情况设置不同颜色
void MainWindow::setLED(QLabel* label, int size,bool isConnect)
{
    // 将label中的文字清空
    label->setText("");
    // 先设置矩形大小
    // 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
    // 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
    QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
    QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
    QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
    QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
    // 再设置边界形状及边框
    QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
    QString border = QString("border:1px solid black;");                    // 边框为1px黑色
    // 最后设置背景颜色
    QString background = "background-color:";
    if(isConnect){

        background += "rgb(0,255,0)";
    }else{
        background += "rgb(255,0,0)";
    }
    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    label->setStyleSheet(SheetStyle);
}
void MainWindow::handler_receive_image(QPixmap*pix)
{
    //ui->DeskTop->setPixmap(*pix);
    ui->DeskTop->setPixmap(pix->scaled(ui->DeskTop->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->DeskTop->setScaledContents(true);
    delete pix;
}
void MainWindow::stopThread()
{

    imageThread->quit();
}
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(isControl){
    switch(ev->key())
    {
    case Qt::Key_Space:
        tcpClient->send_keyBoard(Qt::Key_Space);
            break;
    default:
        tcpClient->send_keyBoard(ev->nativeVirtualKey());
    }
    }

}
bool MainWindow::eventFilter(QObject *obj,QEvent *event)
{
    //监听触摸屏的单击事件
        if(obj==ui->DeskTop)
        {
            if(event->type()==QEvent::MouseButtonPress)
            {

                QMouseEvent* qEvent=(QMouseEvent*)event;
                if(qEvent->button()&Qt::LeftButton){
                    if(isControl)
                    tcpClient->send_mouse(MOUSEEVENTF_LEFTDOWN,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                }else if(qEvent->button()&Qt::RightButton){
                    if(isControl)
                    tcpClient->send_mouse(MOUSEEVENTF_RIGHTDOWN,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());

                }
                return true;
            }
            else if(event->type()==QEvent::MouseButtonRelease)
            {
                QMouseEvent* qEvent=(QMouseEvent*)event;
                if(qEvent->button()&Qt::LeftButton){
                    if(isControl)
                    tcpClient->send_mouse(MOUSEEVENTF_LEFTUP,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                }else if(qEvent->button()&Qt::RightButton){
                    if(isControl)
                    tcpClient->send_mouse(MOUSEEVENTF_RIGHTUP,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                }

                return true;
            }
            else if(event->type()==QEvent::MouseButtonDblClick)
            {
                QMouseEvent* qEvent=(QMouseEvent*)event;
                if(qEvent->button()&Qt::LeftButton){
                    if(isControl){

                        tcpClient->send_mouse(MOUSEEVENTF_LEFTDOWN,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                        tcpClient->send_mouse(MOUSEEVENTF_LEFTUP,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                        tcpClient->send_mouse(MOUSEEVENTF_LEFTDOWN,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                        tcpClient->send_mouse(MOUSEEVENTF_LEFTUP,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                    }
                }else if(qEvent->button()&Qt::RightButton){
                   if(isControl){
                       tcpClient->send_mouse(MOUSEEVENTF_RIGHTDOWN,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                       tcpClient->send_mouse(MOUSEEVENTF_RIGHTUP,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                       tcpClient->send_mouse(MOUSEEVENTF_RIGHTDOWN,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                       tcpClient->send_mouse(MOUSEEVENTF_RIGHTUP,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());
                   }
                }

                return true;
            }
            else if(event->type()==QEvent::Enter)
            {
                ui->DeskTop->setMouseTracking(true);

                return true;
            }
            else if(event->type()==QEvent::Leave)
            {
                ui->DeskTop->setMouseTracking(false);

                return true;
            }
            else if(event->type()==QEvent::MouseMove)
            {
                QMouseEvent* qEvent=(QMouseEvent*)event;
                if(isControl)
                tcpClient->send_mouse(MOUSEEVENTF_MOVE,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());

                return true;
            }
            else if(event->type()==QEvent::Wheel)
            {
                QWheelEvent* qEvent=(QWheelEvent*)event;

                if(qEvent->angleDelta().y()>0){
                    if(isControl)
                    tcpClient->send_mouse(MOUSEEVENTF_WHEEL,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());

                }else{
                    if(isControl)
                    tcpClient->send_mouse(MOUSEEVENTF_WHEEL+1,qEvent->x(),qEvent->y(),ui->DeskTop->width(),ui->DeskTop->height());

                }
                return true;
            }else{
                return false;
            }

        }
        else
        {
            return MainWindow::eventFilter(obj,event);
        }
}



