//client
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

char sendBuf[BUF_SIZE] = {};
char recvBuf[BUF_SIZE] = {};

//线程
HANDLE hThread1, hThread2;
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
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);//地址类型（ipv4），服务类型（流式套接字）

	//client
	addrCli.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrCli.sin_family = AF_INET;
	addrCli.sin_port = htons(PORT);

	//初始化server地址
	addrSer.sin_addr.s_addr = inet_addr("192.168.89.1");
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(PORT);

	//绑定
	//bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR));//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输

	////监听
	//cout << "listening" << endl;
	//listen(sockSer, 5);

	cout << "Client" << endl;


	cout << "connecting" << endl;
	sockCli = connect(sockClient, (SOCKADDR*)&addrSer, sizeof(SOCKADDR));
	if (sockCli != SOCKET_ERROR) cout << "connected" << endl;
	//while (1)
	{
		
		//if (sockCli != INVALID_SOCKET)
		{
			while (1)
			{
				hThread1 = ::CreateThread(NULL, NULL, handlerRequest1, LPVOID(sockClient), 0, &dwThreadId1);

				hThread2 = ::CreateThread(NULL, NULL, handlerRequest2, LPVOID(sockClient), 0, &dwThreadId2);
				WaitForSingleObject(hThread1, 20);
				
				WaitForSingleObject(hThread2, 20);
				CloseHandle(hThread2);
				CloseHandle(hThread1);
				if (cond) break;
				
			}


		}
	}
	closesocket(sockClient);
	WSACleanup();
	return 0;
}
DWORD WINAPI handlerRequest1(LPVOID lparam)
{
	char sendBuf[BUF_SIZE] = {};
	SOCKET socketClient = (SOCKET)(LPVOID)lparam;
	cin.getline(sendBuf, 2048, '\n');
	send(socketClient, sendBuf, 2048, 0);
	if (!strcmp(sendBuf, "quit"))
		//recv(socketClient, recvBuf, 50, 0);
		cond = 1;
	return 0;
}

DWORD WINAPI handlerRequest2(LPVOID lparam)
{
	char recvBuf[BUF_SIZE] = {};
	SOCKET socketClient = (SOCKET)(LPVOID)lparam;
	//send(socketClient, sendBuf, 2048, 0);
	recv(socketClient, recvBuf, 2048, 0);
	if (recvBuf[0])
	{
		if (!strcmp(recvBuf, "quit"))
		{
			cond = 1;
		}
		cout << endl << "收到信息: " << recvBuf << endl;
		
	}

	//closesocket(socketClient);
	return 0;
}
