#pragma once
#include "function.h"


int buildconnectionCli();//三次握手



int buildconnectionCli()//客户端，连接发起方
{
	message a, b;
	a.set_syn();
	simplesend(a);
	clockstart = clock();
	int flag = 0;//重发超过10次退出
	while (1)
	{
		simplerecv(b);
		Sleep(2000);

		if (b.get_ack())//b包含对消息a的ack和syn
			break;
		clockend = clock();
		if (flag == SENT_TIMES)
			return 0;
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)
		{
			flag++;
			clockstart = clock();
			cout << "重传" << flag << endl;
			cout << "连接失败" << endl;
			simplesend(a);//重传
		}
	}
	message c;
	c.set_ack();
	simplesend(c);
	cout << "连接成功";
}