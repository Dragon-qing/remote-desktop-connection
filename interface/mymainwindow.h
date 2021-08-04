#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H
#include "mainwindow.h"
#include "messagelog.h"
#include <QMainWindow>
namespace Ui {
class myMainWindow;
}

class myMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit myMainWindow(QWidget *parent = nullptr);
    ~myMainWindow();
    MainWindow *w;

private:
    Ui::myMainWindow *ui;
    bool isConnected=false;
    MessageLog *m;
    TcpClient *tcpClient;
};

#endif // MYMAINWINDOW_H
