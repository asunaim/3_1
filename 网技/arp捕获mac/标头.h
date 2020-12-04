#pragma once
#include "pcap.h"

#pragma pack(1)//��1byte��ʽ����
typedef struct FrameHeader_t {//֡�ײ�
	BYTE DesMAC[6];//Ŀ�ĵ�ַ
	BYTE SrcMAC[6];//Դ��ַ
	WORD FrameType;//֡����
}FrameHeader_t;
typedef struct ARPFrame_t {//IP�ײ�
	FrameHeader_t FrameHeader;
	WORD HardwareType;
	WORD ProtocolType;
	BYTE HLen;
	BYTE PLen;
	WORD Operation;
	BYTE SendHa[6];
	DWORD SendIP;
	BYTE RecvHa[6];
	DWORD RecvIP;
}ARPFrame_t;


#pragma pack()//�ָ�4bytes����





















//��������
void find_alldevs();	//��ȡ�������豸�б�
//pcap_t* open(char* name);	//������ӿ�
//void get_info(char* name);//��ȡ����ӡ������Ϣ



//�̺߳���
DWORD WINAPI handlerRequest(LPVOID lparam);