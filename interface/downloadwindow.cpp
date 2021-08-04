#include "downloadwindow.h"
#include "ui_downloadwindow.h"

#include <QFileDialog>
#include <QMessageBox>
DownloadWindow::DownloadWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/picture/tb.ico"));
    messagelog=new MessageLog();
    connect(ui->savePathBtn,&QPushButton::clicked,[=](){
        QString savePath;
        savePath=QFileDialog::getExistingDirectory(this, "选择目录", QDir::currentPath(), QFileDialog::ShowDirsOnly);
        ui->lineEdit->setText(savePath);
        if(savePath==""){
           QMessageBox::critical(this,"错误","路径不能为空","确定");
        }else{
            tcpClient->setFilePath(qstr2str(savePath.toUtf8()));
            messagelog->setLogText("路径设置完毕");
            messagelog->startTimer();
            messagelog->show();
        }
    });
}
std::string DownloadWindow::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}
DownloadWindow::~DownloadWindow()
{
    if(messagelog){
        delete messagelog;
    }
    delete ui;
}
