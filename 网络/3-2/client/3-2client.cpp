#include "client.h"




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
	sock = socket(AF_INET, SOCK_DGRAM, 0);//地址类型（ipv4）
	if (sock == INVALID_SOCKET)
	{
		cout << "socket创建失败";
		return -1;
	}

	// 设置超时
	//struct timeval timeout;
	//timeout.tv_sec = 0;//秒
	//timeout.tv_usec = 20;//微秒
	//if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == -1) {
	//	cout << "setsockopt failed:";
	//}

	//初始化地址
	addrop.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrop.sin_family = AF_INET;
	addrop.sin_port = htons(CPORT);


	addr.sin_addr.s_addr = inet_addr("192.168.89.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SPORT);

	int len = sizeof(SOCKADDR_IN);
	//绑定
	//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输
	/*if (bind(sock, (SOCKADDR*)&addrop, sizeof(SOCKADDR)) == -1)
	{
		cout << "bind error" << endl; return -1;
	}*/


	//message a;
	//a.msgseq = 1;
	//simplesend(a);

	cout << "client" << endl;
	int i = 0;
	while (1)
	{
		char name[30] = { "1.jpg" };
		/*cout << "请输入文件名 ";
		cin >> name;
		if (name == "quit")
			break;*/
		buildconnectionCli();
		sendfile(name);
		byecli();

		clock_t timestart = clock();
			if (base < buffersize);
			{
				hThread2 = ::CreateThread(NULL, NULL, sendhandler, LPVOID(i), 0, &dwThreadId2);
				hThread1 = ::CreateThread(NULL, NULL, recvhandler, LPVOID(i), 0, &dwThreadId1);
				WaitForSingleObject(hThread1,INFINITE);
				WaitForSingleObject(hThread2, INFINITE);
				//i++;
				if (base == buffersize)
				{
					cout << "文件发送结束" << endl;
					//break;
				}
			}
		clock_t timeend = clock();
		double endtime = (double)(timeend - timestart) / CLOCKS_PER_SEC;
		cout << "Total time:" << endtime << endl;		//s为单位
		cout << "吞吐率：" << (double)(buffersize) * sizeof(message)/endtime * 8 / 1024 / 1024 << "Mbps" << endl;

		//Sleep(2000);
		//cout << "exitpoint3" << endl;
		break;
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}