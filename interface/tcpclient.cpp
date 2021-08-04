#include "tcpclient.h"
#include <QApplication>
#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QPixmap>
#include <QScreen>
#define MAXBYTES 5*1024
//表示一秒钟会有多少个时钟计时单元
#define CLOCKS_PER_SEC ((clock_t)1000)
mutex m;
//获取当前日期
TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}

TcpClient::~TcpClient()
{

}
void TcpClient::OBTION_TIME() {
    SYSTEMTIME start; //windows.h中
    GetLocalTime(&start);//time.h的tm结构体一样的效果
    qDebug() << start.wYear << "/" << start.wMonth << "/" << start.wDay << " " << start.wHour << ":" << start.wMinute << ":" << start.wSecond << endl;
}
// 获取开始时间
double TcpClient::START_TIME() {
    DWORD start_time;
    start_time = GetTickCount64();
    return (double)start_time;
}
//获取结束时间
double TcpClient::END_TIME() {
    DWORD end_time;
    end_time = GetTickCount64();
    return double(end_time);
}
SOCKET TcpClient::Getcientsock() {
    return clientSock;
}
//获取文件大小
void TcpClient::getByteSize(unsigned long long size) {
    unsigned long long rest = 0;
    if (size < 1024) {
        qDebug() << size << "B" << endl;
        return;
    }
    else {
        size /= 1024;
    }
    if (size < 1024) {
        qDebug() << size << "KB" << endl;
        return;
    }
    else {
        rest = size % 1024;
        size /= 1024;
    }
    if (size < 1024) {
        size *= 100;
        qDebug() << (size / 100) << "." << (rest * 100 / 1024 % 100) << "MB" << endl;
        return;
    }
    else {
        size = size * 100 / 1024;
        qDebug() << (size / 100) << "." << (size % 100) << "GB" << endl;
        return;
    }
}

//void TcpClient::SEND_FILE(string file) {
//    int i = 0;
//    char Temporary_file[MAXBYTE] = { 0 };//保存发送文件的格式
//    for (i = 0; i < file.length(); i++) {
//        wb_file[i] = file[i];
//        Temporary_file[i] = file[i];
//    }
//    wb_file[i] = '\0';
//    Temporary_file[i] = '\0';
//    send(clientSock, Temporary_file, strlen(Temporary_file), 0);
//}
//LPVOID是一个没有类型的指针，也就是说你可以将任意类型的指针赋值给LPVOID类型的变量（一般作为参数传递）
DWORD WINAPI TcpClient::transmmit(const LPVOID arg) {
    //上锁是为了方便看输出
    m.lock();

    //F* temp = (F*)arg;
    TcpClient* so = (TcpClient*)arg;
    /*
    获取文件的序号int file_id = temp->id;
    获取客户机的端口号ntohs(temp -> clientAddr.sin_port);
    */
    qDebug() << "测试开始,等待服务端发送消息..." << endl;
    //从客户端处接受数据
    /*
        char Buffer[MAXBYTE] = { 0 }; //缓冲区
        recv(temp->clientSocket, Buffer, MAXBYTE, 0); //recv方法 从客户端通过clientScocket接收
        qDebug() << "线程" << temp->id << "从客户端的" << ntohs(temp->clientAddr.sin_port) << "号端口收到:" << Buffer << endl;
    */
    char* file_name; //文件路径
    char File_Alias[100] = { 0 };
    file_name = so->wb_file;
    unsigned long long len_file = 0;
    FILE* fp = fopen(file_name, "rb");

    if (fp == NULL) {
        qDebug() << "文件" << file_name << "出错或不存在" << endl;
    }
    else {
        //获取文件大小
        unsigned long long g_fileSize;//注意这个地方不能使用unsigned long long,因为当文件传输很大的时候，ftell返回的是long
        //fseek(fp, 0, SEEK_END);//将读取的文件指针放到文件末尾
        //g_fileSize = ftell(fp);
        //fseek(fp, 0, SEEK_SET);//指针移到文件开头
        struct _stat64 st;
        _stat64(file_name, &st);
        g_fileSize = st.st_size;
//        string send_file_len;
//        send_file_len = to_string(g_fileSize);
//        send(so->clientSock, send_file_len.c_str(), send_file_len.length(), 0);
        qDebug() << "发送文件时间: ";
        so->OBTION_TIME();
        double start_time = so->START_TIME();
        char Buffer[MAXBYTES] = { 0 }; //文件缓冲区
        unsigned long long  size = 0; //读取的文件长度
        //unsigned long long Actual_file_len = 0;//为了确保长度更精确
        while ((size = fread(Buffer, sizeof(char), MAXBYTES, fp)) > 0) {
            //返回非0值表示send错误
            if (send(so->clientSock, Buffer, (unsigned long long)size, NULL) < 0)
            {
                qDebug() << "传输出错，请检查网络配置。" << endl;
                break;
            }
            len_file += size;

//            qDebug().width(3);//i的输出为3位宽
//            if ((len_file * 100 / g_fileSize) % 5 > 0) {
//                qDebug() << (len_file * 100 / g_fileSize) << "%";
//                qDebug() << "\b\b\b\b";//回删三个字符，使数字在原地变化
//            }
            size = 0;
            //每次读取完之后清空缓存区，以便下一块文件读入
            memset(&Buffer, 0, MAXBYTES);
        }
        const char* t = "end";
        send(so->clientSock, t, strlen(t), NULL);
        qDebug() << so->id << "线程已成功发送" << file_name << endl;

        qDebug() << "发送文件大小: ";
        so->getByteSize(len_file);
        qDebug() << "文件发送结束时间: ";
        so->OBTION_TIME();
        double end_time = so->END_TIME();
        double currentTime = 0;
        currentTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        qDebug() << "发送文件耗时: " << currentTime << "s" << endl;
        fclose(fp);
    }


    m.unlock();

    return 0;
}
bool TcpClient::INPUT_IP(string ipt) {
    //客户端socket
    //加载winsock库
    WSADATA wsadata;
    //WSA-windows socket ansyc windows的异步套接字 2.2版本的

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        err("WSAStartup");
        return 0;
    }
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock == INVALID_SOCKET) {
        err("SOCKET");
        return 0;
    }
    //初始化socket信息
    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(SOCKADDR));
    //clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    const char* ips = ipt.c_str();
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(3725);//将本地字节序转换为网络字节序，大端和小端存储
    clientAddr.sin_addr.s_addr = inet_addr(ips);
    if (::connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        err("connect");
        return false;
    }
    return true;
}

//接收和传输文件目录
bool TcpClient::TARGE_FILE(string ip) {
    bool flag = INPUT_IP(ip);
    if (flag == true)return 1;
    else {
        return 0;
    }
}
int TcpClient::MAIN_SOCKET() {
    //建立连接
    //while (true) {
    qDebug() << "已建立连接。" << endl;
    //char Buffer[MAXBYTE] = { 0 }; // 文件缓冲区
   // char wb_file[100] = { 0 }; //写入的文件
    //“句柄” 类似指针, 但通过指针可读写对象, 通过句柄只是使用对象;
    HANDLE hThread[2];
    for (int i = 0; i < 1; i++) {
        sockaddr_in clntAddr;
        memset(&clntAddr, 0, sizeof(SOCKADDR));
        //使用 API 的 CreateThread, 它执行完入口函数后会自动退出, 无需 ExitThread;
        hThread[i] = CreateThread(NULL, 0, &transmmit, this, 0, NULL);
    }
    //等待子线程完成
    WaitForMultipleObjects(1, hThread, TRUE, INFINITE);
    qDebug() << "错误代码: " << WSAGetLastError() << endl;

    //}

    return 0;
}
void TcpClient::CLEAR() {
    closesocket(clientSock);
    ////关闭网络库
    if (WSACleanup() != 0) {
        err("WSACleanup");
        return;
    }
    qDebug() << "客户端连接已关闭。" << endl;

}
void TcpClient::receiveMessage()
{
    while(true){
        int length =0;
        int ret = recv(clientSock,(char*)&length, sizeof(length),0);
        if(ret == sizeof(length)) {
            try{
                char* buf = new char[length];
                if(recv_once(clientSock,buf, length)){
                    do_handler(buf,length);
                }
                delete[] buf;
            }catch(std::exception& e) {
                   err(e.what());
            }
        }
    }
}
bool TcpClient::recv_once(SOCKET sock,char* buff,int length)
{
    int bytesReceivedOnce =0,bytesReceivedAll=0;
    while(bytesReceivedAll<length){
        if((bytesReceivedOnce=recv(sock,buff+bytesReceivedAll,length-bytesReceivedAll,0))<0){
            return false;
        }
        bytesReceivedAll+=bytesReceivedOnce;
    }
    return true;
}

void TcpClient::send_mouse(int message, int dx, int dy,int wx,int wy)
{
    MouseCli mouse;
    mouse.lh.type=MOUSE_DATA;
    mouse.lh.logic_raw_size=0;
    mouse.lh.log_raw_ofset=sizeof(LogicHeader)+sizeof(Mouse);
    mouse.mouse.mesage=message;
    mouse.mouse.dx=dx;
    mouse.mouse.dy=dy;
    mouse.mouse.wx=wx;
    mouse.mouse.wy=wy;
    mouse.p.length=mouse.lh.log_raw_ofset+mouse.lh.logic_raw_size;
    send_once(clientSock,(char*)&mouse,sizeof(mouse));
}

void TcpClient::send_keyBoard(int key)
{
    KeyBoardCli keyBoard;
    keyBoard.lh.type=KEYBOARD_DATA;
    keyBoard.lh.logic_raw_size=0;
    keyBoard.lh.log_raw_ofset=sizeof(LogicHeader)+sizeof(KeyBoard);
    keyBoard.key.key=key;
    keyBoard.p.length=keyBoard.lh.log_raw_ofset+keyBoard.lh.logic_raw_size;
    send_once(clientSock,(char*)&keyBoard,sizeof (keyBoard));
}

bool TcpClient::send_once(SOCKET &sck, char *buf, int length)
{
    int bytesSentOnce=0,bytesSentAll=0;
    while(bytesSentAll<length){
        if((bytesSentOnce=send(sck,buf+bytesSentAll,length-bytesSentAll,0))<0){
            //qDebug()<<"bytesSentOnce"<<bytesSentOnce;
            return false;
        }
        bytesSentAll+=bytesSentOnce;
    }
    return true;
}

void TcpClient::send_file(QString name,int type, char *buf, int length,uint64_t totalLength)
{

    if(type==1){
        char *buff=new char[sizeof(FileHaderCli)+length];
        memset(buff,0,sizeof(FileHaderCli));
        FileHaderCli& file=*(FileHaderCli*)buff;
        file.file.datType=type;
        file.file.length=totalLength;
        string filename=qstr2str(name.toUtf8());
        strcpy_s(file.file.name, filename.c_str());
        file.lh.type=FILE_DATA;
        file.lh.logic_raw_size=length;
        file.lh.log_raw_ofset=sizeof(file.lh)+sizeof(file.file);
        file.p.length=file.lh.log_raw_ofset+file.lh.logic_raw_size;
        memcpy(buff+file.lh.log_raw_ofset,buf,length);
        //qDebug()<<"file.p.length+sizeof(file.p)"<<file.p.length+sizeof(file.p);
        if(send(clientSock,buff, (file.p.length+sizeof(file.p)),0)<0){
            qDebug() << "传输出错，请检查网络配置。" << endl;
        }else{
            //qDebug()<<"发送成功";
        }
        delete []buff;
        Sleep(4);
    }else{
        char* arr=new char[sizeof(FileHaderCli)];
        memset(arr,0,sizeof(FileHaderCli));
        FileHaderCli& file=*(FileHaderCli*)arr;
        file.file.datType=type;
        file.file.length=totalLength;
        string filename=qstr2str(name.toUtf8());
        strcpy_s(file.file.name, filename.c_str());
       // qDebug()<<file.file.name;
        file.lh.type=FILE_DATA;
        file.lh.logic_raw_size=0;
        file.lh.log_raw_ofset=sizeof(file.lh)+sizeof(file.file);
        file.p.length=file.lh.log_raw_ofset+file.lh.logic_raw_size;
        FileHaderCli& files=*(FileHaderCli*)arr;
       // qDebug()<<files.file.datType;
       // qDebug()<<files.file.length;
       // qDebug()<<files.file.name;

        if(send_once(clientSock,arr,sizeof(FileHaderCli))){
            //qDebug()<<"发送成功!!";
        }else{
            //qDebug()<<"发送失败!!";
        }
        delete []arr;
    }
}
void TcpClient::do_handler(char*buf,int length)
{
    LogicHeader& lh=*(LogicHeader*)buf;
    switch(lh.type){
    case IMAGE_DATA:{
        ImageCli& imagecli=*(ImageCli*)buf;
        char* raw_data=buf+imagecli.lh.log_raw_ofset;
        int length=imagecli.lh.logic_raw_size;
        image_handler(imagecli.image,raw_data,length);
        break;
    }
    case FILE_DATA:{
        FileHaderSr& fileSr=*(FileHaderSr*)buf;
        char* raw_data=buf+fileSr.lh.log_raw_ofset;
        int length=fileSr.lh.logic_raw_size;
        FileHader &file=*(FileHader*)(buf+sizeof(LogicHeader));
        file_handler(file,raw_data,length);
        break;
    }
    }
}
void TcpClient::file_handler(struct FileHader file, char *buff, int length)
{
    if(file.datType==0){
           REVER_file(filePath,string(file.name));
           fp = fopen(wb_file, "wb");
        //如果录入文件不存在的话就创建一个新的文件
        if (fp == NULL) {
            fp = fopen(wb_file, "w");
        }
        if (fp == NULL) {
            qDebug() << "操作文件时出错" ;
        }
    }else if(file.datType==1){

        if (fwrite(buff-4, sizeof(char), length, fp) < length) {
            qDebug() << "写入出错，部分文件缺失。" ;
        }
    }else if(file.datType==2){
        qDebug() << "接收完成" ;
        fclose(fp);
    }
}
void TcpClient::image_handler(struct Image image,char*data,int length)
{
    QPixmap *pix=new QPixmap;
    QByteArray q;
    q=q.fromBase64(data-4);
    pix->loadFromData(q,"jpg");
    emit receive_image(pix);
}
string TcpClient::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}
void TcpClient::REVER_file(string file, string filename) {
    int i = 0;
    int len_file = file.length();
    int len_filename = filename.length();
    for (i = 0; i < len_file; i++) {
        wb_file[i] = file[i];
    }
    int j = 0;
    wb_file[len_file]='/';
    for (i = len_file+1; i < (len_file+1 + len_filename) && j < len_filename; j++, i++) {
        wb_file[i] = filename[j];
    }
    wb_file[i] = '\0';
}

void TcpClient::setFilePath(string str)
{
    this->filePath=str;
}
