#include "downloadthread.h"
#include <QDebug>
#define MAXBYTES 8*1024
DownloadThread::DownloadThread(QObject *parent) : QThread(parent)
{

}
string DownloadThread::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}

void DownloadThread::setFileAbsPath(QString str)
{
    this->fileAbsPath=str;
}
void DownloadThread::run()
{
    QString name;
    FILE* fp = fopen(qstr2str(fileAbsPath.toUtf8()).c_str(), "rb");
    if(fp==NULL){
        qDebug()<<"文件"<<fileAbsPath<<"出错或不存在"<<endl;
    }else{
        name=fileAbsPath.mid(fileAbsPath.lastIndexOf("/")+1);
        struct _stat64 st;
        uint64_t g_fileSize;
        _stat64(qstr2str(fileAbsPath.toUtf8()).c_str(), &st);
        g_fileSize = st.st_size;
        tcpServer->send_file(name,0,nullptr,0,g_fileSize);
        char Buffer[MAXBYTES] = { 0 }; //文件缓冲区
        unsigned long long  sizeRead = 0; //读取的文件长度
        uint64_t len_file=0;
        while ((sizeRead = fread(Buffer, sizeof(char), MAXBYTES, fp)) > 0) {
            //返回非0值表示send错误
            tcpServer->send_file(name,1,Buffer,sizeRead,g_fileSize);

            len_file += sizeRead;

//            qDebug().width(3);//i的输出为3位宽
            if ((len_file * 100 / g_fileSize) % 5 > 0) {
                emit change((int)(len_file * 100 / g_fileSize)+1);
            }
            sizeRead = 0;
            //每次读取完之后清空缓存区，以便下一块文件读入
            memset(&Buffer, 0, MAXBYTES);
        }
        tcpServer->send_file(name,2,nullptr,0,g_fileSize);
        qDebug()<<"发送完成";
    }
    fclose(fp);
    emit isDone();
}
