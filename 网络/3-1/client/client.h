#pragma once
#include "function.h"


int buildconnectionCli();//��������
//void readfile(char*& pBuffer,  int  &pSize);
int sendfile(char* name);//�����ļ�

void readfile(char* name, char content[10000][1024], int& length,  int  & index);
//void recvfile(char* name,char content[10000][1024], int length,  int  & index);
int byecli();//��Ϊֻ�ǵ����䣬д���λ��ּ���


int buildconnectionCli()//�ͻ��ˣ����ӷ���
{
	message a, b;
	a.set_syn();
	a.msgseq = sendseq++;
	if (stopwaitsend(a, b))return 1;
	else return 0;
}

int sendfile(char* name)//�����ļ�
{
	// int  pSize=0;
	//char* pBuffer=nullptr;
	cout << "��ʼ�����ļ�" << endl;
	int length = 0;
	 int index = 0;

	readfile( name,content,length,index);
	clock_t timestart = clock();
	message a;
	a.index = index;
	a.filelength = length;
	int t = strlen(name);
	for (int i = 0; i < t; i++)
		a.msg[i]=name[i];//��msg�Ѿ���ʼ��ȫ��Ϊ0
	a.set_startfile();
	message b;
	a.msgseq = sendseq++;
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
		temp.msgseq = sendseq++;
		if (stopwaitsend(temp,b)== 0)
		{
			cout << "�ļ�����ʧ��" << endl;
			return 0;
		}
	}
	clock_t timeend = clock();
	double endtime = (double)(timeend - timestart) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime << endl;		//sΪ��λ
	cout << "�����ʣ�" << (double)(index + 1) * sizeof(message) * 8/endtime /1024/1024<< "Mbps" << endl;
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



int byecli()//�Ĵλ���
{
	//��һ�λ��֣�����FIN
	message a,b;
	a.set_fin();//��һ��
	a.msgseq = sendseq++;
	if (stopwaitsend(a, b))return 1;
	else return 0;

	//if (stopwaitrecv(b, c))//�յ�����Ϣд��b�У��ڶ��κ͵�����
	//{
	//		message d;
	//		d.set_ack(b);
	//		simplesend(d);
	//		{
	//			cout << "�ɹ��Ͽ�����" << endl;
	//			return 1;
	//		}
	//}
	//else
	//{
	//	cout << "�Ͽ�����ʧ��" << endl;
	//	return 0;
	//}
}