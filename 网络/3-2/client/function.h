#pragma once
#include "class.h"



void simplesend(message& a);//Լ���ø�����Ϣ֮��ļ��׷��ͺ���
void simplerecv(message& a);

bool stopwaitsend(message& a, message b);//ͣ�Ȼ���
bool stopwaitrecv(message& a, message b);


void simplesend(message& a)
{
	/*clock_t start = clock();*/
	a.set_exist();
	a.setchecksum();
	//a.checkchecksum();
	if (sendto(sock, (char*)&a, sizeof(message), 0, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR);
	{
		/*cout << "����ʧ��" << endl;
		DWORD dwError = WSAGetLastError();
		cout << dwError << endl;*/
	}
	//if (a.flag) { cout << "���� "; a.print(); }
	//clock_t end = clock();
	//double endtime = (double)(end - start) / CLOCKS_PER_SEC;
	//cout << "time:" << endtime << endl;		//sΪ��λ
}

void simplerecv(message& a)
{
	memset(a.msg, 0, sizeof(a.msg));
	recvfrom(sock, (char*)&a, sizeof(message), 0, (struct sockaddr*)&addr, &addr_len);
	//if (a.flag) { cout << "���� "; a.print(); }
}


bool check(message a, message b)
{
	/*if (b.get_ack() == 1 && b.ackseq == a.msgseq)
		return 1;
	else return 0;*/
	return 1;
}


bool stopwaitsend(message& a, message b)//aд���������Ϣ������յ��Է����ص�ack��ɹ�
{
	simplesend(a);
	clockstart = clock();
	int flag = 0;//�ط�����10���˳�
	while (1)
	{
		simplerecv(b);
		if (b.get_ack() && b.checkchecksum())//b��������Ϣa��ack
		{
			cout << "����&ack" << endl;
			return 1;
		}
		clockend = clock();
		if (flag == SENT_TIMES)
			return 0;
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)//��ʱ
		{
			flag++;
			clockstart = clock();
			cout << "�ش�" << flag << endl;
			simplesend(a);//�ش�
		}
	}
	return 0;//0������ʧ��
}

bool stopwaitrecv(message& a, message b)//�յ�����Ϣд��a��
{
	int flag = 0;
	while (1)
	{
		simplerecv(a);
		if (a.get_exist())
		{
			int check = a.checkchecksum();
			if (check)
			{
				b.set_ack();
				simplesend(b);
				memset((char*)&b, 0, sizeof(message));
				cout << "����&ack" << endl;
				return 1;
			}
		}
	}
	cout << "����ʧ��" << endl;
	return 0;
}

void time()
{
	clock_t start, end;
	start = clock();		//����ʼ��ʱ
	int ans = 0;
	for (int i = 1; i <= 1e8; i++)
		ans++;
	end = clock();		//���������ʱ
	double endtime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime << endl;		//sΪ��λ
	cout << "Total time:" << endtime * 1000 << "ms" << endl;	//msΪ��λ
}
