#pragma once
#include "function.h"


HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;
DWORD WINAPI filehandler(LPVOID lparam1);

DWORD WINAPI messagehandler(LPVOID lparam);

message msgrecv[10000];

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
	/*filepacket* pkt = (filepacket*)(LPVOID)lparam;
	memset(content[pkt->index], 0, 1024);
	for(int j=0;j<pkt->length;j++)
	{
		content[pkt->index][j] = pkt->a.msg[j];
	}*/
	return 0;
}

DWORD WINAPI messagehandler(LPVOID lparam)//�����յ�����Ϣ
{
	message a = *(message*)(char*)(LPVOID)lparam;
	if (a.get_syn())
	{
		if (!buildconnectionSer())
			return 0;
		else status = 1;
	}
	else if (status && a.get_startfile())
	{
		//cout << "�����ļ�" << endl;
		memset(name, 0, sizeof(name));
		filestatus = 1;
		index = a.index;
		length = a.filelength;
		strcpy(name, a.msg);
	}
	else if (status && filestatus && a.get_endfile())//�ļ����ͽ���
	{
		//cout << "�����ļ�" << endl;
		filestatus = 0;

		if (fileseq != index)
		{
			cout << "����" << endl;
			return 0;
		}
		/*filepacket* packet = new filepacket;
		packet->a = a;
		packet->index = index;
		packet->length = length;
		hThread1 = ::CreateThread(NULL, NULL, filehandler, LPVOID(packet), 0, &dwThreadId1);*/
		for (int i = 0; i < length; i++)
		{
			content[index][i] = a.msg[i];
		}
		//WaitForSingleObject(hThread1, 1000);
		fileseq++;
		fileseq = 0;
		//

		cout << "�ļ����ս���" << endl;
	}
	else if (status && filestatus)
	{
		fileseq++;

		/*filepacket* packet = new filepacket;
		packet->a = a;
		packet->index = fileseq - 1;
		packet->length = 1024;
		hThread2 = ::CreateThread(NULL, NULL, filehandler, LPVOID(packet), 0, &dwThreadId2);*/
		for (int i = 0; i < 1024; i++)
		{
			content[fileseq-1][i] = a.msg[i];
		}
		//WaitForSingleObject(hThread2, 1000);
	}
	else 
	return 1;
}
