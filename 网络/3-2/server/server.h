#pragma once
#include "function.h"


HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;
DWORD WINAPI filehandler(LPVOID lparam1);



int tackle(message b);//�����յ��ı���
int buildconnectionSer();//����˽�������
void outfile(char* name, char content[10000][1024], int length, int& index);//���յ����ַ���ת�����ļ�
int recvfile(message a);//�����ļ�����������
int byeser();//�Ĵλ���

//////////////////////////////////////////////////










int tackle(message b)//�����յ��ı���
{
	if (b.get_syn())
	{
		if (buildconnectionSer())
			cout << "��������" << endl;
		else cout << "���ӽ���ʧ��" << endl;
	}
	else if (b.get_startfile())
	{
		recvfile(b);
	}
	if (b.get_fin())
	{
		byeser();
	}
	return 1;
}


int buildconnectionSer()//����˽�������
{
	message a, b;
	a.set_ack();// a.set_syn();
	simplesend(a);
	cout << "���ӳɹ�"<<endl;
	return 1;
}


int recvfile(message a)
{
	message t;
	t.set_ack();
	simplesend(t);

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
			if (!b.get_endfile())
			{
				cout << "����1" << endl;
				return 0;
			}
		}
	}
	outfile(name, content, length, index);
	cout << name << " �ļ����ճɹ�" << endl;
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

int byeser()//���λ���,�Ѿ��յ��Դ�fin
{
	message a, b, c;
	a.set_ack();
	simplesend(a);
	cout << "���ӶϿ��ɹ�" << endl;
	return 1;


}

//������յ����ļ���Ϣ
DWORD WINAPI filehandler(LPVOID lparam)
{
	filepacket* pkt = (filepacket*)(LPVOID)lparam;
	memset(content[pkt->index], 0, 1024);
	for(int j=0;j<pkt->length;j++)
	{
		content[pkt->index][j] = pkt->a.msg[j];
	}
	return 0;
}