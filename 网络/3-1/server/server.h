#pragma once
#include "function.h"



int tackle(message b);//处理收到的报文

int buildconnectionSer();//服务端建立连接

//////////////////////////////////////////////////










int tackle(message b)//处理收到的报文
{
	if (b.get_syn())
	{
		if (buildconnectionSer())
			cout << "建立连接" << endl;
		else cout << "连接建立失败" << endl;
	}
	return 1;
}





int buildconnectionSer()//服务端建立连接
{
	message a,b;
	a.set_ack(); a.set_syn();
	simplesend(a);
	clockstart = clock();
	int flag = 0;//重发超过10次退出
	while (1)
	{
		simplerecv(b);
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
			//cout << "连接失败" << endl;
			simplesend(a);//重传
		}
	}
	cout << "连接成功";
	return 1;
}