#pragma once
#include "function.h"


int tackle(message b);//�����յ��ı���

int buildconnectionSer();//����˽�������
void outfile(char* name, char content[10000][1024], int length, int& index);//���յ����ַ���ת�����ļ�
int recvfile(message a);//�����ļ�����������


//////////////////////////////////////////////////










int tackle(message b)//�����յ��ı���
{
	if (b.get_syn())
	{
		if (buildconnectionSer())
			cout << "��������" << endl;
		else cout << "���ӽ���ʧ��" << endl;
	}
	if (b.get_startfile())
	{
		recvfile(b);
	}
	return 1;
}






int buildconnectionSer()//����˽�������
{
	message a,b;
	a.set_ack(); a.set_syn();
	simplesend(a);
	clockstart = clock();
	int flag = 0;//�ط�����10���˳�
	while (1)
	{
		simplerecv(b);
		int check=1;
		if (b.get_exist())
		{
			check = b.checkchecksum();
			if (b.get_ack() && check)//b��������Ϣa��ack��syn
				break;
		}
		clockend = clock();
		if (flag == SENT_TIMES)
		{
			cout << "���ӽ���ʧ��" << endl;
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
			flag++;
			clockstart = clock();
			simplesend(a);//�ش�
		}
	}
	cout << "���ӳɹ�";
	return 1;
}


int recvfile(message a)
{
	
	
	int index = a.index;
	int length = a.filelength;
	char name[30];
	memset(name, 0, 30);
	for (int i = 0; a.msg[i]; i++)
		name[i] = a.msg[i];

	//char content[10000][1024];
	for (int i = 0; i <= index; i++)
	{
		message b, c;
		memset(content[i], 0, 1024);
		
		if (stopwaitrecv(b, c))
		{
			//strcpy(content[i], b.msg);
			if (i == index)
			{
				for (int j = 0; j < length; j++)
					content[i][j] = b.msg[j];
			}
			else
			{
				for (int j = 0; j < 1024; j++)
					content[i][j] = b.msg[j];
			}

		}
		else
		{
			cout << "����0" << endl;
			return 0;
		}
		if (i == index)
		{
			if(!b.get_endfile())
			{
				cout << "����1" << endl;
				return 0;
			}
		}
	}
	outfile(name, content, length, index);
	cout <<name<< " �ļ����ճɹ�" << endl;
	return 1;
}




void outfile(char* name, char content[10000][1024], int length, int& index)
{
	ofstream fout(name, ofstream::binary);
	for (int i = 0; i < index; i++)
	{
		for (int j = 0; j < FILE_PACKET_LENGTH; j++)
			fout << content[i][j];
	}
	for (int j = 0; j < length; j++)
		fout << content[index][j];
	fout.close();
}
