#ifndef MYQWIDGET_H
#define MYQWIDGET_H

#include <QPushButton>
#include <QWidget>

namespace Ui {
class myQWidget;
}

class myQWidget : public QWidget
{
    Q_OBJECT

public:
    explicit myQWidget(QWidget *parent = nullptr);
    ~myQWidget();
    QPushButton* getDisplayBtn();
    QPushButton* getControlBtn();
    QPushButton* getUploadBtn();
    QPushButton* getDownloadBtn();
    QPushButton* getExitBtn();
private:
    Ui::myQWidget *ui;
signals:
    void displayable();
};

#endif // MYQWIDGET_H
