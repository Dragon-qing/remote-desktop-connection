#include "messagedlg.h"
#include "ui_messagedlg.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QPushButton>
#include <windows.h>
#include <QLibrary>
#define SHADOW_WIDTH 10 //边框阴影宽度
MessageDlg* MessageDlg::instance = nullptr;
MessageDlg::AnimationMode MessageDlg::mode = MessageDlg::AllAnimation;
MessageDlg::MessageDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageDlg),
    showGroup(new QParallelAnimationGroup(this))
{
    ui->setupUi(this);
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground,true);//背景透明
    setAttribute(Qt::WA_DeleteOnClose);
    //setWindowModality(Qt::WindowModal);

    ui->btnClose->setIcon(QIcon(":/YozoUCloud/Resources/Main/window_close_n.png"));

    //缩放比适配
    m_dpi = getDpi();
    setFixedSize(300 * m_dpi, 160 * m_dpi);
    QFont font;
    font.setPixelSize(14*m_dpi); //字体基础是14
    font.setFamily(QString::fromLocal8Bit("微软雅黑"));
    ui->titleLabel->setFont(font);
    ui->contentLabel->setFont(font);
    ui->countLabel->setFont(font);
    //添加阴影效果
    m_pEffect = new QGraphicsDropShadowEffect(this);//该类提供了图形元素的阴影效果，用于增加立体感。
    m_pEffect->setOffset(0, 0);//用于设定在哪个方向产生阴影效果，如果dx为负数，则阴影在图形元素的左边
    m_pEffect->setColor(Qt::gray);//用于设定阴影的颜色
    m_pEffect->setBlurRadius(20);//用于设定阴影的模糊度
    ui->frame->setGraphicsEffect(m_pEffect);
    ui->verticalLayout->setContentsMargins(10,10,10,0);//设置frame和主窗口的距离，也就是阴影的距离

    //关闭按钮事件绑定
    connect(ui->btnClose, &QPushButton::clicked, this, &MessageDlg::hideTip);
    //程序退出时释放
    connect(qApp, &QApplication::aboutToQuit, this, &MessageDlg::close);
    //动画初始化设置
    initAnimation();
    //定时器设置
    initTimer();
}

MessageDlg::~MessageDlg()
{
    if (ui){
        delete ui;
        ui = NULL;
    }
    if (showGroup){
        delete showGroup;
        showGroup = NULL;
    }
    if (hideTimer){
        delete hideTimer;
        hideTimer = NULL;
    }
    if (m_pEffect){
        delete m_pEffect;
        m_pEffect = NULL;
    }

}
double MessageDlg::getDpi()

{

    double dDpi = 1;
    // Get desktop dc
    HDC desktopDc = GetDC(NULL);
    // Get native resolution
    float horizontalDPI = ::GetDeviceCaps(desktopDc, LOGPIXELSX);
    float verticalDPI = ::GetDeviceCaps(desktopDc, LOGPIXELSY);

    int dpi = (horizontalDPI + verticalDPI) / 2;
    dDpi = 1 + ((dpi - 96) / 24)*0.25;
    //为了保证页面显示正常，暂时不支持小于1和大于2的缩放系数
    if (dDpi < 1)
    {
        dDpi = 1;
    }
    return dDpi;
}
void MessageDlg::showTip(const QString &title, const QString &texts, int timeout)
{

        if (!instance){
            //仅在ui线程
            instance = new MessageDlg();
        }
        instance->readyTimer(timeout);
        //模态框
        instance->setWindowModality(Qt::WindowModal);
        instance->ui->contentLabel->setText(texts);
        instance->ui->titleLabel->setText(title);
        instance->showAnimation();



}

void MessageDlg::keepTip(const QString &texts)
{

        if (!instance){
            //仅在ui线程
            instance = new MessageDlg;
        }
        instance->readyTimer(0);
        //模态框
        instance->setWindowModality(Qt::WindowModal);
        instance->ui->contentLabel->setText(texts);
        instance->keepAnimation();


}


//关闭按钮点击事件
void MessageDlg::hideTip()
{

        if (!instance){
            return;
        }
        instance->ui->countLabel->hide();
        instance->hideAnimation();

}

MessageDlg::AnimationMode MessageDlg::getMode()
{
    return mode;
}

void MessageDlg::setMode(MessageDlg::AnimationMode newMode)
{
    if (mode != newMode){
        mode = newMode;
    }
}

void MessageDlg::initAnimation()
{

    //透明度动画
    showOpacity = new QPropertyAnimation(this, "windowOpacity");
    //判断是否设置了此模式的动画
    if (mode&AnimationMode::OpacityAnimation){
        showOpacity->setDuration(1500);
        showOpacity->setStartValue(0);
    }
    else{
        showOpacity->setDuration(0);
        showOpacity->setStartValue(1);
    }
    showOpacity->setEndValue(1);
    showGroup->addAnimation(showOpacity);

    //位置动画
    showPos = new QPropertyAnimation(this, "pos");
    QScreen * screen = QGuiApplication::primaryScreen();
    if (screen) {
        const QRect desk_rect = screen->availableGeometry();
        const QPoint hide_pos{ desk_rect.width() - this->width(),
            desk_rect.height() };
        const QPoint show_pos{ desk_rect.width() - this->width(),
            desk_rect.height() - this->height() };
        //判断是否设置了此模式的动画
        if (mode&AnimationMode::PosAnimation){
            showPos->setDuration(1500);
            showPos->setStartValue(hide_pos);
        }
        else{
            showPos->setDuration(0);
            showPos->setStartValue(show_pos);
        }
        showPos->setEndValue(show_pos);
    }
    showGroup->addAnimation(showPos);
    //
    connect(showGroup, &QParallelAnimationGroup::finished, [this]{
        //back消失动画结束关闭窗口
        if (showGroup->direction() == QAbstractAnimation::Backward){
            //Qt::WA_DeleteOnClose后手动设置为null
            instance = nullptr;
            qApp->disconnect(this);
            //关闭时设置为非模态，方式主窗口被遮挡，待测试
            this->setWindowModality(Qt::NonModal);
            this->close();
        }
        else{
            //配合keepAnimation
            showAnimEnd = true;
            //配合定时关闭
            if (hideCount>0)
                hideTimer->start();
        }
    });



}

void MessageDlg::initTimer()
{

    hideTimer = new QTimer(this);
    hideTimer->setInterval(1000); //1s间隔
    connect(hideTimer, &QTimer::timeout, [this]{
        if (hideCount>1){
            hideCount--;
            ui->countLabel->setText(QString::fromLocal8Bit("%1s后自动关闭").arg(hideCount));
        }
        else{
            ui->countLabel->hide();
            hideTimer->stop();
            hideTip();
        }
    });

}

void MessageDlg::readyTimer(int timeout)
{

        //先设置，在显示动画结束再start开始计时器
        hideCount = timeout;
        hideTimer->stop();

        if (hideCount>0){
            ui->countLabel->setText(QString::fromLocal8Bit("%1s后自动关闭").arg(hideCount));
        }
        else{
            ui->countLabel->hide();
        }

}

void MessageDlg::showAnimation()
{

        showGroup->setDirection(QAbstractAnimation::Forward);
        if (showGroup->state() == QAbstractAnimation::Running)
        {
            showGroup->stop();   //停止正在进行的动画重新
        }
        showGroup->start();
        show();


}

void MessageDlg::keepAnimation()
{

        //show没有完成，或者正在动画中才进入
        if (!showAnimEnd || showGroup->state() != QAbstractAnimation::Stopped){
            showGroup->setDirection(QAbstractAnimation::Forward);
            showGroup->start();
            show();
        }

}
void MessageDlg::hideAnimation()
{

    //Backward反向执行动画
    showGroup->setDirection(QAbstractAnimation::Backward);
    showGroup->start();

}
void MessageDlg::paintEvent(QPaintEvent *event)
{

}
