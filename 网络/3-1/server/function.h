#pragma once
#include "class.h"


void simplesend(message& a);//约定好各项信息之后的简易发送函数
bool simplerecv(message& a);

bool stopwaitsend(message a, message b);//停等机制

void simplesend(message& a)
{
	a.set_exist();
	a.setchecksum();
	if (sendto(sock, (char*)&a, 1024, 0, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR);
	{
		/*cout << "发送失败" << endl;
		DWORD dwError = WSAGetLastError();
		cout << dwError << endl;*/
	}
}

bool simplerecv(message& a)
{
	memset(a.msg, 0, sizeof(a.msg));
	recvfrom(sock, (char*)&a, 1024, 0, (struct sockaddr*)&addr, &addr_len);
	
	return 0;
}

bool check(message a, message b)
{
	/*if (b.get_ack() == 1 && b.ackseq == a.msgseq)
		return 1;
	else return 0;*/
	return 1;
}


bool stopwaitsend(message& a, message& b)//a写入待发送消息
{
	//simplesend(a);
	clockstart = clock();
	int flag = 0;//重发超过10次退出
	while (1)
	{
		simplerecv(b);
		if (b.msg[0] != 0 && check(a, b))//b包含对消息a的ack
			return 1;
		clockend = clock();
		if (flag == SENT_TIMES)
			return 0;
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)
		{
			flag++;
			clockstart = clock();
			cout << "重传" << flag << endl;
			simplesend(a);//重传
		}
	}
	return 0;//0代表发送时报
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
