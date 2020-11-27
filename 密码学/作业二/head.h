#pragma once
#include <iostream>
#include<fstream>
#include<windows.h>
#include<iomanip>
#include<bitset>
using namespace std;

class tuple_
{
public:
	int byte_;//第几个字节，0开始
	int bit_;//第几位，从左至右，0开始
	tuple_() {};
	tuple_(int a, int b) {
		byte_ = a;
		bit_ = b;
	}
};
//将字节索引该为字索引
int byte2bit(tuple_ a);
//将字索引改为字节索引
tuple_ bit2byte(int a);

//取第几位
int getbit(BYTE a, int x);//从左至右第0-7位
//在第几位写入
void write(BYTE& a, int x, int num);
//做对应转换
void convert(BYTE a[], BYTE b[], int i, int conv[]);

//初始置换
void convIP(BYTE a[], BYTE b[]);//将a进行IP置换，结果存入b中
//初始置换逆置换IP^(-1)
void convIP_1(BYTE a[], BYTE b[]);//逆初始置换，将a进行IP置换，结果存入b中
//密钥生成
void getkeys(BYTE key[8], BYTE key_final[16][6]);
//扩展置换
void convertE(BYTE a[4], BYTE b[6]);
//48bit异或，结果存到c中
void xor_(BYTE a[6], BYTE b[6], BYTE c[6]);

//S盒
void convertS(BYTE a[6], BYTE b[4]);
void convertP(BYTE a[4], BYTE b[4]);

//轮加密
void round(BYTE m[8], BYTE key_final[16][6], BYTE afterIP_1[8]);

