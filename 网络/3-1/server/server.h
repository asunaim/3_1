#pragma once
#include "function.h"


int tackle(message b);//处理收到的报文

int buildconnectionSer(message t);//服务端建立连接
void outfile(char* name, char content[50000][1024], int length, int& index);//将收到的字符串转化成文件
int recvfile(message a);//接收文件并存入数组
int byeser(message t);//四次挥手

//////////////////////////////////////////////////



int tackle(message b)//处理收到的报文
{
	if (b.get_syn())
	{
		if (buildconnectionSer(b))
		{
			status = 1;
			cout << "建立连接" << endl;
		}
		else cout << "连接建立失败" << endl;
	}
	else if (b.get_startfile())
	{
		if (status)
		{
			cout << "开始接收文件" << endl;
			recvfile(b);
		}
		else cout << "请先建立连接" << endl;
	}
	if (b.get_fin())
	{
		if (status)
		{
			if (byeser(b)) {
				status = 0;
				sendseq = 0;
			}
		}
		else cout << "未连接" << endl;
		cout << "对方已经断开连接，是否结束程序？0退出，1不退出 ";
		int i;
		cin >> i;
		if (i == 0)
			return 0;
	}
	return 1;
}






int buildconnectionSer(message t)//服务端建立连接
{
	message a,b;
	a.set_ack(t);
	a.msgseq = sendseq++;
	simplesend(a);
	cout << "连接成功";
	return 1;
}


int recvfile(message a)
{
	message t;
	t.set_ack(a);
	a.msgseq = sendseq++;
	simplesend(t);
	
	int index = a.index;
	int length = a.filelength;
	char name[30];
	memset(name, 0, 30);
	for (int i = 0; a.msg[i]; i++)
		name[i] = a.msg[i];

	//char content[10000][1024];
	for (int i = 0; i <= index; i++)
	{
		message b, c;
		memset(content[i], 0, 1024);
		
		if (stopwaitrecv(b, c))
		{
			//strcpy(content[i], b.msg);
			if (i == index)
			{
				for (int j = 0; j < length; j++)
					content[i][j] = b.msg[j];
			}
			else
			{
				for (int j = 0; j < 1024; j++)
					content[i][j] = b.msg[j];
			}

		}
		else
		{
			cout << "出错0" << endl;
			return 0;
		}
		if (i == index)
		{
			if(!b.get_endfile())
			{
				cout << "出错1" << endl;
				return 0;
			}
		}
	}
	outfile(name, content, length, index);
	cout <<name<< " 文件接收成功" << endl;
	return 1;
}




void outfile(char* name, char content[50000][1024], int length, int& index)
{
	ofstream fout(name, ofstream::binary);
	for (int i = 0; i < index; i++)
	{
		for (int j = 0; j < FILE_PACKET_LENGTH; j++)
			fout << content[i][j];
	}
	for (int j = 0; j < length; j++)
		fout << content[index][j];
	fout.close();
}

int byeser(message t)//三次挥手,已经收到以此fin
{
	message a;
	a.set_ack(t);
	a.msgseq = sendseq++;
	simplesend(a);
	return 1;
}
