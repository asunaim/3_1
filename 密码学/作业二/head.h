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
	int byte_;//�ڼ����ֽڣ�0��ʼ
	int bit_;//�ڼ�λ���������ң�0��ʼ
	tuple_() {};
	tuple_(int a, int b) {
		byte_ = a;
		bit_ = b;
	}
};
//���ֽ�������Ϊ������
int byte2bit(tuple_ a);
//����������Ϊ�ֽ�����
tuple_ bit2byte(int a);

//ȡ�ڼ�λ
int getbit(BYTE a, int x);//�������ҵ�0-7λ
//�ڵڼ�λд��
void write(BYTE& a, int x, int num);
//����Ӧת��
void convert(BYTE a[], BYTE b[], int i, int conv[]);

//��ʼ�û�
void convIP(BYTE a[], BYTE b[]);//��a����IP�û����������b��
//��ʼ�û����û�IP^(-1)
void convIP_1(BYTE a[], BYTE b[]);//���ʼ�û�����a����IP�û����������b��
//��Կ����
void getkeys(BYTE key[8], BYTE key_final[16][6]);
//��չ�û�
void convertE(BYTE a[4], BYTE b[6]);
//48bit��򣬽���浽c��
void xor_(BYTE a[6], BYTE b[6], BYTE c[6]);

//S��
void convertS(BYTE a[6], BYTE b[4]);
void convertP(BYTE a[4], BYTE b[4]);

//�ּ���
void round(BYTE m[8], BYTE key_final[16][6], BYTE afterIP_1[8]);

