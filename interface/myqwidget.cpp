#include "myqwidget.h"
#include "ui_myqwidget.h"
#include<QPainter>
#include<QIcon>
#include <QPushButton>
#include <QFileDialog>
//中间的按钮悬浮窗（太菜了，没办法在最大化时让它依然处于中间）
myQWidget::myQWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myQWidget)
{
    ui->setupUi(this);
    ui->mqw_button1->setStyleSheet("border:none;");//按钮无边框
    ui->mqw_button2->setStyleSheet("border:none;");
    ui->mqw_button3->setStyleSheet("border:none;");
    ui->mqw_button4->setStyleSheet("border:none;");
    ui->mqw_button5->setStyleSheet("border:none;");
    //查看按钮
    QIcon icon1(":/picture/picture1.jpeg");//按钮图片
    ui->mqw_button1->setIcon(icon1);
    ui->mqw_button1->setIconSize(QSize(60,60));
    //控制按钮
    QIcon icon2(":/picture/picture2.jpeg");
    ui->mqw_button2->setIcon(icon2);
    ui->mqw_button2->setIconSize(QSize(60,60));
    //上传按钮
    QIcon icon3(":/picture/picture3.jpeg");
    ui->mqw_button3->setIcon(icon3);
    ui->mqw_button3->setIconSize(QSize(60,60));

    //下载按钮
    QIcon icon4(":/picture/picture4.jpeg");
    ui->mqw_button4->setIcon(icon4);
    ui->mqw_button4->setIconSize(QSize(60,60));

    //退出按钮
    QIcon icon5(":/picture/picture5.jpeg");
    ui->mqw_button5->setIcon(icon5);
    connect(ui->mqw_button5,&QPushButton::clicked,[=](){
        exit(0);
    });
    ui->mqw_button5->setIconSize(QSize(60,60));
}

myQWidget::~myQWidget()
{
    delete ui;
}

QPushButton *myQWidget::getDisplayBtn()
{
    return ui->mqw_button1;
}

QPushButton *myQWidget::getControlBtn()
{
    return ui->mqw_button2;
}

QPushButton *myQWidget::getUploadBtn()
{
    return ui->mqw_button3;
}

QPushButton *myQWidget::getDownloadBtn()
{
    return ui->mqw_button4;
}

QPushButton *myQWidget::getExitBtn()
{
    return ui->mqw_button5;
}
//背景色（灰黑色）
void QWidget::paintEvent(QPaintEvent* event){
  QPainter painter(this);
  painter.fillRect(rect(),QColor(53,53,53,53));
}
