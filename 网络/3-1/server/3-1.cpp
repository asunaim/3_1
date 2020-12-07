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
	cout << "re" << endl;
	//初始化地址
	addrop.sin_addr.s_addr = inet_addr("192.168.89.1");
	addrop.sin_family = AF_INET;
	addrop.sin_port = htons(SPORT);

	addr.sin_addr.s_addr = inet_addr("192.168.89.1");
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
	//	
	//	//simplerecv(b);

	//	if (b.get_exist())
	//	{
	//		cout << (char*)&b << endl;
	//		if (!tackle(b))
	//			break;//挥手 连接中断
	//	}
	//	//Sleep(200);
	//}

	while (1)
	{
		message b;
		//cout << "1";
		//strcpy_s(b.msg, "xxxxxxxx");
		//b.set_ack();
		//b.set_fin();
		////b.checksum = 235;
		//u_char* a ;
		//a = (u_char*)&b;
		simplerecv(b);
		if (b.get_exist())
		{
			cout << b.msg << endl;
			if (b.checkchecksum() != 1)
			cout << "报文错误" << endl;
		}
		Sleep(2000);
	}
}