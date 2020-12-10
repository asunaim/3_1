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


	//while (1)
	//{
	//	message b;
	//	simplerecv(b);
	//	if (b.get_exist())
	//	{
	//		//cout << b.msg << endl;
	//		tackle(b);
	//	}
	//	Sleep(2000);
	//}
	//message a;
	//strcpy(a.msg, "hihihihi");
	//while (1)
	//{
	//	simplesend(a);
	//	Sleep(200);
	//}
	//message a;
	//a.set_ack();
	//for (int i = 0; i < 1817; i++)
	//{
	//	cout << "发送" << endl;
	//	a.ackseq = i;
	//	simplesend(a);
	//}
	int filestatus = 0;//当前不在传输文件的状态
	int fileseq = 0;
	int index = 0; int length = 0;
	char name[30] = {};
	clockstart = clock();
	int status = 0;//标识server状态
	while (1)
	{
		message a, b;
		simplerecv(a);
		if (a.get_exist())//收到消息且顺序正确
		{
			//cout << "status " << status << endl;
			if (a.msgseq == recvnextseq)
			{
				//cout << "recvnextseq" << recvnextseq << endl;
				clockstart = clock();
				if (a.get_syn())
				{
					if (!buildconnectionSer()) 
						continue;
					else status = 1;
				}
				if (status && a.get_startfile())
				{
					//cout << "接收文件" << endl;
					memset(name, 0, sizeof(name));
					filestatus = 1;
					index = a.index;
					length = a.filelength;
					strcpy(name, a.msg);
					message f;
					f.set_ack();
					f.ackseq = recvnextseq;
				
					simplesend(f);
				}
				else if (status && filestatus && a.get_endfile())//文件发送结束
				{
					//cout << "接收文件" << endl;
					filestatus =0;
					
					if (fileseq != index)
					{
						cout << "出错" << endl;
						continue;
					}
					filepacket* packet = new filepacket;
					packet->a = a;
					packet->index = index;
					packet->length = length;
					hThread1 = ::CreateThread(NULL, NULL, filehandler, LPVOID(packet), 0, &dwThreadId1);
					WaitForSingleObject(hThread1, 1000);
					fileseq++;
					fileseq = 0;
					message b;
					b.set_ack();
					b.ackseq = recvnextseq;
					simplesend(b);
					outfile(name, content, length, index);
					cout << "文件接收结束" << endl;
					
				}
				else if (status && filestatus)
				{
					fileseq++;
					
					filepacket* packet = new filepacket;
					packet->a = a;
					packet->index = fileseq-1;
					packet->length = 1024;
					hThread2 = ::CreateThread(NULL, NULL, filehandler, LPVOID(packet), 0, &dwThreadId2);
					WaitForSingleObject(hThread2, 1000);
					message a;
					a.set_ack();
					a.ackseq = recvnextseq;
					simplesend(a);
				}
				else if (status && a.get_fin())
				{
					status = 0;
					message a;
					a.set_ack();
					a.ackseq = recvnextseq;
					simplesend(a);
					cout << "断开连接" << endl;
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
		Sleep(20);
	}





	closesocket(sock);
	WSACleanup();
	return 0;
}