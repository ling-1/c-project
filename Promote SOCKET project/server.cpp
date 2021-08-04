//#include<pch.h>
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
    WSAEVENT servEvent=WSACreateEvent();//创建一个人工重设的未传信事件
    WSAEventSelect(servSock,servEvent,FD_ALL_EVENTS);//绑定事件并监听所有事件

    cliSock[0]=servSock;
    cliEvent[0]=servEvent;
    
    listen(servSock,10);

    //创建接受连接的线程；
    CloseHandle(CreateThread(NULL,0,servEventThread,(LPVOID)&servSock,0,0));
    cout<<"聊天服务器已开启"<<endl;
    cout<<"限制最大连接数："<<MAX_LINK_NUM<<endl;

    /*int addrLen=sizeof(SOCKADDR);
    SOCKET cliSock=accept(servSock,(SOCKADDR*)&serAddr,&addrLen);
    if (cliSock!=INVALID_SOCKET)
    {
        cout<<"connect sucessfully"<<endl;
    }
    while (true)
    {
        char buf[100]={0};
        int nrecv=recv(cliSock,buf,sizeof(buf),0);
        if(nrecv>0) cout<<buf<<endl;
    }*/
    while (true){
        char contentbuf[BUFFER_SIZE]={0};
        char sendBuf[BUFFER_SIZE]={0};
        sprintf(sendBuf,"[server]%s",contentbuf);
        cin.getline(sendBuf,sizeof(sendBuf));
        for (int j=1;j<=link_num;j++){
            send(cliSock[j],sendBuf,sizeof(sendBuf),0);
        }

    }

    WSACleanup();
    return 0;
}
//server事件处理线程；
DWORD WINAPI servEventThread(LPVOID lpParameter)
{
    SOCKET servSock=*(SOCKET*)lpParameter;
    
    while (true)
    {
        //遍历所有
        for(int i=0;i<link_num+1;i++)
        {
            int index=WSAWaitForMultipleEvents(1,&cliEvent[i],false,WAIT_TIME,0);
            index-=WSA_WAIT_EVENT_0;//此时index为第n个事件；
            if(index==WSA_WAIT_TIMEOUT||index==WSA_WAIT_FAILED)
            {
                continue;//超时或则出错直接跳过；
            }
            else if(index==0)
            {

                WSANETWORKEVENTS networkEvent;//获取返回的结构体；
                WSAEnumNetworkEvents(cliSock[i],cliEvent[i],&networkEvent);//查看事件性质；
                
                if (networkEvent.lNetworkEvents & FD_ACCEPT)//若产生accept事件，与位掩码相与；
                {
                    if(networkEvent.iErrorCode[FD_ACCEPT_BIT]!=0)
                    {
                        cout<<"号客户端连接时出现错误，错误代码："<<networkEvent.iErrorCode[FD_ACCEPT_BIT]<<endl;
                        continue;
                    }
                    //接受连接
                    if (link_num+1<MAX_LINK_NUM)
                    {   int newIndex=link_num+1;
                        int addrLen=sizeof(SOCKADDR);
                        SOCKET newSock=accept(servSock,(SOCKADDR*)&cliAddr[newIndex],&addrLen);
                        if (newSock!=INVALID_SOCKET)
                        {   //给新客户端分配SOCKET;
                            setsockopt(newSock,SOL_SOCKET,SO_SNDTIMEO,(const char*)&SEND_TIMEOUT,sizeof(SEND_TIMEOUT));
                            setsockopt(newSock,SOL_SOCKET,SO_SNDTIMEO,(const char*)&RECV_TIMEOUT,sizeof(SEND_TIMEOUT));//设置发送与接受时限；
                            cliSock[newIndex]=newSock;
                            WSAEVENT newEvent =WSACreateEvent();//为客户端绑定事件对象；
                            WSAEventSelect(cliSock[newIndex],newEvent,FD_CLOSE|FD_READ|FD_WRITE);//设置监听，close,read,write事件；
                            cliEvent[newIndex]=newEvent;
                            link_num++;
                            cout<<"#"<<newIndex<<"游客（IP)"<<inet_ntoa(cliAddr[i].sin_addr)<<")进入了聊天室,当前连接数："<<link_num<<endl;
                            
                            //给所有游客发送欢迎消息；
                            char buf[BUFFER_SIZE]={0};
                            sprintf(buf,"欢迎游客（IP:%s",inet_ntoa(cliAddr[i].sin_addr),")进入了聊天室)");
                            for (int j=1;j<=link_num;j++)
                            {
                                send(cliSock[j],buf,sizeof(buf),0);
                            }


                        }
                        
                    }
                    
                }
                else if (networkEvent.lNetworkEvents&FD_CLOSE)//客户端关闭即断开连接
                {   link_num--;
                    for (int j=1;j<=link_num;j++)
                        {
                            char buf[BUFFER_SIZE]="[server]（IP:";
                            strcat(buf,inet_ntoa(cliAddr[i].sin_addr));
                            strcat(buf,")退出了聊天室");
                            send(cliSock[j],buf,sizeof(buf),0);
                        }

                    if(networkEvent.iErrorCode[FD_CLOSE_BIT]!=0) {
                        cout<<"号客户端连接时出现错误，错误代码："<<networkEvent.iErrorCode[FD_ACCEPT_BIT]<<endl;
                        continue;
                    }
                    cout<<"游客（IP)"<<inet_ntoa(cliAddr[i].sin_addr)<<")退出了了聊天室，当前连接数："<<link_num<<endl;
                    //i表示已经关闭的客户端下标，释放这个资源
                    closesocket(cliSock[i]);
                    WSACloseEvent(cliEvent[i]);
                    //数组调整，所有元素向前移动一位；
                    for(int j=i;i<link_num;j++){
                        cliSock[j]=cliSock[j+1];
                        cliEvent[j]=cliEvent[j+1];
                        cliAddr[j]=cliAddr[j+1];
                    }
                    char buf[BUFFER_SIZE]={0};
                    sprintf(buf,"游客（IP:%s",inet_ntoa(cliAddr[i].sin_addr),")退出了聊天室)");
                    for(int j=1;j<=link_num;j++){
                        send(cliSock[j],buf,sizeof(buf),0);
                    }

                }
                else if(networkEvent.lNetworkEvents&FD_READ)//接受消息
                {
                    char buf[BUFFER_SIZE]={0};//字符缓冲区
                    char buf2[BUFFER_SIZE]={0};//字符缓冲区
                    for(int j=1;j<=link_num;j++){
                        int nrecv=recv(cliSock[j],buf,sizeof(buf),0);
                        if(nrecv>0)
                        {   sprintf(buf2,"[#%s]%s",j,buf);//在服务端显示
                            cout<<"[server]"<<buf2<<endl;
                            for (int k=1;k<=link_num;k++){
                                send(cliSock[k],buf2,sizeof(buf),0);
                            }
                        }
                    }
                    
                }
                
            }

        }
    }
    
    return 0;

}