#pragma once

#include<ctime>
#include<cmath>
#include<iostream>
#include<cstring>

using namespace std;

#define INT_MAX 4294967296

//线性同余算法
unsigned int LINEAR_M = INT_MAX-1;
unsigned int LINEAR_A = 16807;


int change(char a);

class bigint
{
public:
	unsigned int num[64];
	//int deg;//多少个32位-1
	bigint() ;
	//bigint(int x);
	void in(string);//16进制输入
	void copy(bigint a);//深拷贝
	void writebit(int x, int y);//向二进制的第x次项写入y
	int getbit(int x);//取二进制的第x次项
	void div(unsigned int b,bigint &q,bigint &r);//num%b==r
	unsigned long long  getint(unsigned int x, unsigned int y);//取出大整数的第x次项到第y次项  0<=（y-x）<32
	void writenum(bigint b,int j);//最低项第j次项写入数b

	void add(unsigned int x);//+x
	void sub(unsigned int x);//-x
	void sub(unsigned int x,int j);
	//void mul(unsigned int x, bigint temp);
	void leftmove(bigint &temp);//左移一位
	void leftmove4(bigint& temp);//左移四位
	bool check0();//检查是否是0
	bool equal(unsigned int x);//相等返回1，不等返回0
	void print();
	
	static void getinv(bigint a, bigint b,bigint&c);//求逆元a>b b mod a


	static void mul(bigint a, unsigned long long b, bigint& c);//c=a*b

	static void add(bigint a, bigint b, bigint &c);//c=a+b
	static void sub(bigint a, bigint b, bigint &c);//c=a-b
	static void mul(bigint a, bigint b, bigint &c);//c=a*b
	static void mod(bigint a, bigint b, bigint &c);//c=a%b
	static void mod(bigint a, unsigned int  b, bigint& c);//c=a%b

	static int compare(bigint a, bigint b);//a>b 1，a=b 0, a<b -1
	static void pow_(bigint m, bigint e, bigint n, bigint& c);//c=m^e mod n

};





class big//十六进制
{
public:
	int num[256];
	big();
	void copy(big);
	void print();
	void set(unsigned int a);
	void set(int a);
	void set(string a);
	void set(string a, int x);
	void set(unsigned long long a);
	int getbit(int i);//第2^i位
};
big add(big a, big b);
big sub(big a, big b);
big sub(big a, big b, int d);//偏移
int compare(big a, big b,int d);
int compare(big a, big b);
big mul(big a, big b);
big mod(big a, big b);
big pow(big a, big b,big n);//a^b mod n
big getinv(big a, big b);
big div(big a, big b,big &c);//返回商，r是余数

unsigned int smallprime[5000];
void getsmallprime();
class prime {
public:
	big number;
	prime();
	prime(string a);
	prime(string a,int x);
	void getarbitrary();//生成512bit随机数，奇数
	void check2000();//检查是否能被2000以内的素数整除,若不能则加2，直至可以
	void millerrabin();//检查是否是素数
};


class RSA_//加密
{
public:
	big n, phi;
	big q;
	big p;
	big d, e;
	//big c, m;//密文和明文

	RSA_(big p, big q,big e);

};






class RSAen_//加密
{
public:
	big n, e;
	big c, m;//密文和明文

	RSAen_(RSA_ a, big m);


};

class RSAde_//加密
{
public:
	big n, d;
	big c, m;//密文和明文
	RSAde_(RSA_ a, big c);
};



/////////////////////////

//
//a = [2, 3, 13, 17, 19]
//
//flag = 1
//
//
//
//for k in a :
//print("----------------")
//print(k, ":")
//i = 511
//d = 1
//n = 0xc0332681a37e694c87062174073e3cbf2bab7b94f97bc0bfb67dbed515463db7c25fa4f29da1340a55a46bcc9fffd79ce3964aa9542f26b50001484300000007
//while i >= 0:
//print(i)
//x = d
//d = (d * d) % n
//print(hex(d))
//if d == 1 :
//	if x != 1 and x != n - 1 :
//		flag = 0
//		break
//		if ((n - 1) & (2 * *i)) != 0 :
//			d = (d * k) % n
//			print(hex(d))
//			i -= 1
//
//			if d != 1 :
//				flag = 0
//
//				print(flag)

