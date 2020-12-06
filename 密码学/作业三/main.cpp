#include "function.h"
//#include "function.cpp"
#include"testdata.h"
#include <iostream>
#include<Windows.h>
using namespace std;


struct testdata
{
	int Nb, Nr, Nk;
	int keysize;
	int state[16];
	int key[16];
	int cipher[16];
}cases[2] = {
	{
		4,10,4,128,
	{0x00,0x01,0x00,0x01,0x01,0xa1,0x98,0xaf,0xda,0x78,0x17,0x34,0x86,0x15,0x35,0x66},
	{0x00,0x01,0x20,0x01,0x71,0x01,0x98,0xae,0xda,0x79,0x17,0x14,0x60,0x15,0x35,0x94},
	{0x6c,0xdd,0x59,0x6b,0x8f,0x56,0x42,0xcb,0xd2,0x3b,0x47,0x98,0x1a,0x65,0x42,0x2a}
},{
	4,10,4,128,
	{0x32,0x43, 0xf6,0xa8, 0x88,0x5a, 0x30,0x8d, 0x31,0x31, 0x98,0xa2, 0xe0,0x37, 0x07,0x34},
	{0x2b,0x7e, 0x15,0x16, 0x28,0xae, 0xd2,0xa6, 0xab,0xf7, 0x15,0x88, 0x09,0xcf, 0x4f,0x3c},
	{0x39,0x25, 0x84,0x1d, 0x02,0xdc, 0x09,0xfb, 0xdc,0x11, 0x85,0x97, 0x19,0x6a, 0x0b,0x32}
}
};


int C[3][4] = { {0,1,2,3},{0,1,2,3},{0,1,3,4}};


int m = 0x11B;//8次不可约多项式
int Nb, Nr, Nk;
int* state, * key, * cipher;//
int** stateword;//将一维的状态排列成二维
int Nr_[3][3] = { {10,12,14},{12,12,14},{14,14,14} };
int shiftindex;//行移位不同分组的位移量



int main()
{

	initializeen(0);
	encode();
	initializede(0);
	decode();
	initializeen(1);
	encode();
	initializede(1);
	decode();
	
}


void initializeen(int index)//初始化，明文密钥输入
{
	Nb = cases[index].Nb;
	Nr = cases[index].Nr;
	Nk = cases[index].Nk;
	if (state == nullptr)
	{
		state = new int[4 * Nb];
		key = new int[4 * Nk];
		cipher = new int[4 * Nb];
	}
		for (int i = 0; i < Nb * 4; i++)
		{
			state[i] = cases[index].state[i];
			cipher[i] = cases[index].cipher[i];
		}
		for (int i = 0; i < Nk * 4; i++)
		{
			key[i] = cases[index].key[i];
		}

		stateword = new int* [4];
		for (int i = 0; i < 4; i++)
		{
			stateword[i] = new int[Nb];
		}
	
	shiftindex = (Nb - 4) / 2;
	change1to2(state,stateword,Nb);
}
void initializede(int index)//初始化
{
	Nb = cases[index].Nb;
	Nr = cases[index].Nr;
	Nk = cases[index].Nk;
	if (state == nullptr)
	{
		state = new int[4 * Nb];
		key = new int[4 * Nk];
		cipher = new int[4 * Nb];
	}
		for (int i = 0; i < Nb * 4; i++)
		{
			state[i] = cases[index].cipher[i];
			cipher[i] = cases[index].state[i];
		}
		for (int i = 0; i < Nk * 4; i++)
		{
			key[i] = cases[index].key[i];
		}

		stateword = new int* [4];
		for (int i = 0; i < 4; i++)
		{
			stateword[i] = new int[Nb];
		}
	shiftindex = (Nb - 4) / 2;
	change1to2(state, stateword, Nb);
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
void divmodm(int a, int& r)//mod x^8+x^4+x^3+x+1
{
	int q = 0;
	while (r > 0x100)
	{
		int lenlarge = getdeg(r);
		writebit(q, lenlarge - 8);
		int temp = m << (lenlarge - 8);
		sub(r, temp, r);
	}
}
int getinv(int a)//求逆元,扩展欧几里得     
{
	if (a == 0)return 0;
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


int roundbit(int a)//一个字节按位右循环移位一位
{
	int temp = getbit(a, 0);
	int b = ((BYTE)a) >> 1;
	if (temp)//循环移位
		writebit(b, 7);
	return b;
}

int roundbit2(int a)////一个字节右循环移位两位
{
	int temp = getbit(a, 0);
	int temp1 = getbit(a, 1);
	int b = ((BYTE)a) >> 2;
	if (temp)//循环移位
		writebit(b, 6);
	if (temp1)
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
		a[i] = roundbit(a[i - 1]);
	for (int i = 0; i < 8; i++)//第i行，右移i位
	{
		for (int j = 0; j < 8; j++)
		{
			int temp=getbit(a[i], j)* x_[7-j];
			//int s = 0;
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
int Invchange(int x)//仿射变换
{
	int a[8];
	a[0] = 0b00100101;
	int x_[8] = {};
	int y_[8] = {};
	int y = 0;
	for (int i = 0; i < 8; i++)
		x_[i] = getbit(x, i);
	for (int i = 1; i < 8; i++)
		a[i] = roundbit(a[(i-1)]);
	add(x_[0], 1, x_[0]);
	add(x_[1], 1, x_[1]);
	add(x_[5], 1, x_[5]);
	add(x_[6], 1, x_[6]);
	for (int i = 0; i < 8; i++)//第i行，右移i位
	{
		for (int j = 0; j < 8; j++)
		{
			int t = getbit(a[i], j);
			int s= x_[7 - j];
			int temp = getbit(a[i], j) * x_[7 - j];
			//int s = 0;
			add(y_[i], temp, y_[i]);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (y_[i])
		{
			writebit(y, i);
		}
	}
	return y;
}



void ByteSub()//字节代换
{
	for (int i = 0; i < Nb; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int temp = getinv(state[i * 4 + j]);
			int temp2 = change(temp);
			state[i * 4 + j] = temp2;
		}
	}
}

void InvByteSub()//字节代换
{
	for (int i = 0; i < Nb; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int s = Invchange(0x2a);
			int temp2 = Invchange(state[i * 4 + j]);
			int temp = getinv(temp2);
			state[i * 4 + j] = temp;
		}
	}
}
void ShiftRow()//行移位
{
	change1to2(state, stateword,Nb);
	for (int i=1; i < 4; i++)
	{
		int* temp = new int[Nb];
		shiftsinglerow(stateword[i], temp, C[shiftindex][i]);
		for (int j = 0; j < Nb; j++)
		{
			stateword[i][j] = temp[j];
		}
	}
	change2to1(state, stateword, Nb);
}

void InvShiftRow()//行移位,对stateword进行修改
{
	change1to2(state, stateword, Nb);
	for (int i = 1; i < 4; i++)
	{
		int* temp = new int[Nb];
		shiftsinglerow(stateword[i], temp, Nb-C[shiftindex][i]);
		for (int j = 0; j < Nb; j++)
		{
			stateword[i][j] = temp[j];
		}
	}
	change2to1(state, stateword, Nb);
}
void shiftsinglerow(int a[], int b[], int shift)//将a左移shift位存入b中
{
	//int* temp = new int[Nb];
	for (int i = 0; i < Nb; i++)
	{
		b[i] = a[(i + shift) % Nb];
	}
}


void change1to2(int* a, int** b,int x)//将长为4*x的一维数组排列成二维数组
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < 4; j++)//第j行第i列
		{
			b[j][i] = a[i * 4 + j];
		}
	}
}
void change2to1(int* a, int** b, int x)//列数x，二维数组转一维；
{
	for (int j = 0; j < x; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			a[j * 4 + i] = b[i][j];
		}
	}
}
void mulbyte(int *a, int *b,  int div)//多项式乘法运算,b=A*a
{
	int c[] = { 2,1,1,3 };//列混淆系数
	int** A = new int* [div];
	for (int i = 0; i < div; i++)
		A[i] = new int[div];
	for (int j = 0; j < div; j++)
	{
		for (int i = 0; i < div; i++)
		{
			A[i][j] = c[(i - j+4) % 4];
		}
	}
	for (int i = 0; i < div; i++)
	{
		b[i] = 0;
		for (int j = 0; j < div; j++)
		{
			int temp;
			mulbit(A[i][j], a[j], temp);
			//modx41(temp);
			add(temp, b[i], b[i]);
		}
		if (b[i] >=0x100)
		{
			int q = 0;
			divmodm(b[i],  b[i]);
		}

	}
}
void Invmulbyte(int* a, int* b, int div)//多项式乘法运算,c=a*b
{
	int c[4] = { 0xE,0x9,0xD,0xB };
	int** A = new int* [div];
	for (int i = 0; i < div; i++)
		A[i] = new int[div];
	for (int j = 0; j < div; j++)
	{
		for (int i = 0; i < div; i++)
		{
			A[i][j] = c[(i - j + 4) % 4];
		}
	}
	for (int i = 0; i < div; i++)
	{
		b[i] = 0;
		for (int j = 0; j < div; j++)
		{
			int temp;
			mulbit(A[i][j], a[j], temp);
			//modx41(temp);
			add(temp, b[i], b[i]);
		}
		if (b[i] >= 0x100)
		{
			int q = 0;
			divmodm(b[i], b[i]);
		}

	}
}
void modx41(int& a)//mod x^4+1
{
	int deg = getdeg(a);
	if (deg < 4)return;
	for (int i = deg; i >= 4; i--)
	{
		if (getbit(a, i) == 1) {
			writebit(a, i);
			writebit(a, i - 4);
		}
	}
}
void MixColumn()//列混合
{
	for (int j = 0; j < Nb; j++)
	{
		int a[4];
		for (int i = 0; i < 4; i++)
		{
			a[i] = stateword[i][j];
		}
		int b[4] = {};
		mulbyte(a, b, 4);
		for (int i = 0; i < 4; i++)
			a[i] = b[i];
		for (int i = 0; i < 4; i++)
		{
			stateword[i][j] = a[i];
		}
	}
	change2to1(state, stateword, Nb);
}
void InvMixColumnkey(int** key)//列混合
{
	for (int j = 0; j < Nb; j++)
	{
		int a[4];
		for (int i = 0; i < 4; i++)
		{
			a[i] = key[i][j];
		}
		int b[4] = {};
		Invmulbyte(a, b, 4);
		for (int i = 0; i < 4; i++)
			a[i] = b[i];
		for (int i = 0; i < 4; i++)
		{
			key[i][j] = a[i];
		}
	}
}
void InvMixColumn()//列混合
{
	for (int j = 0; j < Nb; j++)
	{
		int a[4];
		for (int i = 0; i < 4; i++)
		{
			a[i] = stateword[i][j];
		}
		int b[4] = {};
		Invmulbyte(a, b, 4);
		for (int i = 0; i < 4; i++)
			a[i] = b[i];
		for (int i = 0; i < 4; i++)
		{
			stateword[i][j] = a[i];
		}
	}
	change2to1(state, stateword, Nb);
}

void AddRoundKey(int** roundkey,int round)//密钥加
{
	for (int i = 0; i < Nb; i++)
	{
		for (int j = 0; j < Nb; j++)
		{
			add(stateword[i][j], roundkey[i][j+Nb*round], stateword[i][j]);
			divmodm(stateword[i][j], stateword[i][j]);
		}
	}
	change2to1(state, stateword, Nb);
}
void KeyExpansion(int* key, int** keyround)//密钥扩展
{
	if (Nk <= 6)
	{
		for (int i = 0; i < Nk; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				keyround[j][i] = key[4 * i + j];
			}
		}
		for (int i = Nk; i < Nb * (Nr + 1); i++)
		{
			int temp[4];
			for (int j = 0; j < 4; j++)
			{
				temp[j] = keyround[j][i - 1];
			}
			int temp2[4] = {};
			int temp3[4] = {};
			if (i % Nk == 0)
			{
				
				for (int k = 0; k < 4; k++)
				{
					//temp3[k] = getinv(0x95);
					temp3[k] = getinv(temp[(k+1)%4]);
					int r = 0;
					mulbit(temp3[k], temp[(k + 1) % 4], r);
					temp2[k] = change(temp3[k]);
				}
				int RC = 0;
				writebit(RC, i / Nk-1);
				temp[0] =temp2[0]^ RC;
				temp[1] = temp2[1] ^ 0;
				temp[2] = temp2[2] ^ 0;
				temp[3] = temp2[3] ^ 0;
			}
			
			for (int k = 0; k < 4; k++)
			{
				keyround[k][i] = temp[k]^keyround[k][i-Nk];
			}
			int s = 0;
		}
	}
	else
	{
		for (int i = 0; i < Nk; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				keyround[j][i] = key[4 * i + j];
			}
		}
		for (int i = Nk; i < Nb * (Nr + 1); i++)
		{
			int temp[4];
			for (int j = 0; j < 4; j++)
			{
				temp[j] = keyround[j][i - 1];
			}
			int temp2[4] = {};
			int temp3[4] = {};
			if (i % Nk == 0)
			{

				for (int k = 0; k < 4; k++)
				{
					temp3[k] = getinv(temp[(k + 1) % 4]);
					temp2[k] = change(temp3[k]);
				}
				int RC = 0;
				writebit(RC, i / Nk);
				temp2[0] ^= RC;
				temp2[1] ^= 0;
				temp2[2] ^= 0;
				temp2[3] ^= 0;
			}
			else if (i % Nk == 4)
			{
				for (int k = 0; k < 4; k++)
				{
					temp3[k] = getinv(temp[k]);
					temp2[k] = change(temp3[k]);
				}
			}
			for (int k = 0; k < 4; k++)
			{
				keyround[k][i] = temp2[k];
			}

		}
	}
}
void selectkey(int** a, int** roundkey,int round)//密钥选取,第i轮
{
	for (int j = Nb * round; j < Nb * (round + 1); j++)
	{
		for (int i = 0; i < 4; i++)
		{
			roundkey[i][j - Nb * round] = a[i][j];
		}
	}
}

void Round(int** key,int round)//轮加密
{
	ByteSub();
	ShiftRow();
	MixColumn();
	AddRoundKey(key,round);
}
void InvRound(int** key, int round)
{
	InvByteSub();
	InvShiftRow();
	
	InvMixColumn();
	AddRoundKey(key, Nr - round);
	
}




void Finalround(int** key)//最后一轮轮加密
{
	ByteSub();
	ShiftRow();
	AddRoundKey(key,Nr);
}

void InvFinalround(int** key)//最后一轮轮加密
{
	InvByteSub();
	InvShiftRow();
	AddRoundKey(key, 0);
}


void encode()//加密运算
{
	//change1to2(state, stateword, Nb);
	int** expandedkey = new int* [4];
	for (int i = 0; i < 4; i++)
		expandedkey[i] = new int [Nb*(Nr+1)];
	KeyExpansion(key, expandedkey);

	AddRoundKey(expandedkey,0);

	for (int i = 1; i < Nr; i++)
	{
		Round(expandedkey,i);
	}

	Finalround(expandedkey);
	change2to1(state, stateword, Nb);
	for (int i = 0; i <4* Nb; i++)
	{
		cout << "0x" << hex << state[i] << " ";
	}
	cout << endl;
}


void getInvkey(int** expandedkey)//解密密钥
{
	int** e = new int* [4];
	for (int i = 0; i < 4; i++)
		e[i] = new int[Nb * (Nr + 1)];
	/*for (int i = 0; i < Nb; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			e[j][Nr * Nb + i] = expandedkey[j][i];
			e[j][i] = expandedkey[j][Nr * Nb + i];
		}
	}*/
	//int** key = expandedkey;
	for (int i = 1; i < Nr; i++)
	{
		int** key = new int* [4];
		for (int j = 0; j < 4; j++)
		{
			key[j] = new int[Nb];
			for (int k = 0; k < Nb; k++)
				key[j][k] = expandedkey[j][k + i * Nb];
		}
		InvMixColumnkey(key);
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < Nb; k++)
				expandedkey[j][k + i * Nb] = key[j][k];
		}
	}
	//expandedkey = e;
}

void decode()//解密
{
	//change1to2(state, stateword, Nb);
	int** expandedkey = new int* [4];
	for (int i = 0; i < 4; i++)
		expandedkey[i] = new int[Nb * (Nr + 1)];
	KeyExpansion(key, expandedkey);
	getInvkey(expandedkey);
	AddRoundKey(expandedkey, Nr);

	for (int i = 1; i < Nr; i++)
	{
		InvRound(expandedkey, i);
	}

	InvFinalround(expandedkey);
	change2to1(state, stateword, Nb);
	for (int i = 0; i < 4 * Nb; i++)
	{
		cout <<"0x"<< hex << state[i] << " ";
	}
	cout << endl;
}


