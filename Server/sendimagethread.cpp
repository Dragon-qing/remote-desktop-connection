#include "sendimagethread.h"

SendImageThread::SendImageThread(QObject *parent) : QThread(parent)
{

}

void SendImageThread::run()
{
    //tcpServer->send_image(image->width(),image->height(),bytes,bytes->toBase64().length());
}
