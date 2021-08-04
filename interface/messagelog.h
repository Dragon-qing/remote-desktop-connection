#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <QDialog>
//右下角消息窗口
namespace Ui {
class MessageLog;
}

class MessageLog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageLog(QWidget *parent = nullptr);
    ~MessageLog();
    void setLogText(QString s);
    void startTimer();
private:
    Ui::MessageLog *ui;
    QTimer *timer;
};

#endif // MESSAGELOG_H
