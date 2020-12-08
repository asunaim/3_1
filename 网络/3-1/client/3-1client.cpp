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
	struct timeval timeout;
	timeout.tv_sec = 1;//秒
	timeout.tv_usec = 0;//微秒
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == -1) {
		cout<<"setsockopt failed:";
	}

	//初始化地址
	addrop.sin_addr.s_addr = inet_addr("192.168.89.1");
	addrop.sin_family = AF_INET;
	addrop.sin_port = htons(CPORT);


	addr.sin_addr.s_addr = inet_addr("192.168.89.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SPORT);

	int len = sizeof(SOCKADDR_IN);
	//绑定
	//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输
	if (bind(sock, (SOCKADDR*)&addrop, sizeof(SOCKADDR)) == -1)
	{
		cout << "bind error" << endl; return -1;
	}

	////绑定
	////强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输
	//if (bind(serversocket, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == -1)
	//{
	//	cout << "bind error" << endl; return -1;
	//}

	//while (1)
	//{
	//	/*cout << "sending";
	//	char sendBuf[BUF_SIZE] = {};
	//	if(sendto(sock, "hi", 1024, 0, (struct sockaddr*)&addrSer,sizeof(sockaddr))==SOCKET_ERROR);
	//	{
	//		cout << "发送失败" << endl;
	//		DWORD dwError = WSAGetLastError();
	//		cout << dwError<<endl;
	//	}*/
	//	message a;
	//	strcpy_s(a.msg, "hihihi");
	//	simplesend(a);
	//	Sleep(2000);
	//}
	
	//while (1)
	//{
	//	message b;
	//	strcpy_s(b.msg, "xxxxxxxx");
	//	simplesend(b);
	//	/*if (b.exist)
	//	{
	//		cout << b.msg << endl;
	//	}*/
	//}

	//while (1)
	//{
	//	message b;
	//	strcpy_s(b.msg, "xxxxxxxx");
	//	b.exist = 1;
	//	simplesend(b);
	//	
	//	//if (b.exist)
	//	//{
	//	//	cout << b.msg << endl;
	//	//		//break;//挥手 连接中断
	//	//}
	//	Sleep(2000);
	//}




	//buildconnectionCli();
	char* name=new char[10];
	strcpy(name, "helloworld.txt");
	sendfile(name);
}