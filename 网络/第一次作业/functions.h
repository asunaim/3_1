#pragma once
# define _CRT_SECURE_NO_WARNINGS
# define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <WinSock2.h>//windows socket 编程头文件
#include<string.h>
#include<cstring>
#include<string>

#define BUF_SIZE 2048
#define PORT 6666
#define CLIENTNUM 2

//报文格式
class msg_form
{
public:

	char from_name;//发送方标号
	char to_name;//接收方
	char msg[BUF_SIZE];//消息内容
	msg_form() { memset(msg, 0, BUF_SIZE); }
	msg_form(const char a, const char b, const char c[BUF_SIZE])
	{
		from_name = a;
		to_name = b;
		strcpy(msg, c);

	}
	msg_form(char a, char b, char c[BUF_SIZE])
	{

		from_name = a;
		to_name = b;
		strcpy(msg, c);

	}
};



//函数声明
char* msg_to_string(msg_form a);//用字符串存储报文
msg_form string_to_msg(char a[]);//将字符串恢复成类








//函数定义
char* msg_to_string(msg_form a)//将报文格式转为字符串
{
	//char from_name[4];//发送方标号
	//char to_name[4];//接收方
	//char msg[BUF_SIZE];//消息内容
	char* c = new char[BUF_SIZE + 6];

	c[0] = a.from_name;
	c[1] = a.to_name;
	c[2] = 0;
	strcat(c, a.msg);

	return c;
}
msg_form string_to_msg(char a[])//将字符串恢复成类
{
	msg_form msg_;
	msg_.from_name = a[0];
	msg_.to_name = a[1];
	int i;
	for (i = 0; a[i + 2]; i++)
	{
		msg_.msg[i] = a[i + 2];
	}


	return msg_;
}
