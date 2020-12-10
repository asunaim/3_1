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
	struct timeval timeout;
	timeout.tv_sec = 1;//秒
	timeout.tv_usec = 0;//微秒
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == -1) {
		cout << "setsockopt failed:";
	}
	cout << "server3-1" << endl;
	//初始化地址
	addrop.sin_addr.s_addr = inet_addr("192.168.89.1");
	addrop.sin_family = AF_INET;
	addrop.sin_port = htons(SPORT);

	addr.sin_addr.s_addr = inet_addr("192.168.89.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(CPORT);

	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addr.sin_family = AF_INET;
	//addr.sin_port = htons(7777);

	cout << "是否使用默认IP？是1，否2";
	int i;
	cin >> i;
	if (i == 2)
	{
		char s[20] = {};
		char c[20] = {};
		cout << "请输入服务器IP: " ;
		cin >> s;
		cout << "请输入客户端IP: ";
		cin >> c;
		addrop.sin_addr.s_addr = inet_addr(s);
		addr.sin_addr.s_addr = inet_addr(c);
	}



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
	//strcpy(a.msg, "ihihihih");
	//for (int i = 0; i < 1817; i++)
	//{
	//	cout << "发送" << endl;
	//	simplesend(a);
	//}

	//while (1)
	//{
	//	message a;
	//	simplerecv(a);
	//	cout << a.msg;
	//}


	while (1)
	{
		message b;
		simplerecv(b);
		if (b.get_exist())
		{
			//cout << b.msg << endl;
			if(!tackle(b))break;
		}
		Sleep(20);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}