#include <stdio.h>
#include <thread>
#include <WinSock2.h>
#include <iostream>
#include <cstring>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

const int BUFFER_SIZE=1024;
DWORD WINAPI recvMsgThread(LPVOID lpParameter);
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
    SOCKET cliSock=socket(AF_INET,SOCK_STREAM,0);
    SOCKADDR_IN cliAddr={0};
    cliAddr.sin_family=AF_INET;
    cliAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
    cliAddr.sin_port=htons(1212);
    SOCKADDR_IN serAddr={0};
    serAddr.sin_family=AF_INET;
    serAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
    serAddr.sin_port=htons(1212);
    if (connect(cliSock,(SOCKADDR*)&serAddr,sizeof(SOCKADDR))==SOCKET_ERROR){
        cout<<"连接出现错误，错误代码"<<WSAGetLastError()<<endl;
    }
        //创建接受连接的线程；
    //CloseHandle(CreateThread(NULL,0,recvMsgThread,(LPVOID)&cliSock,0,0));
    while (true)
    {
		char buf2[100]={0};
		//发送消息；
        cin>>buf2;
        if (strcmp(buf2,"quit")==0) break;//若输入quit则退出；
        send(cliSock,buf2,sizeof(buf2),0);

		
    }
    closesocket(cliSock);
    WSACleanup();
    return 0;
}
/*
DWORD WINAPI recvMsgThread(LPVOID lpParameter){
    SOCKET cliSock=*(SOCKET*)lpParameter;
    while (true)
    {
        char buffer[BUFFER_SIZE]={0};//字符缓冲区
        int nrecv=recv(cliSock,buffer,sizeof(buffer),0);
        if(nrecv>0)
        {
            cout<<"[server]"<<buffer<<endl;

        }
        else if(nrecv<0){
            cout<<"与服务端断开连接"<<endl;
            break;
        }
    }
    
    return 0;
}*/