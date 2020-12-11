#pragma once
#include "class.h"



void simplesend(message& a);//约定好各项信息之后的简易发送函数
void simplerecv(message& a);

bool stopwaitsend(message& a, message b);//停等机制
bool stopwaitrecv(message& a, message b);


void simplesend(message& a)
{
	/*clock_t start = clock();*/
	a.set_exist();
	a.setchecksum();
	//a.checkchecksum();
	if (sendto(sock, (char*)&a, sizeof(message), 0, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR);
	{
		/*cout << "发送失败" << endl;
		DWORD dwError = WSAGetLastError();
		cout << dwError << endl;*/
	}
	//if (a.flag) { cout << "发送 "; a.print(); }
	//clock_t end = clock();
	//double endtime = (double)(end - start) / CLOCKS_PER_SEC;
	//cout << "time:" << endtime << endl;		//s为单位
}

void simplerecv(message& a)
{
	memset(a.msg, 0, sizeof(a.msg));
	recvfrom(sock, (char*)&a, sizeof(message), 0, (struct sockaddr*)&addr, &addr_len);
	//if (a.flag) { cout << "接收 "; a.print(); }
}


bool check(message a, message b)
{
	/*if (b.get_ack() == 1 && b.ackseq == a.msgseq)
		return 1;
	else return 0;*/
	return 1;
}


bool stopwaitsend(message& a, message b)//a写入待发送消息，如果收到对方返回的ack则成功
{
	simplesend(a);
	clockstart = clock();
	int flag = 0;//重发超过10次退出
	while (1)
	{
		simplerecv(b);
		if (b.get_ack() && b.checkchecksum())//b包含对消息a的ack
		{
			cout << "发送&ack" << endl;
			return 1;
		}
		clockend = clock();
		if (flag == SENT_TIMES)
			return 0;
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)//超时
		{
			flag++;
			clockstart = clock();
			cout << "重传" << flag << endl;
			simplesend(a);//重传
		}
	}
	return 0;//0代表发送失败
}

bool stopwaitrecv(message& a, message b)//收到的消息写入a中
{
	int flag = 0;
	while (1)
	{
		simplerecv(a);
		if (a.get_exist())
		{
			int check = a.checkchecksum();
			if (check)
			{
				b.set_ack();
				simplesend(b);
				memset((char*)&b, 0, sizeof(message));
				cout << "接收&ack" << endl;
				return 1;
			}
		}
	}
	cout << "接收失败" << endl;
	return 0;
}

void time()
{
	clock_t start, end;
	start = clock();		//程序开始计时
	int ans = 0;
	for (int i = 1; i <= 1e8; i++)
		ans++;
	end = clock();		//程序结束用时
	double endtime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime << endl;		//s为单位
	cout << "Total time:" << endtime * 1000 << "ms" << endl;	//ms为单位
}
