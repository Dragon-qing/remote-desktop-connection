#ifndef MESSAGEDLG_H
#define MESSAGEDLG_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#pragma comment(lib,"User32.lib")
namespace Ui {
class MessageDlg;
}

class MessageDlg : public QWidget
{
    Q_OBJECT
public:
    //动画模式枚举
    enum AnimationMode
    {
        //无动画
        NoAnimation = 0x00,
        //仅透明度动画
        OpacityAnimation = 0x01,
        //仅位置动画
        PosAnimation = 0x02,
        //全部动画
        //OpacityAnimation|PosAnimation
        AllAnimation = 0xFF
    };

public:
    explicit MessageDlg(QWidget *parent = nullptr);
    ~MessageDlg();
    //显示弹框-已显示动画重新开始,timeout<=0不会定时消失
    static void showTip(const QString &title, const QString &texts, int timeout);
    //显示弹框-已显示不重复动画
    static void keepTip(const QString &texts);
    //隐藏弹框
    static void hideTip();
    //设置动画模式
    static MessageDlg::AnimationMode getMode();
    static void setMode(MessageDlg::AnimationMode newMode);
    double getDpi();
protected:
    void paintEvent(QPaintEvent *event);
private:
    //初始化动画设置
    void initAnimation();
    //初始化定时器设置
    void initTimer();
    //准备定时器
    void readyTimer(int timeout);
    //启动显示动画-已显示动画重新开始
    void showAnimation();
    //启动显示动画-已显示不重复动画
    void keepAnimation();
    //启动隐藏动画
    void hideAnimation();
private:
    Ui::MessageDlg *ui;
    //唯一实例
    static MessageDlg *instance;

    //动画设置
    static AnimationMode mode;
    //动画组
    QParallelAnimationGroup *showGroup;
    //保存动画结束状态
    bool showAnimEnd = false;
    //透明度属性动画
    QPropertyAnimation *showOpacity = nullptr;
    //位置属性动画
    QPropertyAnimation *showPos = nullptr;

    //定时关闭
    QTimer *hideTimer = nullptr;
    //定时计数
    int hideCount = 0;
    //缩放比例，适配高分屏
    double m_dpi;
    QGraphicsDropShadowEffect *m_pEffect;
};

#endif // MESSAGEDLG_H
