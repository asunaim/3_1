#pragma once
#include "function.h"



HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;
DWORD WINAPI sendhandler(LPVOID lparam);
DWORD WINAPI recvhandler(LPVOID lparam);


message msgsend[SENDBUFFER];


int buildconnectionCli();//握手
int sendfile(char* name);//发送文件

void readfile(char* name, char content[10000][1024], int& length, int& index);
int byecli();//因为只是单向传输，写三次挥手即可


int buildconnectionCli()//客户端，连接发起方
{
	message a;
	a.set_syn();
	a.msgseq = buffersize;
	message::copy(msgsend[buffersize++], a);
	return 1;
}

int sendfile(char* name)//发送文件
{
	//开始发送文件消息
	message a;
	a.set_startfile();
	int index = 0; int length = 0;
	readfile(name, content, length, index);
	a.index = index;
	a.filelength = length;
	strcpy(a.msg, name);
	a.msgseq = buffersize;
	message::copy(msgsend[buffersize++], a);
	if (index==0 && length == 0)
	{

		return 0;//文件读入失败
	}
	//文件具体内容
	for (int i = 0; i < index; i++)
	{
		//message b;
		for (int j = 0; j < 1024; j++)
		{
			msgsend[buffersize].msg[j] = content[i][j];
		}
		msgsend[buffersize].msgseq = buffersize;
		buffersize++;
		//message::copy(msgsend[buffersize++], b,i);
	}
	for (int i = index; i <= index; i++)
	{
		message b;
		for (int j = 0; j < length; j++)
		{
			b.msg[j] = content[i][j];
		}
		b.msgseq = buffersize;
		b.set_endfile();
		message::copy(msgsend[buffersize++], b);
	}
	return 1;
}


void readfile(char* name, char content[10000][1024], int& length, int& index)
{
	index = 0;
	length = 0;

	ifstream in(name, ifstream::binary);
	if (!in)
	{
		cout << "文件无效" << endl;
		return;
	}
	char t = in.get();
	while (in)
	{

		content[index][length % 1024] = t;
		length++;
		if (length % 1024 == 0)//上一条缓冲区
		{
			index++; length = 0;
		}
		t = in.get();
	}
	in.close();
}

int byecli()//四次挥手
{
	message a;
	a.set_fin();
	a.msgseq = buffersize;
	message::copy(msgsend[buffersize++], a);
	return 1;
	////第一次挥手，发送FIN
	//message a, b, c;
	//a.set_fin();//第一次
	//simplesend(a);
}



DWORD WINAPI recvhandler(LPVOID lparam)
{
	int i = (int)(LPVOID)lparam;
	//cout << "接收" << i << endl;


	while (base < buffersize)
	{
		if (sendnextseq > base + N||sendnextseq==base)//窗口已满（对方还没有发送ack）或者窗口为空
			Sleep(2);
		//cout << "接" << base << endl;
		message a;
		simplerecv(a);
		if (a.get_ack())//收到确认消息且序号正确
		{
			//cout << "receive" << endl;
			clockstart = clock();//重置计时器
			if (a.ackseq >= base)//收到正确的消息序号
			{
				base++;
				overtime = 0;
			}
			else 
			{
				overtime = 1;//重传所有已发送未确认的分组
			}
		}
		clockend = clock();
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)
		{
			//超时，加锁
			overtime = 1;
		}
		//mxt.~mutex();
	}
	cout << "exitpoint2" << endl;
	return 0;
}



DWORD WINAPI sendhandler(LPVOID lparam)//发线程
{
	while (base < buffersize)
	{
		if (sendnextseq == base + N)
			Sleep(2);
		//cout << "发送base " << base << endl;
		int i = (int)(LPVOID)lparam;
		//cout << "发送" << i << endl;
		if (!overtime)
			for (; sendnextseq < base + N && sendnextseq < buffersize; sendnextseq++)
			{
				simplesend(msgsend[sendnextseq]);
				//clock_t start = clock();
				//if (!overtime)//当前不用重传，可以正常发送
				//{
				//	//发送消息
				//	simplesend(msgsend[sendnextseq]);
				//}
				//else {//前面的出问题了，跳转到重传部分
				//	//sendnextseq = base;
				//	break;
				//}
				//clock_t end = clock();
				//double endtime = (double)(end - start) / CLOCKS_PER_SEC;
				//cout << "time:" << endtime << endl;		//s为单位
			}
		if (overtime)//这一部分需要加锁，运行此部分
		{
			for (int i = base; i < sendnextseq; i++)
			{
				//重新发送
				simplesend(msgsend[i]);
				//cout << "xxxxxxxxxxx" << endl;
			}
		}
	}
	cout << "exitpoint1" << endl;
	return 0;
}


