#pragma once
#include "function.h"


int buildconnectionCli();//��������
//void readfile(char*& pBuffer,  int  &pSize);
int sendfile(char* name);//�����ļ�

void readfile(char* name, char content[10000][1024], int& length,  int  & index);
//void recvfile(char* name,char content[10000][1024], int length,  int  & index);


int buildconnectionCli()//�ͻ��ˣ����ӷ���
{
	message a, b;
	a.set_syn();
	simplesend(a);
	clockstart = clock();
	int flag = 0;//�ط�����10���˳�
	while (1)
	{
		simplerecv(b);
		Sleep(2000);
		int check=1;
		if (b.get_exist())
		{
			check = b.checkchecksum();
			if (b.get_ack()&&check)//b��������Ϣa��ack��syn
				break;
		}
		clockend = clock();
		if (flag == SENT_TIMES)
		{
			cout << "����ʧ��" << endl;
			return 0;
		}
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME||b.get_nak())
		{
			flag++;
			clockstart = clock();
			simplesend(a);//�ش�
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
	cout << "���ӳɹ�";
}

int sendfile(char* name)//�����ļ�
{
	// int  pSize=0;
	//char* pBuffer=nullptr;
	cout << "��ʼ�����ļ�" << endl;
	int length = 0;
	 int index = 0;

	readfile( name,content,length,index);
	message a;
	a.index = index;
	a.filelength = length;
	int t = strlen(name);
	for (int i = 0; i < t; i++)
		a.msg[i]=name[i];//��msg�Ѿ���ʼ��ȫ��Ϊ0
	a.set_startfile();
	message b;
	if (!stopwaitsend(a, b))//������Ϣa
	{
		cout << "�ļ�����ʧ��" << endl;
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
			cout << "�ļ�����ʧ��" << endl;
			return 0;
		}
	}
	cout << "�ļ����ͳɹ�" << endl;
	return 1;

	//recvfile(name,content,length,index);
	//readfile(pBuffer, pSize);
	//recvfile(pBuffer, pSize);
	//���Ҫɾ��
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
		cout << "�ļ���Ч" << endl;
		return;
	}
	char t = in.get();
	while (in)
	{

		content[index][length % 1024] = t;
		length++;
		if (length % 1024 == 0)//��һ��������
		{
			index++; length = 0;
		}
		t = in.get();
	}
	in.close();
}



