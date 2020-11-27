//DES的加密和解密
//加密和解密只是密钥的使用顺序相反，其余相同
#include"head.h"
#include"data.h"
#include<stdio.h>
//将字节索引该为字索引
int byte2bit(tuple_ a)
{
	return a.byte_ * 8 + a.bit_;
}
//将字索引改为字节索引
tuple_ bit2byte(int a)
{
	tuple_ b;
	b.byte_ = a / 8;
	b.bit_ = a % 8;
	return b;
}


//取第几位
int getbit(BYTE a, int x)//从左至右第0-7位
{
	a = a << x;
	a = a >> 7;
	return a;
}
//在第几位写入
void write(BYTE& a, int x, int num)//将num写入a的第x位
{
	a += num << (7 - x);
}

//做对应转换
void convert(BYTE a[], BYTE b[], int i, int conv[])//conv规则中的第i位完成转换写入到b中
{
	int index_bit_b = i;
	tuple_ index_byte_b = bit2byte(index_bit_b);
	int index_bit_a = conv[index_bit_b] - 1;
	tuple_ index_byte_a = bit2byte(index_bit_a);
	int temp = getbit(a[index_byte_a.byte_], index_byte_a.bit_);
	write(b[index_byte_b.byte_], index_byte_b.bit_, temp);

}



//初始置换
void convIP(BYTE a[], BYTE b[])//将a进行IP置换，结果存入b中
{
	for (int i = 0; i < 64; i++)
	{
		convert(a, b, i, IP);
	}
}
//初始置换逆置换IP^(-1)
void convIP_1(BYTE a[], BYTE b[])//逆初始置换，将a进行IP置换，结果存入b中
{
	for (int i = 0; i < 64; i++)
		convert(a, b, i, IP_1);
}





//密钥产生
void getkeys(BYTE key[8], BYTE key_final[16][6])
{
	//ofstream("keys.txt");
	BYTE keys[7] = {};
	for (int i = 0; i < 56; i++)
		convert(key, keys, i, PC_1);
	bitset<56> key_round[17];
	//初始化全为0
	for (int i = 0; i < 16; i++)key_round[i].reset();
	//信息复制
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (getbit(keys[i], j) == 1)
				key_round[0].set(i * 8 + j);
		}
	}
	for (int i = 0; i < 16; i++)
	{
		bitset<56> left_mov = key_round[i] >> MOV[i];//注意大小端序问题，实际是左移
		for (int j = 0; j < MOV[i]; j++)
		{
			left_mov.set(28 - MOV[i] + j, key_round[i][j]);
			left_mov.set(56- MOV[i] + j, key_round[i][28 + j]);
		}
		key_round[i+1] = left_mov;
	}

	//PC_2置换
	for(int i=0;i<16;i++)
		for (int j = 0; j < 48; j++)
		{
			write(key_final[i][j/8], j%8, key_round[i + 1][PC_2[j] - 1]);
		}
}



//轮加密
void round(BYTE m_[8], BYTE key_final[16][6], BYTE afterIP_1[8])
{
	BYTE m[8] = {};
	convIP(m_, m);
	BYTE left[4], right[4];
	for (int i = 0; i < 4; i++)
	{
		left[i] = m[i];
		right[i] = m[i+4];
	}
 	for (int i = 0; i < 16; i++)//十六轮加密
	{
		cout << i<<": left ";
		for (int i = 0; i < 4; i++)//cout << getbit(left[i/8], i%8);
			printf("%x ", left[i]);
		cout << "right ";
		for (int i = 0; i <4; i++)//cout << getbit(right[i / 8], i % 8);
			printf("%x ", right[i]);
		cout << endl;

		BYTE afterE[6] = {};//扩展置换后的结果
		//扩展置换E
		convertE(right,afterE);
		//与密钥进行逐bit异或
		BYTE afterXOR[6] = {};
		xor_(key_final[i], afterE, afterXOR);
		//S盒代换
		BYTE afterS[4] = {};
		convertS(afterXOR, afterS);
		//P置换
		BYTE afterP[4] = {};
		convertP(afterS, afterP);
		BYTE afterXOR2[4] = {};
		for (int i = 0; i < 4; i++)//逐比特异或
		{
			afterXOR2[i] = left[i] ^ afterP[i];
		}
		//生成下一轮的left和right
		for (int i = 0; i < 4; i++)left[i] = right[i];
		for (int i = 0; i < 4; i++)right[i] = afterXOR2[i];
		int ix = 0;
	}
	cout << "16: left ";
	for (int i = 0; i < 4; i++)printf("%x ", left[i]);
	cout << "right ";
	for (int i = 0; i < 4; i++)printf("%x ", right[i]);
	cout << endl;



	//左右交换
	{
		BYTE temp[4] = {};
		for (int i = 0; i < 4; i++)
		{
			temp[i] = left[i];
			left[i] = right[i];
			right[i] = temp[i];
		}
	}
	//合并
	BYTE afterCombine[8];
	for (int i = 0; i < 4; i++)
	{
		afterCombine[i] = left[i];
		afterCombine[i + 4] = right[i];
	}
	//逆初始置换
	convIP_1(afterCombine, afterIP_1);
}




//扩展置换
void convertE(BYTE a[4], BYTE b[6])
{
	for (int i = 0; i < 48; i++)
		convert(a, b, i, E);
}
//48比特异或
void xor_(BYTE a[6], BYTE b[6], BYTE c[6])
{
	for(int i=0;i<6;i++)
		for (int j = 0; j < 8; j++)
		{
			int temp = getbit(a[i], j) ^ getbit(b[i], j);
			write(c[i], j, temp);
		}
}
//S盒
void convertS(BYTE a[6],BYTE b[4])
{
	bitset<48> a_;
	BYTE b_[8];
	a_.reset();
	//数据类型转换
	for (int i = 0; i < 48; i++)
	{
		a_.set(i, getbit(a[i / 8], i % 8));
	}
	//S盒
	for (int i = 0; i < 8; i++)
	{
		//行
		int box_index = a_[i * 6] * 2 + a_[i * 6 + 5];
		//列
		int line_index = 0;
		for (int j = 1; j <= 4; j++)
		{
			line_index = line_index * 2 + a_[i * 6 + j];
		}
		b_[i] = S[i][box_index*16+line_index];
	}
	for(int i=0;i<4;i++)
		for (int j = 0; j < 4; j++)
		{
			write(b[i], j, getbit(b_[2*i],4+j));
			write(b[i], j+4, getbit(b_[2*i+1],4+j));
		}
	int i = 0;
}
//P置换
void convertP(BYTE a[4], BYTE b[4])
{
	for (int i = 0; i < 32; i++)
		convert(a, b, i, P);
}

//int main()
//{
//	//BYTE a = 0;
//	//write(a, 1, 1);
//	//cout << "0x" <<hex<<(int)a<< endl;
// //   a = 254;
//	//cout<<getbit(a, 1)<<endl;
//	//cout<<getbit(a, 8)<<endl;
//
//	/*BYTE a[] = { 128,0,0,0,0,0,0,0,0 };
//	BYTE b[8] = {};
//	BYTE c[8] = {};
//	convIP(a, b);
//	convIP_1(b, c);*/
//
//	//BYTE key[8] = { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A };
//	//BYTE key_final[16][6];
//	//getkeys(key,key_final);
//
//	/*BYTE a[] = { 0x10, 0x31, 0x6E, 0x02 };
//	BYTE b[6] = {};
//	convertE(a, b);*/
//	BYTE a[] = { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F };
//	BYTE b[4] = {};
//	convertS(a, b);
//	
//	int i = 0;
//}
