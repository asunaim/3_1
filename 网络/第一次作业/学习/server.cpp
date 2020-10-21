#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <WinSock2.h>//windows socket 编程头文件
#include <cstring>

#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中

using namespace std;

#define BUF_SIZE 2048
#define PORT 6666


//创建socket
SOCKET sockSer, sockCli;//服务器和客户端的socket
SOCKADDR_IN addrSer, addrCli;//ip+端口号
int len = sizeof(SOCKADDR_IN);


//缓冲区
char sendBuf[BUF_SIZE];
char recvBuf[BUF_SIZE];


int main()
{
	//加载socket库
	WSADATA wsaData;
	//MAKEWORD(2.2),成功返回0
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "socket初始化失败" << endl;
		return 0;
	}

	//创建Socket
	//创建一个socket，并将该socket绑定到一个特定的传输层
	sockSer = socket(AF_INET, SOCK_STREAM, 0);//地址类型（ipv4），服务类型（流式套接字）

	//初始化地址
	addrSer.sin_addr.s_addr = inet_addr("10.130.124.12");
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(PORT);

	//绑定
	if(bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR))==-1) 
		cout<<"bind error"<<endl;//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输

	cout << "Server" << endl;
	//监听
	cout << "listening" << endl;

	listen(sockSer, 5);
	while (1)
	{
		SOCKET sockConn = accept(sockSer, (SOCKADDR*)&addrCli, &len);//失败sockConn=INVALID_SOCKET
	if (sockConn != INVALID_SOCKET)
	{
		cout << "connected" << endl;
		recv(sockConn, recvBuf, 2048, 0);
		cout << "收到消息：" << recvBuf << endl;
		/*cin >> sendBuf;
		send(sockConn, sendBuf, strlen(sendBuf), 0);
		cout << "connected" << endl;*/

		}
	}
		closesocket(sockSer);
		WSACleanup();
	return 0;
}
