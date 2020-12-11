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

	// recv非阻塞
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

	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addr.sin_family = AF_INET;

	//addr.sin_port = htons(7777);

	int len = sizeof(SOCKADDR_IN);
	//绑定
	//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输
	/*if (bind(sock, (SOCKADDR*)&addrop, sizeof(SOCKADDR)) == -1)
	{
		cout << "bind error" << endl; return -1;
	}*/


	cout << "是否使用默认IP？是1，否2";
	int i;
	cin >> i;
	if (i == 2)
	{
		char s[20] = {};
		char c[20] = {};
		cout << "请输入服务器IP: ";
		cin >> s;
		cout << "请输入客户端IP: ";
		cin >> c;
		addrop.sin_addr.s_addr = inet_addr(c);
		addr.sin_addr.s_addr = inet_addr(s);
	}



	//while (1)
	//{
	//	message a;
	//	simplerecv(a);
	//	cout << a.msg;
	//}

	//message b;
	//simplerecv(b);
	//message a;
	//strcpy(a.msg, "ihihihih");
	//for (int i = 0; i < 1817; i++)
	//{
	//	cout << "发送" << endl;
	//	simplesend(a);
	//}
	cout << "client3-1" << endl;

	while (1)
	{

		int op;
		cout << "请选择操作：传输文件1，退出0 " << endl;
		cin >> op;
		if (op == 0)
			break;
		else if (op != 1)
			cout << "无效操作" << endl;
		else if (op == 1)
		{
			cout << "请输入文件名 " << endl;
			char name[30] = {};
			cin >> name;
			if (buildconnectionCli())
			{
				if (sendfile(name))
				{
					cout << "文件发送成功" << endl;
				}
				else cout << "文件发送失败" << endl;
			}
			else cout << "连接建立失败" << endl;
			if (byecli())
			{
				cout << "连接已断开" << endl;
				sendseq = 0;
			}
			else cout << "连接断开失败" << endl;
		}
		sendseq = 0;
	}
	
	
a:	closesocket(sock);
	WSACleanup();
	return 0;
}