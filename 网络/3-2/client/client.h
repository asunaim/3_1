#pragma once
#include "function.h"



HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;
DWORD WINAPI sendhandler(LPVOID lparam);
DWORD WINAPI recvhandler(LPVOID lparam);


message msgsend[SENDBUFFER];


int buildconnectionCli();//����
int sendfile(char* name);//�����ļ�

void readfile(char* name, char content[10000][1024], int& length, int& index);
int byecli();//��Ϊֻ�ǵ����䣬д���λ��ּ���


int buildconnectionCli()//�ͻ��ˣ����ӷ���
{
	message a;
	a.set_syn();
	a.msgseq = buffersize;
	message::copy(msgsend[buffersize++], a);
	return 1;
}

int sendfile(char* name)//�����ļ�
{
	//��ʼ�����ļ���Ϣ
	message a;
	a.set_startfile();
	int index = 0; int length = 0;
	readfile(name, content, length, index);
	a.index = index;
	a.filelength = length;
	strcpy(a.msg, name);
	a.msgseq = buffersize;
	message::copy(msgsend[buffersize++], a);
	if (index==0 && length == 0)
	{

		return 0;//�ļ�����ʧ��
	}
	//�ļ���������
	for (int i = 0; i < index; i++)
	{
		//message b;
		for (int j = 0; j < 1024; j++)
		{
			msgsend[buffersize].msg[j] = content[i][j];
		}
		msgsend[buffersize].msgseq = buffersize;
		buffersize++;
		//message::copy(msgsend[buffersize++], b,i);
	}
	for (int i = index; i <= index; i++)
	{
		message b;
		for (int j = 0; j < length; j++)
		{
			b.msg[j] = content[i][j];
		}
		b.msgseq = buffersize;
		b.set_endfile();
		message::copy(msgsend[buffersize++], b);
	}
	return 1;
}


void readfile(char* name, char content[10000][1024], int& length, int& index)
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

int byecli()//�Ĵλ���
{
	message a;
	a.set_fin();
	a.msgseq = buffersize;
	message::copy(msgsend[buffersize++], a);
	return 1;
}



DWORD WINAPI recvhandler(LPVOID lparam)
{
	int i = (int)(LPVOID)lparam;
	//cout << "����" << i << endl;
	clock_t finalovertime;
	while (base < buffersize)
	{
		if (sendnextseq > base + N||sendnextseq==base)//�����������Է���û�з���ack�����ߴ���Ϊ��
			Sleep(2);
		//cout << "��" << base << endl;
		message a;
		simplerecv(a);
		if (a.get_ack())//�յ�ȷ����Ϣ�������ȷ
		{
			//cout << "receive" << endl;
			clockstart = clock();//���ü�ʱ��
			if (a.ackseq == base)//�յ���ȷ����Ϣ���
			{
				base++;
				overtime = 0;
				//cout << "������������" << endl;
				//flag = 0;
			}
			else if (a.ackseq < base-1)//ǰ���ack����
			{
				//base = a.ackseq+1;//״̬�˻�
				//overtime = 1;
				//cout << "ǰ���ack����" << endl;
			}
			else if(a.ackseq == base - 1)
			{
				overtime = 1;//�ش������ѷ���δȷ�ϵķ���
				cout << "��Ҫ�ش�" << endl;
				base = a.ackseq+1;
				//flag++
			}
			else
			{
				overtime = 1;
				cout << "ʧ��" << endl;
			}
		}
		clockend = clock();
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)
		{
			//��ʱ������
			mutex mxt;
			mxt.lock();
			overtime = 1;
			mxt.unlock();
			cout << "��ʱ" << endl;
			//flag++;
		}
		//mxt.~mutex();
		finalovertime = clock();
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME*SENT_TIMES)
		{
			mutex mxt;
			mxt.lock();
			overtime = 2;
			mxt.unlock();
			cout << "�Է��ƺ�������" << endl;
			return 0;
		}
	}
	cout << "exitpoint2" << endl;
	return 1;
}



DWORD WINAPI sendhandler(LPVOID lparam)//���߳�
{
	while (base < buffersize)
	{
		if (sendnextseq == base + N)
			Sleep(20);
		//cout << "����base " << base << endl;
		int i = (int)(LPVOID)lparam;
		//cout << "����" << i << endl;
		if (!overtime)
			for (; sendnextseq < base + N && sendnextseq < buffersize; sendnextseq++)
			{
				if (!overtime)
				{
					simplesend(msgsend[sendnextseq]);
					//Sleep(10);
				}
				else { 
					break; 
				}
				//clock_t start = clock();
				//if (!overtime)//��ǰ�����ش���������������
				//{
				//	//������Ϣ
				//	simplesend(msgsend[sendnextseq]);
				//}
				//else {//ǰ��ĳ������ˣ���ת���ش�����
				//	//sendnextseq = base;
				//	break;
				//}
				//clock_t end = clock();
				//double endtime = (double)(end - start) / CLOCKS_PER_SEC;
				//cout << "time:" << endtime << endl;		//sΪ��λ
			}
		if (overtime==1)//��һ������Ҫ���������д˲���
		{
			Sleep(30);//�����ش�����
			for (int i = base; i < sendnextseq; i++)
			{
				//���·���
				if (!overtime)
				{
					simplesend(msgsend[i]); Sleep(10);
				}
				else
				{
					break;
				}
				
				//cout << "xxxxxxxxxxx" << endl;
			}
			overtime = 0;
		}
		if (overtime == 2)
			return 0;//����������
	}
	cout << "exitpoint1" << endl;
	return 0;
}


