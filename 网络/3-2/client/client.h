#pragma once
#include "function.h"



HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;
DWORD WINAPI sendhandler(LPVOID lparam);
DWORD WINAPI recvhandler(LPVOID lparam);


message msgsend[SENDBUFFER];


int buildconnectionCli();//握手
int sendfile(char* name);//发送文件

void readfile(char* name, char content[50000][1024], int& length, int& index);
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


void readfile(char* name, char content[50000][1024], int& length, int& index)
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
}



DWORD WINAPI recvhandler(LPVOID lparam)
{
	int i = (int)(LPVOID)lparam;
	//cout << "接收" << i << endl;
	clock_t finalovertime;
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
				base=a.ackseq+1;
				overtime = 0;
				//cout << "现在是正常的" << endl;
				//flag = 0;
			}
			//else if (a.ackseq < base-1)//前面的ack丢包
			//{
			//	//base = a.ackseq+1;//状态退回
			//	//overtime = 1;
			//	//cout << "前面的ack丢包" << endl;
			//	overtime = 0;
			//}
			//else if(a.ackseq == base - 1)
			//{
			//	//overtime = 1;//重传所有已发送未确认的分组
			//	//cout << "需要重传" << endl;
			//	//base = a.ackseq+1;
			//	//flag++
			//}
			//else
			//{
			//	/*overtime = 1;
			//	cout << "失序" << endl;*/
			//	base = a.ackseq + 1;
			//	overtime = 0;
			//}
		}
		clockend = clock();
		//if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)
		//{
		//	//超时，加锁
		//	mutex mxt;
		//	mxt.lock();
		//	overtime = 1;
		//	mxt.unlock();
		//	cout << "超时" << endl;
		//	//flag++;
		//}
		////mxt.~mutex();
		//finalovertime = clock();
		//if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME*SENT_TIMES)
		//{
		//	mutex mxt;
		//	mxt.lock();
		//	overtime = 2;
		//	mxt.unlock();
		//	cout << "对方似乎断网了" << endl;
		//	return 0;
		//}
	}
	cout << "exitpoint2" << endl;
	return 1;
}



DWORD WINAPI sendhandler(LPVOID lparam)//发线程
{
	int flag = 0;
	clock_t s = clock();
	while (base < buffersize)
	{
		if (sendnextseq == base + N)
			Sleep(20);
		//cout << "发送base " << base << endl;
		int i = (int)(LPVOID)lparam;
		//cout << "发送" << i << endl;
		if (!overtime)
			for (; sendnextseq < base + N && sendnextseq < buffersize; sendnextseq++)
			{
				flag = 0;//正常发送
				if (!overtime)
				{
					simplesend(msgsend[sendnextseq]);
					s = clock();
					//Sleep(20);
				}
				else { 
					break; 
				}
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
		if (overtime==1)//这一部分需要加锁，运行此部分
		{
			if(flag) 
				Sleep(50);//减少重传次数
			for (int i = base; i < sendnextseq; i++)
			{
				//重新发送
					simplesend(msgsend[i]); //Sleep(10);
					s = clock();
					flag++;
					//Sleep(100);
				
				//cout << "xxxxxxxxxxx" << endl;
			}
			overtime = 0;
		}
		//if (overtime == 2)
			//return 0;//断网啦别发了
		clock_t e = clock();
		if ((e - s)/ CLOCKS_PER_SEC >= WAIT_TIME)//超时重发
			overtime = 1;
		if ((e - s) / CLOCKS_PER_SEC >= WAIT_TIME * SENT_TIMES||flag==SENT_TIMES)//超时重发
		{
			overtime = 2;
			return 0;
		}
	}
	cout << "exitpoint1" << endl;
	return 0;
}


