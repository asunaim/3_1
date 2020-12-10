#pragma once
#include "function.h"


HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;
DWORD WINAPI filehandler(LPVOID lparam1);

DWORD WINAPI messagehandler(LPVOID lparam);

message msgrecv[10000];

int tackle(message b);//处理收到的报文
int buildconnectionSer();//服务端建立连接
void outfile(char* name, char content[10000][1024], int length, int& index);//将收到的字符串转化成文件
int recvfile(message a);//接收文件并存入数组
int byeser();//四次挥手

//////////////////////////////////////////////////










int tackle(message b)//处理收到的报文
{
	if (b.get_syn())
	{
		if (buildconnectionSer())
			cout << "建立连接" << endl;
		else cout << "连接建立失败" << endl;
	}
	else if (b.get_startfile())
	{
		recvfile(b);
	}
	if (b.get_fin())
	{
		byeser();
	}
	return 1;
}


int buildconnectionSer()//服务端建立连接
{
	cout << "连接成功"<<endl;
	return 1;
}


int recvfile(message a)
{
	message t;
	t.set_ack();
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
			if (!b.get_endfile())
			{
				cout << "出错1" << endl;
				return 0;
			}
		}
	}
	outfile(name, content, length, index);
	cout << name << " 文件接收成功" << endl;
	return 1;
}


void outfile(char* name, char content[10000][1024], int length, int& index)
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

int byeser()//三次挥手,已经收到以此fin
{
	message a, b, c;
	a.set_ack();
	simplesend(a);
	cout << "连接断开成功" << endl;
	return 1;
}

//处理接收到的文件消息
DWORD WINAPI filehandler(LPVOID lparam)
{
	/*filepacket* pkt = (filepacket*)(LPVOID)lparam;
	memset(content[pkt->index], 0, 1024);
	for(int j=0;j<pkt->length;j++)
	{
		content[pkt->index][j] = pkt->a.msg[j];
	}*/
	return 0;
}

DWORD WINAPI messagehandler(LPVOID lparam)//处理收到的消息
{
	message a = *(message*)(char*)(LPVOID)lparam;
	if (a.get_syn())
	{
		if (!buildconnectionSer())
			return 0;
		else status = 1;
	}
	else if (status && a.get_startfile())
	{
		//cout << "接收文件" << endl;
		memset(name, 0, sizeof(name));
		filestatus = 1;
		index = a.index;
		length = a.filelength;
		strcpy(name, a.msg);
	}
	else if (status && filestatus && a.get_endfile())//文件发送结束
	{
		//cout << "接收文件" << endl;
		filestatus = 0;

		if (fileseq != index)
		{
			cout << "出错" << endl;
			return 0;
		}
		/*filepacket* packet = new filepacket;
		packet->a = a;
		packet->index = index;
		packet->length = length;
		hThread1 = ::CreateThread(NULL, NULL, filehandler, LPVOID(packet), 0, &dwThreadId1);*/
		for (int i = 0; i < length; i++)
		{
			content[index][i] = a.msg[i];
		}
		//WaitForSingleObject(hThread1, 1000);
		fileseq++;
		fileseq = 0;
		//

		cout << "文件接收结束" << endl;
	}
	else if (status && filestatus)
	{
		fileseq++;

		/*filepacket* packet = new filepacket;
		packet->a = a;
		packet->index = fileseq - 1;
		packet->length = 1024;
		hThread2 = ::CreateThread(NULL, NULL, filehandler, LPVOID(packet), 0, &dwThreadId2);*/
		for (int i = 0; i < 1024; i++)
		{
			content[fileseq-1][i] = a.msg[i];
		}
		//WaitForSingleObject(hThread2, 1000);
	}
	else 
	return 1;
}
