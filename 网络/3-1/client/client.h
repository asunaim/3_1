#pragma once
#include "function.h"


int buildconnectionCli();//��������



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

		if (b.get_ack())//b��������Ϣa��ack��syn
			break;
		clockend = clock();
		if (flag == SENT_TIMES)
			return 0;
		if ((clockend - clockstart) / CLOCKS_PER_SEC >= WAIT_TIME)
		{
			flag++;
			clockstart = clock();
			cout << "�ش�" << flag << endl;
			cout << "����ʧ��" << endl;
			simplesend(a);//�ش�
		}
	}
	message c;
	c.set_ack();
	simplesend(c);
	cout << "���ӳɹ�";
}