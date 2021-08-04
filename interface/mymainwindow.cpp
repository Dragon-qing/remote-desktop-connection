#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include"mainwindow.h"
#include<QPushButton>
#include<QMessageBox>
#include<QTextCodec>
//登录窗口设置
myMainWindow::myMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::myMainWindow)
{
    ui->setupUi(this);
    m=new MessageLog();
    tcpClient=new TcpClient();
    //设置图标，窗口名称，窗口大小
    setWindowIcon(QIcon(":/picture/tb.ico"));
    this->setWindowTitle(QString::fromUtf16(u"远程工具"));
    this->setMinimumSize(370, 150);
    this->setMaximumSize(370, 150);
    //去掉边框
    setWindowFlags(this->windowFlags()&~Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint);
    //连接按钮信号
    connect(ui->mw_btn1,&QPushButton::clicked,this,[=](){
        QString ipAddr=ui->lineEdit1->text();
        //QString ipAddr="127.0.0.1";
        if(ipAddr==""){
            QMessageBox::critical(this,QString::fromUtf16(u"错误"),QString::fromUtf16(u"ip地址不能为空！！!"));
        }else{
            isConnected=tcpClient->TARGE_FILE(ipAddr.toStdString());
            w=new MainWindow(isConnected,tcpClient);
            this->hide();
            w->show();
            if(isConnected){
                m->setLogText("连接成功");
                m->startTimer();
                m->show();
            }else{
                m->setLogText("连接失败！！！");
                m->startTimer();
                m->show();
            }
        }

    });

}

myMainWindow::~myMainWindow()
{
    delete ui;
    if(m){
        delete m;
    }
}
