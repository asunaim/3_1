#include "function.h"
//#include "function.cpp"
#include <iostream>
#include<Windows.h>
using namespace std;

int m = 0x11B;//8次不可约多项式
int main()
{
	int a = 0x57, b = 0x81;
	int q, r,t;
	//mulbit(a, b, r);
	//divmod(b,a, q, r);
	r=getinv(b);
	mulbit(b, r, q);
	divmod(q, m, q, r);
	int x = 0b01010001;
	int y = change(x);
	cout << hex << endl;
}

void add(int a, int b, int& c)//c=a+b
{
	c = a ^ b;
}
void sub(int a, int b, int& c)//c=a-b、
{
	c = a ^ b;
}
int getdeg(int a)//a是几次
{
	for (int i = 31; i >= 0; i--)
	{
		if (getbit(a, i))
			return i;
	}
	return 0;
}
int getbit(int a, int j)//取一个字节中的第几次项位，最高位为第7位
{
	int b = a >> j;
	return b % 2;
}
void writebit(int& a, int j)//向一个字节的第几次项位写入数值1
{
	int b = 1 << j;
	a += b;
}
void mulbit(int a, int b, int& r)//r=(a*b)%m,在GF(2^8)下的乘法
{
	r = 0;
	int len = getdeg(b);
	for (int i = len; i >= 0; i--)
	{
		if (getbit(b, i))
		{
			int temp = a << i;
			add(r, temp, r);
		}
	}
	while (r >= m)
	{
		int deg = getdeg(r) - 8;
		int temp = m << deg;
		sub(r, temp, r);
	}
}
void divmod(int a, int b, int& q, int& r)//求商和余数,a/b
{
	int len = getdeg(b);
	r = a;
	q = 0;
	while (r >= b)
	{
		int lenlarge = getdeg(r);
		writebit(q, lenlarge - len);
		int temp = b << (lenlarge - len);
		sub(r, temp, r);
	}
}
int getinv(int a)//求逆元,扩展欧几里得     
{
	int r[100] = {};
	int q[100] = {};
	//int s[100] = {};
	int t[100] = {};
	//s[0] = 1; 
	t[1] = 1; 
	//s[1] = 0; 
	t[0] = 0;//赋初值
	r[0] = m;
	if (a > m)
	{
		int q_ = 0;
		divmod(a, m, q_, r[1]);
	}
	else r[1] = a;
	int round = 1;
	while (r[round] != 1)
	{
		divmod(r[round - 1], r[round], q[round], r[round + 1]);
		round++;
	}
	for (int i = 2; i <= round; i++)
	{

		//s[i] = s[i - 2] - q[i - 1] * s[i - 1];
		int product = 0;
		mulbit(t[i - 1], q[i - 1], product);
		sub(t[i - 2], product, t[i]);
	}
	return t[round];
}
int roundbyte(int a)//一个字节右循环移位一位
{
	int temp = getbit(a, 0);
	int b = ((BYTE)a) >> 1;
	if (temp)//循环移位
		writebit(b, 7);
	return b;
}
int change(int x)//放射变换
{
	int a[8];
	a[0] = 0b10001111;
	int x_[8] = {};
	int y_[8] = {};
	int y=0;
	for (int i = 0; i < 8; i++)
		x_[i] = getbit(x, i);
	for (int i = 1; i < 8; i++)
		a[i] = roundbyte(a[i - 1]);
	for (int i = 0; i < 8; i++)//第i行，右移i位
	{
		for (int j = 0; j < 8; j++)
		{
			int temp=getbit(a[i], j)* x_[j];
			add(y_[i], temp, y_[i]);
		}
	}
	add(y_[0], 1, y_[0]);
	add(y_[1], 1, y_[1]);
	add(y_[5], 1, y_[5]);
	add(y_[6], 1, y_[6]);

	for (int i = 0; i < 8; i++)
	{
		if (y_[i])
		{
			writebit(y, i);
		}
	}
	return y;
}
