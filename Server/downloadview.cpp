#include "downloadview.h"
#include "ui_downloadview.h"

#include <QFileDialog>
#include <QMessageBox>
DownloadView::DownloadView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadView)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/picture/tb.ico"));
    downloadThread=new DownloadThread;
    ui->uploadBtn->setEnabled(false);
    connect(ui->selectBtn,&QPushButton::clicked,this,[=](){
        filePath=QFileDialog::getOpenFileName(this,QString::fromUtf16(u"选择文件"),QDir::homePath());
        ui->fileLine->setText(filePath);
        if(filePath==""){
            QMessageBox::critical(this,"error","The file address cannot be empty");
        }else{
            downloadThread->setFileAbsPath(filePath);
            ui->progressBar->setValue(0);
            ui->uploadBtn->setEnabled(true);
        }
    });
    connect(ui->uploadBtn,&QPushButton::clicked,this,[=](){

        ui->uploadBtn->setEnabled(false);
        this->downloadThread->setTcpServer(tcpServer);
        this->downloadThread->start();
    });
    connect(downloadThread,&DownloadThread::change,this,&DownloadView::dealChange);
    connect(this, &DownloadView::destroyed, this, &DownloadView::stopThread);
}

DownloadView::~DownloadView()
{
    delete ui;
    if(downloadThread){
        stopThread();
        delete downloadThread;
    }
}
void DownloadView::dealChange(int i)
{
    ui->progressBar->setValue(i);
}

void DownloadView::setTcpServer(TcpServer *tcp)
{
    this->tcpServer=tcp;
}
void DownloadView::stopThread()
{
    //停止线程
    downloadThread->quit();

    //等待线程处理完手头工作
    downloadThread->wait();
}
string DownloadView::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}
