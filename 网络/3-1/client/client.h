#pragma once
#include "function.h"


int buildconnectionCli();//三次握手
//void readfile(char*& pBuffer,  int  &pSize);
int sendfile(char* name);//发送文件

void readfile(char* name, char content[10000][1024], int& length,  int  & index);
//void recvfile(char* name,char content[10000][1024], int length,  int  & index);
int byecli();//因为只是单向传输，写三次挥手即可


int buildconnectionCli()//客户端，连接发起方
{
	message a, b;
	a.set_syn();
	a.msgseq = sendseq++;
	if (stopwaitsend(a, b))return 1;
	else return 0;
}

int sendfile(char* name)//发送文件
{
	// int  pSize=0;
	//char* pBuffer=nullptr;
	cout << "开始发送文件" << endl;
	int length = 0;
	 int index = 0;

	readfile( name,content,length,index);
	clock_t timestart = clock();
	message a;
	a.index = index;
	a.filelength = length;
	int t = strlen(name);
	for (int i = 0; i < t; i++)
		a.msg[i]=name[i];//。msg已经初始化全部为0
	a.set_startfile();
	message b;
	a.msgseq = sendseq++;
	if (!stopwaitsend(a, b))//发送消息a
	{
		cout << "文件传输失败" << endl;
		return 0;
	}
	for (int i = 0; i <= index; i++)
	{
		message temp;
		if (i == index)
		{
			temp.set_endfile();
			for (int j = 0; j < length; j++)
				temp.msg[j] = content[i][j];
		}
		else
		{
			for (int j = 0; j < 1024; j++)
				temp.msg[j] = content[i][j];
		}
		//strcpy(temp.msg, content[i]);
		temp.msgseq = sendseq++;
		if (stopwaitsend(temp,b)== 0)
		{
			cout << "文件发送失败" << endl;
			return 0;
		}
	}
	clock_t timeend = clock();
	double endtime = (double)(timeend - timestart) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime << endl;		//s为单位
	cout << "吞吐率：" << (double)(index + 1) * sizeof(message) * 8/endtime /1024/1024<< "Mbps" << endl;
	cout << "文件发送成功" << endl;
	return 1;

	//recvfile(name,content,length,index);
	//readfile(pBuffer, pSize);
	//recvfile(pBuffer, pSize);
	//最后要删除
	/*delete[] pBuffer;
	pBuffer = 0;*/
}


void readfile(char* name,char content[10000][1024],int &length,  int  & index)
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
	//第一次挥手，发送FIN
	message a,b;
	a.set_fin();//第一次
	a.msgseq = sendseq++;
	if (stopwaitsend(a, b))return 1;
	else return 0;

	//if (stopwaitrecv(b, c))//收到的消息写入b中，第二次和第三次
	//{
	//		message d;
	//		d.set_ack(b);
	//		simplesend(d);
	//		{
	//			cout << "成功断开连接" << endl;
	//			return 1;
	//		}
	//}
	//else
	//{
	//	cout << "断开连接失败" << endl;
	//	return 0;
	//}
}