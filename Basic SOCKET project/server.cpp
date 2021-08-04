#include<thread>
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <cstring>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
//全局变量区，创建最大连接数，创建SOKET数量，
const int WAIT_TIME=10;//客户端等待事件(ms)
const int BUFFER_SIZE=1024;
const int MAX_LINK_NUM=10;
int RECV_TIMEOUT=10;
int SEND_TIMEOUT=10;

SOCKET cliSock[MAX_LINK_NUM];
WSAEVENT cliEvent[MAX_LINK_NUM];
SOCKADDR_IN cliAddr[MAX_LINK_NUM];
int link_num=0;
DWORD WINAPI servEventThread(LPVOID lpParameter);

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

    SOCKET servSock=socket(AF_INET,SOCK_STREAM,0);
    //server地址包；
    SOCKADDR_IN serAddr;
    serAddr.sin_family=AF_INET;
    serAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
    serAddr.sin_port=htons(1212);

    bind(servSock,(SOCKADDR*)&serAddr,sizeof(serAddr));
    //给服务器SOCKET绑定一个事件对象；
/*   WSAEVENT servEvent=WSACreateEvent();//创建一个人工重设的未传信事件
     WSAEventSelect(servSock,servEvent,FD_ALL_EVENTS);//绑定事件并监听所有事件

    cliSock[0]=servSock;
    cliEvent[0]=servEvent;*/
    
    listen(servSock,10);

    //创建接受连接的线程；
    /*CloseHandle(CreateThread(NULL,0,servEventThread,(LPVOID)&servSock,0,0));
    cout<<"聊天服务器已开启"<<endl;
    cout<<"限制最大连接数："<<MAX_LINK_NUM<<endl;*/

    int addrLen=sizeof(SOCKADDR);
    SOCKET cliSock=accept(servSock,(SOCKADDR*)&serAddr,&addrLen);
    if (cliSock!=INVALID_SOCKET)
    {
        cout<<"connect sucessfully"<<endl;
    }
    while (true)
    {
        char buf[100]={0};
		//接收消息
        int nrecv=recv(cliSock,buf,sizeof(buf),0);
        if(nrecv>0) cout<<buf<<endl;

    }
    WSACleanup();
    return 0;
}
DWORD WINAPI servEventThread(LPVOID lpParameter){
	return 0;
}