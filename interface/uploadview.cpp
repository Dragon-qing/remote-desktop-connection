#include "uploadview.h"
#include "ui_uploadview.h"

#include <QFileDialog>
#include <QMessageBox>
UploadView::UploadView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UploadView)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/picture/tb.ico"));
    uploadThread=new UploadThread;
    ui->uploadBtn->setEnabled(false);
    connect(ui->selectBtn,&QPushButton::clicked,this,[=](){
        filePath=QFileDialog::getOpenFileName(this,QString::fromUtf16(u"选择文件"),QDir::homePath());
        ui->fileLine->setText(filePath);
        if(filePath==""){
            QMessageBox::critical(this,"error","The file address cannot be empty");
        }else{
            uploadThread->setFileAbsPath(filePath);
            ui->progressBar->setValue(0);
            ui->uploadBtn->setEnabled(true);
        }
    });
    connect(ui->uploadBtn,&QPushButton::clicked,this,[=](){

        ui->uploadBtn->setEnabled(false);
        this->uploadThread->setTcpClient(tcpClient);
        this->uploadThread->start();
    });
    connect(uploadThread,&UploadThread::change,this,&UploadView::dealChange);
    connect(this, &UploadView::destroyed, this, &UploadView::stopThread);
}

UploadView::~UploadView()
{
    delete ui;
    if(uploadThread){
        stopThread();
        delete uploadThread;
    }
}
void UploadView::dealChange(int i)
{
    ui->progressBar->setValue(i);
}

void UploadView::setTcpClient(TcpClient *tcp)
{
    this->tcpClient=tcp;
}
void UploadView::stopThread()
{
    //停止线程
    uploadThread->quit();

    //等待线程处理完手头工作
    uploadThread->wait();
}
string UploadView::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}
