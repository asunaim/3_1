#pragma once
#include "function.h"


int buildconnectionCli();//三次握手
//void readfile(char*& pBuffer,  int  &pSize);
int sendfile(char* name);//发送文件

void readfile(char* name, char content[10000][1024], int& length,  int  & index);
//void recvfile(char* name,char content[10000][1024], int length,  int  & index);


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
		int check=1;
		if (b.get_exist())
		{
			check = b.checkchecksum();
			if (b.get_ack()&&check)//b包含对消息a的ack和syn
				break;
		}
		clockend = clock();
		if (flag == SENT_TIMES)
		{
			cout << "连接失败" << endl;
			return 0;
		}
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME||b.get_nak())
		{
			flag++;
			clockstart = clock();
			simplesend(a);//重传
		}
		if (!check)
		{
			message c;
			c.set_nak();
			flag++;
			clockstart = clock();
			simplesend(c);
		}
	}
	message c;
	c.set_ack();
	simplesend(c);
	cout << "连接成功";
}

int sendfile(char* name)//发送文件
{
	// int  pSize=0;
	//char* pBuffer=nullptr;
	cout << "开始发送文件" << endl;
	int length = 0;
	 int index = 0;

	readfile( name,content,length,index);
	message a;
	a.index = index;
	a.filelength = length;
	int t = strlen(name);
	for (int i = 0; i < t; i++)
		a.msg[i]=name[i];//。msg已经初始化全部为0
	a.set_startfile();
	message b;
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
		if (stopwaitsend(temp,b)== 0)
		{
			cout << "文件发送失败" << endl;
			return 0;
		}
	}
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



