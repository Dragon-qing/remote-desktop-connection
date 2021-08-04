#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <WinSock2.h> //windows socket的头文件
#include <Windows.h>
#include <iostream>
#include <thread>
#include <process.h>
#include<mutex>
#include<string>
#include<process.h>
#define err(errMsg) printf("[error] %s failed,code %d\
line:%d\n",errMsg, WSAGetLastError(),__LINE__)
#pragma comment(lib, "ws2_32.lib") //连接winsock2.h的静态库文件

using namespace std;
class TcpClient : public QObject
{
    Q_OBJECT
public:
    enum {IMAGE_DATA,
          KEYBOARD_DATA,
          MOUSE_DATA,
          FILE_DATA
         };
public:
    explicit TcpClient(QObject *parent = nullptr);
    //获取ClientSock
    ~TcpClient();
    SOCKET Getcientsock();
    //获取当前日期
    void OBTION_TIME();
    //获取开始时间
    double START_TIME();
    //获取结束时间
    double END_TIME();
    //获取文件大小
    void  getByteSize(unsigned long long size);
    //返回以MB为单位的文件大小
    unsigned long long RETURN_MB(unsigned long long size);
    //判断输入IP是否存在
    bool TARGE_FILE(string ip);
    //输入文件
//    void SEND_FILE(string file);
    //LPVOID是一个没有类型的指针，也就是说你可以将任意类型的指针赋值给LPVOID类型的变量（一般作为参数传递）
    static DWORD WINAPI transmmit(const LPVOID arg);
    bool INPUT_IP(string ipt);
    //系统的实现以上函数
    int MAIN_SOCKET();
    //清理和关闭网络库
    void CLEAR();
    //处理接受到的信息
    void do_handler(char*buf,int length);
    //处理图片信息
    void image_handler(struct Image image,char*data,int length);
    //接受所有信息
    void receiveMessage();
    //一次性接受
    bool recv_once(SOCKET sock,char* buff,int length);
    //发送鼠标信息
    void send_mouse(int message, int dx, int dy,int wx,int wy);
    //发送键盘信息
    void send_keyBoard(int key);
    //一次性发送
    bool send_once(SOCKET &sck, char *buf, int length);
    //发送文件
    void send_file(QString name,int type, char*buf,int length,uint64_t totalLength);
    string qstr2str(const QString qstr);
    void REVER_file(string file, string filename);
    void setFilePath(string str);
    void file_handler(struct FileHader file, char *buff, int length);
signals:
    void receive_image(QPixmap *pix);
    void change(int i);

private:
    SOCKET clientSock;
    int id;
    string ip;
    char wb_file[MAXBYTE];
    string filePath;//文件保存路径(文件夹)
    FILE* fp;
    //QPixmap pix;
};



//************************************
//***********业务协议******************
typedef struct Potcol {
    int length; //代表整个包后数据的长度
} Potcol;

typedef struct LogicHeader {
    int type; //代表业务类别，结合任务，可以定义:0(IMAGE_DATA) 为图像数据，1(KEYBOARD_DATA)为键盘数据，2(MOUSE_DATA)为鼠标数据，3(FILE_DATA)为文件数据
    int logic_raw_size; //业务存在要发送裸数据时，裸数据的长度
    int log_raw_ofset; // 业务存在要发送裸数据时，裸数据的偏移
}LogicHeader;

typedef struct Image{ //图像业务，存在裸数据
    int width;
    int height;
    char dat[0]; //可以通过定义一个0长度的数据来标识存在裸数据，增加可读性
} Image;

typedef struct KeyBoard{
    int key;
}KeyBoard;

typedef struct Mouse{
    int mesage;
    int dx;
    int dy;
    int wx;
    int wy;
}Mouse;

typedef struct FileHader{ //文件业务存在裸数据
    int datType; //此文件包的类别，0 为打开文件，1为数据，2为结束,3请求下载文件
    uint64_t length;
    char name[64]; //文件名称
    //char raw[0];
}FileHader;

typedef struct ImageSr{
    Potcol p;
    LogicHeader lh;
    Image image;
}ImageSr;

typedef struct ImageCli{
    LogicHeader lh;
    Image image;
}ImageCli;

typedef struct KeyBoardSr{
    LogicHeader lh;
    KeyBoard key;
}KeyBoardSr;

typedef struct KeyBoardCli{
    Potcol p;
    LogicHeader lh;
    KeyBoard key;
}KeyBoardCli;

typedef struct MouseSr{
    LogicHeader lh;
    Mouse mouse;
}MouseSr;

typedef struct MouseCli{
    Potcol p;
    LogicHeader lh;
    Mouse mouse;
}MouseCli;

typedef struct FileHaderSr{
    LogicHeader lh;
    FileHader file;
}FileHaderSr;

typedef struct FileHaderCli{
    Potcol p;
    LogicHeader lh;
    FileHader file;
}FileHaderCli;
#endif // TCPCLIENT_H
