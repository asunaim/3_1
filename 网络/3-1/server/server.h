#pragma once
#include "function.h"


int tackle(message b);//�����յ��ı���

int buildconnectionSer(message t);//����˽�������
void outfile(char* name, char content[50000][1024], int length, int& index);//���յ����ַ���ת�����ļ�
int recvfile(message a);//�����ļ�����������
int byeser(message t);//�Ĵλ���

//////////////////////////////////////////////////



int tackle(message b)//�����յ��ı���
{
	if (b.get_syn())
	{
		if (buildconnectionSer(b))
		{
			status = 1;
			cout << "��������" << endl;
		}
		else cout << "���ӽ���ʧ��" << endl;
	}
	else if (b.get_startfile())
	{
		if (status)
		{
			cout << "��ʼ�����ļ�" << endl;
			recvfile(b);
		}
		else cout << "���Ƚ�������" << endl;
	}
	if (b.get_fin())
	{
		if (status)
		{
			if (byeser(b)) {
				status = 0;
				sendseq = 0;
			}
		}
		else cout << "δ����" << endl;
		cout << "�Է��Ѿ��Ͽ����ӣ��Ƿ��������0�˳���1���˳� ";
		int i;
		cin >> i;
		if (i == 0)
			return 0;
	}
	return 1;
}






int buildconnectionSer(message t)//����˽�������
{
	message a,b;
	a.set_ack(t);
	a.msgseq = sendseq++;
	simplesend(a);
	cout << "���ӳɹ�";
	return 1;
}


int recvfile(message a)
{
	message t;
	t.set_ack(a);
	a.msgseq = sendseq++;
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




void outfile(char* name, char content[50000][1024], int length, int& index)
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

int byeser(message t)//���λ���,�Ѿ��յ��Դ�fin
{
	message a;
	a.set_ack(t);
	a.msgseq = sendseq++;
	simplesend(a);
	return 1;
}
