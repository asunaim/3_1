#pragma once
#include "function.h"



int tackle(message b);//�����յ��ı���

int buildconnectionSer();//����˽�������

//////////////////////////////////////////////////










int tackle(message b)//�����յ��ı���
{
	if (b.get_syn())
	{
		if (buildconnectionSer())
			cout << "��������" << endl;
		else cout << "���ӽ���ʧ��" << endl;
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
			//cout << "����ʧ��" << endl;
			simplesend(a);//�ش�
		}
	}
	cout << "���ӳɹ�";
	return 1;
}