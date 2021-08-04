#include "tcpserver.h"
#include <QDebug>

#define MAXBYTES 5*1024
mutex m;
//获取当前日期
TcpServer::TcpServer(QObject *parent) : QObject(parent)
{

}
void TcpServer::OBTION_TIME() {
    SYSTEMTIME start; //windows.h中
    GetLocalTime(&start);//time.h的tm结构体一样的效果
    qDebug() << start.wYear << "/" << start.wMonth << "/" << start.wDay << " " << start.wHour << ":" << start.wMinute << ":" << start.wSecond << endl;
}
//获取开始时间
double TcpServer::START_TIME() {
    DWORD start_time;
    start_time = GetTickCount64();
    return (double)start_time;
}
//获取结束时间
double TcpServer::END_TIME() {
    DWORD end_time;
    end_time = GetTickCount64();
    return double(end_time);
}
//获取文件大小
void  TcpServer::getByteSize(unsigned long long size) {
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
string TcpServer::TYPE_file() {
    string end_file = "";
    //char Temporary[1024] = { 0 };
    char file[1024] = { 0 };
    int index_last = 0;
    int ret = recv(GetclientSock(), file, 100, 0);
    for (int i = strlen(file) - 1; i >= 0; i--) {
        if (file[i] == '\\') {
            index_last = i;
            break;
        }
    }
    index_last++;
    if (ret > 0) {
        file[ret] = '\0';
        for (int i = index_last; i < strlen(file); i++) {
            end_file += file[i];
        }
    }
    return end_file;
}
void TcpServer::startSocket() {
    //加载网络库
    WSADATA wsaData;
    //第一个参数是winsocket load的版本号（2.2）
    if (WSAStartup(MAKEWORD(2, 3), &wsaData) != 0) {
        err("WSAStartup");
        return;
    }
    //创建服务器端的socket（协议族， sokcet类型）
    servSocket = socket(AF_INET, SOCK_STREAM, 0);//如果改成SOCK_DGRAM则使用UDP
    if (servSocket == INVALID_SOCKET) {
        err("SOCKET");
        return;
    }
    sockaddr_in servAddr; //服务器的socket地址，包含sin_addr表示IP地址，sin_port保持端口号和sin_zero填充字节
    memset(&servAddr, 0,  sizeof(SOCKADDR)); //初始化socket地址
    servAddr.sin_family = AF_INET; //设置使用的协议族
    servAddr.sin_port = htons(3725); //设置使用的端口
    servAddr.sin_addr.s_addr = INADDR_ANY; //define s_addr = S_un.S_addr
     //将之前创建的servSocket和端口，IP地址绑定

        if (::bind(servSocket, (SOCKADDR*)&servAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
            err("bind");
            return;
        }
        listen(servSocket, 1); //监听服务器端口
        sockaddr_in clntAddr;
        int nSize = sizeof(clntAddr);
        qDebug() << "等待连接..." << endl;
        clientSock = accept(servSocket, (SOCKADDR*)&clntAddr, &nSize);
        if (clientSock == INVALID_SOCKET) {
            err("accept");
        }
        qDebug() << "连接成功" << endl;


}
void TcpServer::REVER_file(string file, string filename) {
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
SOCKET TcpServer::GetclientSock() {
    return clientSock;
}
SOCKET TcpServer::GetservSocket() {
    return servSocket;
}
int TcpServer::MAIN_Server() {
    char Buffer[MAXBYTES] = { 0 }; // 文件缓冲区
    //char wb_files[MAXBYTE] = { 0 };
    FILE* fp = fopen(wb_file, "wb");
    //如果录入文件不存在的话就创建一个新的文件
    if (fp == NULL) {
        fp = fopen(wb_file, "w");
    }
    unsigned long long len_file = 0;
    if (fp == NULL) {
        qDebug() << "操作文件时出错" << endl;
        system("pause");
    }
    else {
        qDebug() << "接收文件时间: ";
        OBTION_TIME();
        unsigned long long g_fileSize = 0;
        char rev_buffer[MAXBYTES] = { 0 };//接收文件的长度
        int rev_len = recv(clientSock, rev_buffer, MAXBYTE, 0);
        if (rev_len > 0) {
            rev_buffer[rev_len] = '\0';
            for (int i = 0; i < strlen(rev_buffer); i++) {
                g_fileSize = g_fileSize * 10 + ((unsigned long long)rev_buffer[i] - 48);
            }
        }
        double start_time = START_TIME();
        memset(&Buffer, 0, MAXBYTES);
        unsigned long long  size = 0;
        //当成功接收文件（size > 0）时，判断写入的时候文件长度是否等于接收的长度
        while ((size = recv(clientSock, Buffer, MAXBYTES, 0)) > 0) {

            if (Buffer[size - 3] == 'e' && Buffer[size - 2] == 'n' && Buffer[size - 1] == 'd')
            {
                char buffer[MAXBYTES] = { 0 };
                for (int i = 0; i < strlen(Buffer) - 3; i++) {
                    buffer[i] = Buffer[i];
                }
                len_file += size - 3;
                size -= 3;
                if (fwrite(buffer, sizeof(char), size, fp) < size) {
                    qDebug() << "写入出错，部分文件缺失。" << endl;
                    break;
                }
//                qDebug().width(3);//i的输出为3位宽
//                if ((len_file * 100 / g_fileSize) % 5 > 0) {
//                    qDebug() << (len_file * 100 / g_fileSize) << "%";
//                    qDebug() << "\b\b\b\b";//回删三个字符，使数字在原地变化
//                }
                break;
            }
            else {
                if (fwrite(Buffer, sizeof(char), size, fp) < size) {
                    qDebug() << "写入出错，部分文件缺失。" << endl;
                    break;
                }
                len_file += size;
            }
//            qDebug().width(3);//i的输出为3位宽
//            if ((len_file * 100 / g_fileSize) % 5 > 0) {
//                qDebug() << (len_file * 100 / g_fileSize) << "%";
//                qDebug() << "\b\b\b\b";//回删三个字符，使数字在原地变化
//            }
            //清空缓存区以便下一次接收
            memset(&Buffer, 0, MAXBYTE);
        }
        qDebug() << "接收完成" << endl;
        qDebug() << "接受文件大小: ";
        len_file = (unsigned long long)len_file;
        getByteSize(len_file);
        qDebug() << "文件结束接受时间: ";
        OBTION_TIME();
        double end_time = END_TIME();
        double currentTime = 0;
        currentTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        qDebug() << "接收文件耗时: " << currentTime << "s" << endl;
        fclose(fp);
    }
    return 0;

}
void TcpServer::CLEAR() {
    //关闭socket，释放winsock
    closesocket(clientSock);
    closesocket(servSocket);

    ////关闭网络库
    if (WSACleanup()!=0) {
        err("WSACleanup");
        return;
    }
    qDebug() << "服务器连接已关闭。" << endl;
}

void TcpServer::receive_All()
{
    while(true){
        int length =0;
        int ret = recv(clientSock,(char*)&length, sizeof(length),0);
        //qDebug()<<"length="<<length;
        if(ret == sizeof(length)) {
            try{
                char* buf = new char[length];
                if(recv_once(clientSock,buf, length)){

                    do_handler(buf,length);
                }
                delete[] buf;
            }catch(std::exception& e) {
                qDebug()<<e.what();
            }
        }
    }
}

bool TcpServer::recv_once(SOCKET sck, char *buf, int length)
{
    int bytesReceivedOnce =0,bytesReceivedAll=0;
    while(bytesReceivedAll<length){
        if((bytesReceivedOnce=recv(sck,buf+bytesReceivedAll,length-bytesReceivedAll,0))<0){
            return false;
        }
        bytesReceivedAll+=bytesReceivedOnce;
    }
//    qDebug()<<"接受了"<<bytesReceivedAll<<"字节";
    return true;
}

void TcpServer::do_handler(char *buf, int length)
{
    LogicHeader& lh=*(LogicHeader*)buf;
    //qDebug()<<"lh.type="<<lh.type;
    switch(lh.type){
    case KEYBOARD_DATA:{

        KeyBoardSr& keyBoardSr=*(KeyBoardSr*)buf;

        keyBoard_handler(keyBoardSr.key);
        break;
    }
    case MOUSE_DATA:{

        MouseSr& mouseSr=*(MouseSr*)buf;
        mouse_handler(mouseSr.mouse);
        break;
    }
    case FILE_DATA:{
        //queFile->push(buf);
        FileHaderSr& fileSr=*(FileHaderSr*)buf;
//        qDebug()<<"fileSr.lh.log_raw_offset="<<fileSr.lh.log_raw_offset;
//        qDebug()<<"fileSr.lh.logic_raw_size="<<fileSr.lh.logic_raw_size;
        char* raw_data=buf+fileSr.lh.log_raw_offset;
        int length=fileSr.lh.logic_raw_size;
        FileHader &file=*(FileHader*)(buf+sizeof(LogicHeader));
        //emit receive_file(file,raw_data,length);
        file_handler(file,raw_data,length);
        break;
    }
    }
}

void TcpServer::mouse_handler(Mouse mouse)
{

    double dx=mouse.dx/(double)mouse.wx;
    double dy=mouse.dy/(double)mouse.wy;


    emit receive_mouse( dx, dy, mouse.mesage);
}

void TcpServer::keyBoard_handler(KeyBoard keyboard)
{
//    qDebug()<<"key:"<<keyboard.key;
//    qDebug()<<"空格"<<VK_SPACE;
    emit receive_keyBoard(keyboard.key);
}

void TcpServer::file_handler(FileHader file, char *buff, int length)
{
//    qDebug()<<"file.name"<<file.name;
//    qDebug()<<"file.datType"<<file.datType;
//    qDebug()<<"file.length"<<file.length;
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
    }else if(file.datType==3){
        emit requestFile();
    }
}

void TcpServer::setFilePath(string s)
{
    filePath=s;
}

void TcpServer::send_file(QString name, int type, char *buf, int length, uint64_t totalLength)
{
    if(type==1){
        char *buff=new char[sizeof(FileHaderCli)+length];
        memset(buff,0,sizeof(FileHaderCli));
        //qDebug()<<"sizeof(FileHaderCli)"<<sizeof(FileHaderCli);
        //qDebug()<<"sizeof(FileHaderCli)+length="<<sizeof(FileHaderCli)+length;
        FileHaderCli& file=*(FileHaderCli*)buff;
        file.file.datType=type;
        file.file.length=totalLength;
        string filename=qstr2str(name.toUtf8());
        strcpy_s(file.file.name, filename.c_str());
        file.lh.type=FILE_DATA;
        file.lh.logic_raw_size=length;
        file.lh.log_raw_offset=sizeof(file.lh)+sizeof(file.file);
        file.p.length=file.lh.log_raw_offset+file.lh.logic_raw_size;
        memcpy(buff+file.lh.log_raw_offset,buf,length);
        if(send(clientSock,buff, (file.p.length+sizeof(file.p)),0)<0){
            qDebug() << "传输出错，请检查网络配置。" << endl;
        }
        delete []buff;
        Sleep(1);
    }else{
        char* arr=new char[sizeof(FileHaderCli)];
        memset(arr,0,sizeof(FileHaderCli));
        FileHaderCli& file=*(FileHaderCli*)arr;
        //FileHaderCli file;
        file.file.datType=type;
        file.file.length=totalLength;
        string filename=qstr2str(name.toUtf8());
        strcpy_s(file.file.name, filename.c_str());
        qDebug()<<file.file.name;
        file.lh.type=FILE_DATA;
        file.lh.logic_raw_size=0;
        file.lh.log_raw_offset=sizeof(file.lh)+sizeof(file.file);
        file.p.length=file.lh.log_raw_offset+file.lh.logic_raw_size;
        FileHaderCli& files=*(FileHaderCli*)arr;
//        qDebug()<<files.file.datType;
//        qDebug()<<files.file.length;
//        qDebug()<<files.file.name;

        if(send_once(clientSock,arr,sizeof(FileHaderCli))){
            //qDebug()<<"发送成功!!";
        }else{
            //qDebug()<<"发送失败!!";
        }
        delete []arr;
    }
}

void TcpServer::send_image(int width,int heigth,QByteArray qba,int length)
{
    char*buff =new char[sizeof(ImageSr)+length];
    memset(buff, 0, sizeof(ImageSr)+length);
    ImageSr&image=*(ImageSr*)buff;
    image.image.width=width;
    image.image.heighth=heigth;
    image.lh.type=IMAGE_DATA;
    image.lh.logic_raw_size=length;
    image.lh.log_raw_offset=sizeof(image.lh)+sizeof(image.image);
    image.p.length=image.lh.logic_raw_size+image.lh.log_raw_offset;
    memcpy(buff+image.lh.log_raw_offset,qba.toBase64(),length);
    QByteArray q;
    q=q.fromBase64(buff+image.lh.log_raw_offset);
    send(clientSock,buff,image.p.length+sizeof(image.p),0);
    delete []buff;
}
bool TcpServer::send_once(SOCKET &sck, char *buf, int length)
{
    int bytesSentOnce=0,bytesSentAll=0;
    while(bytesSentAll<length){
        if((bytesSentOnce=send(sck,buf+bytesSentAll,length-bytesSentAll,0))<0){
            qDebug()<<"bytesSentOnce"<<bytesSentOnce;
            return false;
        }
        bytesSentAll+=bytesSentOnce;
    }
    return true;
}
string TcpServer::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}
