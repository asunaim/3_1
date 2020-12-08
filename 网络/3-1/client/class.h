#pragma once
# define _CRT_SECURE_NO_WARNINGS
# define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中
using namespace std;
#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <WinSock2.h>//windows socket 编程头文件
#include<string.h>
#include<cstring>
#include<string>
#include<ctime>
#include <fstream>

clock_t clockstart, clockend;
int sendseq = 0;
int recvseq = 0;

#define SPORT 30000
#define CPORT 6666
#define BUF_SIZE 1024
#define WAIT_TIME 5//1s
#define SENT_TIMES 8//最多八次重传
#define FILE_PACKET_LENGTH 1024

int addr_len = sizeof(struct sockaddr_in);
SOCKET sock;
SOCKADDR_IN addrop, addr;//ip+端口号
char content[10000][1024];
struct cond//初始序号，接收缓冲区大小，最大段尺寸
{

};


#pragma pack(1)
struct message//报文格式
{
	//ACK=0x01, SYN=0x02, FIN=0x04, NAK=0x08,EXIST 0x10,startfile 0x20,endfile 0x40
	int flag;//标志位
	DWORD SendIP, RecvIP;
	int msgseq;
	int length;
	int SendPort, RecvPort;
	int fill;
	/*int ackseq;
	int synseq;
	int finseq;*/
	//int nakseq;//消息序号和确认的消息序号
	u_short checksum;
	int index;
	int filelength;
	char msg[BUF_SIZE];
	message();
	void set_send_ip(char* s);
	void set_recv_ip(char* s);
	void set_send_port(int s);
	void set_recv_port(int s);
	void set_ack();
	void set_syn();
	void set_fin();
	void set_nak();
	void set_exist();
	void set_startfile();
	void set_endfile();

	int get_ack();
	int get_syn();
	int get_fin();
	int get_nak();
	int get_exist();
	int get_startfile();
	int get_endfile();

	void setchecksum();//设置校验和
	bool checkchecksum();//检验校验和

	void print();//输出标志位
};
#pragma pack()


message::message() {
	memset(this, 0, sizeof(message));//初始化全0
}

void message::set_send_ip(char* s) {}
void message::set_recv_ip(char* s) {}
void message::set_send_port(int s) {}
void message::set_recv_port(int s) {}
//ACK=0x01, SYN=0x02, FIN=0x04, NAK=0x08
void message::set_ack()
{
	if (get_ack() == 0)
		flag += 0x01;
}
void message::set_syn()
{
	if (get_syn() == 0)
		flag += 0x02;
}
void message::set_fin()
{
	if (get_fin() == 0)
		flag += 0x04;
}
void message::set_nak()
{
	if (get_nak() == 0)
		flag += 0x08;
}
void message::set_exist()
{
	if (get_exist() == 0)
		flag += 0x10;
}

void message::set_startfile()
{
	if (get_startfile() == 0)
		flag += 0x20;
}
void message::set_endfile()
{
	if (get_startfile() == 0)
		flag += 0x40;
}

int message::get_ack()
{
	if (this->flag & 0x01)
		return 1;
	else return 0;
}
int message::get_syn()
{
	if (this->flag & 0x02)
		return 1;
	else return 0;
}
int message::get_fin()
{
	if (this->flag & 0x04)
		return 1;
	else return 0;
}
int message::get_nak()
{
	if (this->flag & 0x08)
		return 1;
	else return 0;
}
int message::get_exist()
{
	if (this->flag & 0x10)
		return 1;
	else return 0;
}
int message::get_startfile()
{
	if (this->flag & 0x20)
		return 1;
	else return 0;
}
int message::get_endfile()
{
	if (this->flag & 0x40)
		return 1;
	else return 0;
}

void message::print()//输出标志位
{
	if (get_ack())cout << "ACK ";
	if (get_syn())cout << "SYN ";
	if (get_fin())cout << "FIN ";
	if (get_nak())cout << "NAK ";
	if (get_exist())cout << "EX ";
	if (get_startfile())cout << "SF ";
	if (get_endfile())cout << "EF ";
	cout << endl;
}

void message::setchecksum()
{
	int sum = 0;
	u_char* temp = (u_char*)this;
	for (int i = 0; i < 16; i++)
	{
		sum += temp[2 * i] << 8 + temp[2 * i + 1];
		while (sum >= 0x10000)
		{//溢出
			int t = sum >> 16;
			sum += t;
		}
	}
	u_short a = (u_short)sum;
	this->checksum = ~(u_short)sum;
	u_short t = a + ~sum;
}

bool message::checkchecksum()
{
	int sum = 0;
	u_char* temp = (u_char*)this;
	for (int i = 0; i < 16; i++)
	{
		sum += temp[2 * i] << 8 + temp[2 * i + 1];
		while (sum >= 0x10000)
		{//溢出
			int t = sum >> 16;
			sum += t;
		}
	}
	if (checksum + (u_short)sum == 65535)
		return true;
	return false;
}



