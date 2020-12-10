#include "server.h"
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中
using namespace std;




//SOCKET sock, clientsocket;
//SOCKADDR_IN addrSer, addrCli;//ip+端口号
//
//int addr_len = sizeof(struct sockaddr_in);


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
	sock = socket(AF_INET, SOCK_DGRAM, 0);//地址类型（ipv4）,数据报套接字
	if (sock == INVALID_SOCKET)
	{
		cout << "socket创建失败";
		return -1;
	}

	// 设置超时
	//struct timeval timeout;
	//timeout.tv_sec = 1;//秒
	//timeout.tv_usec = 0;//微秒
	//if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == -1) {
	//	cout << "setsockopt failed:";
	//}
	cout << "server" << endl;
	//初始化地址
	addrop.sin_addr.s_addr = inet_addr("192.168.89.1");
	addrop.sin_family = AF_INET;
	addrop.sin_port = htons(SPORT);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(CPORT);




	int len = sizeof(SOCKADDR_IN);
	//绑定
	//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输
	if (bind(sock, (SOCKADDR*)&addrop, sizeof(SOCKADDR)) == -1)
	{
		cout << "bind error" << endl; return -1;
	}


	//message b;
	//simplerecv(b);
	//message a;
	//a.set_ack();
	//for (int i = 0; i < 1817; i++)
	//{
	//	cout << "发送" << endl;
	//	a.ackseq = i;
	//	simplesend(a);
	//}


	clockstart = clock();

	while (1)
	{
		//message a, b;
		//msgrecv[recvnextseq]
		simplerecv(msgrecv[recvnextseq]);
		if (msgrecv[recvnextseq].get_exist())//收到消息且顺序正确
		{
			//cout << "status " << status << endl;
			if (msgrecv[recvnextseq].msgseq == recvnextseq)
			{
				message b;
				b.set_ack();
				b.ackseq = recvnextseq;
				simplesend(b);
				
				//cout << "recvnextseq" << recvnextseq << endl;
				clockstart = clock();

				if (status && msgrecv[recvnextseq].get_fin())
				{
					status = 0;
					cout << "断开连接" << endl;
					break;//可以断开连接
				}
				else
				{
					hThread1 = ::CreateThread(NULL, NULL, filehandler, LPVOID((char*)&msgrecv[recvnextseq]), 0, &dwThreadId1);
				}	
				recvnextseq++;
			}
			else//消息乱序
			{
				message a;
				a.set_ack();
				a.ackseq = recvnextseq-1;
				simplesend(a);
			}
		}
		else
		{
			clockend = clock();
			if (status&&(clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME && recvnextseq)//需要接收到消息且超时
			{
				message a;
				a.ackseq = recvnextseq - 1;
				a.set_ack();
			}
		}
		//Sleep(20);
	}

	WaitForSingleObject(hThread1, INFINITE);
	//WaitForSingleObject(hThread2, INFINITE);
	outfile(name, content, length, index);


	closesocket(sock);
	WSACleanup();
	return 0;
}