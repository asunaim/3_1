#pragma once
#include "class.h"


void simplesend(message& a);//Լ���ø�����Ϣ֮��ļ��׷��ͺ���
bool simplerecv(message& a);

bool stopwaitsend(message a, message b);//ͣ�Ȼ���

void simplesend(message& a)
{
	a.set_exist();
	a.setchecksum();
	if (sendto(sock, (char*)&a, 1024, 0, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR);
	{
		/*cout << "����ʧ��" << endl;
		DWORD dwError = WSAGetLastError();
		cout << dwError << endl;*/
	}
}

bool simplerecv(message& a)
{
	memset(a.msg, 0, sizeof(a.msg));
	recvfrom(sock, (char*)&a, 1024, 0, (struct sockaddr*)&addr, &addr_len);
	
	return 0;
}

bool check(message a, message b)
{
	/*if (b.get_ack() == 1 && b.ackseq == a.msgseq)
		return 1;
	else return 0;*/
	return 1;
}


bool stopwaitsend(message& a, message& b)//aд���������Ϣ
{
	//simplesend(a);
	clockstart = clock();
	int flag = 0;//�ط�����10���˳�
	while (1)
	{
		simplerecv(b);
		if (b.msg[0] != 0 && check(a, b))//b��������Ϣa��ack
			return 1;
		clockend = clock();
		if (flag == SENT_TIMES)
			return 0;
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)
		{
			flag++;
			clockstart = clock();
			cout << "�ش�" << flag << endl;
			simplesend(a);//�ش�
		}
	}
	return 0;//0������ʱ��
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
