//server.cpp

#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <WinSock2.h>//windows socket 编程头文件
#include<string.h>
#include<cstring>
#include<string>

#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中

using namespace std;

#define BUF_SIZE 2048
#define PORT 6666

DWORD WINAPI handlerRequest1(LPVOID lparam);
DWORD WINAPI handlerRequest2(LPVOID lparam);
//创建socket
SOCKET sockSer, sockCli;//服务器和客户端的socket
SOCKADDR_IN addrSer, addrCli;//ip+端口号
int len = sizeof(SOCKADDR_IN);


HANDLE hThread1,hThread2;
DWORD dwThreadId1, dwThreadId2;

//状态码
int cond;


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
	addrSer.sin_addr.s_addr = inet_addr("192.168.89.1");
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(PORT);

	//绑定
	if (bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == -1)
		cout << "bind error" << endl;//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输

	cout << "Server" << endl;
	//监听
	cout << "listening" << endl;

	listen(sockSer, 5);
	SOCKET sockConn = accept(sockSer, (SOCKADDR*)&addrCli, &len);//失败sockConn=INVALID_SOCKET
	if (sockConn != INVALID_SOCKET) cout << "connected" << endl;
																 //while (1)
	{
		//if (sockConn != INVALID_SOCKET)
		{
			while (1)
			{
				hThread1 = ::CreateThread(NULL, NULL, handlerRequest1, LPVOID(sockConn), 0, &dwThreadId1);
				hThread2 = ::CreateThread(NULL, NULL, handlerRequest2, LPVOID(sockConn), 0, &dwThreadId2);
				WaitForSingleObject(hThread1, 20);
				WaitForSingleObject(hThread2, 20);
				CloseHandle(hThread2);
				CloseHandle(hThread1);
				if (cond)break;

			}

		}
		
	}
	closesocket(sockSer);
	WSACleanup();
	return 0;
}

DWORD WINAPI handlerRequest1(LPVOID lparam)
{
	char sendBuf[BUF_SIZE] = {};
	//cout << "send" << endl;
	SOCKET socketClient = (SOCKET)(LPVOID)lparam;
	cin.getline(sendBuf,2048,'\n');
	send(socketClient, sendBuf, 2048, 0);
	if (!strcmp(sendBuf, "quit"))
		//recv(socketClient, recvBuf, 50, 0);
		cond = 1;
	return 0;
}

DWORD WINAPI handlerRequest2(LPVOID lparam)
{
	char recvBuf[BUF_SIZE] = {};
	//cout << "recv" << endl;
	SOCKET socketClient = (SOCKET)(LPVOID)lparam;
	//send(socketClient, sendBuf, 2048, 0);
	recv(socketClient, recvBuf, 50, 0);
	if (recvBuf[0])
	{
		if (!strcmp(recvBuf, "quit"))
		{
			cond = 1;
		}
		cout << endl << "收到信息: " << recvBuf << endl;
	}
	//if (!strcmp(recvBuf, "我要下线了"))
	//{
	//	send(socketClient, "88", 2048, 0);
	//	return 0;
	//}
	//if (!strcmp(recvBuf, "88"))
	//	return 0;
		//closesocket(socketClient);
	return 0;
}



//cout << "connected" << endl;
//recv(sockConn, recvBuf, 2048, 0);
//cout << "收到消息：" << recvBuf << endl;
//cin >> sendBuf;
//cout << sendBuf << endl;
//send(sockConn, sendBuf, strlen(sendBuf), 0);
//cout << "connected" << endl;
