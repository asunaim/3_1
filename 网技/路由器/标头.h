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


typedef struct IPHeader_t {//IP�ײ�
	BYTE Ver_HLen;
	BYTE TOS;
	WORD TotalLen;
	WORD ID;
	WORD Flag_Segment;
	BYTE TTL;
	BYTE Protocol;
	WORD Checksum;
	ULONG SrcIP;
	ULONG DstIP;
}IPHeader_t;
typedef struct Data_t {//����֡�ײ���IP�ײ������ݰ�
	FrameHeader_t FrameHeader;
	IPHeader_t IPHeader;
}Data_t;

#pragma pack()//�ָ�4bytes����





#pragma pack(1)//��1byte��ʽ����
class routeitem
{
public:
	DWORD mask;
	DWORD net;//Ŀ������
	DWORD nextip;//��һ��
	BYTE nextMAC[6];//��һ����MAC��ַ
	int index;//�ڼ���
	int type;//0Ϊֱ�����ӣ�1Ϊ�û���ӣ�1����ɾ��
	routeitem* nextitem;
	routeitem()
	{
		memset(this, 0, sizeof(*this));
	}
	void printitem();

};

#pragma pack()//�ָ�4bytes����

#pragma pack(1)//�ָ�4bytes����
class routetable
{
public:
	routeitem* head,*tail;//֧��������50ת����
	int num;//����
	routetable();//��ʼ�������ֱ�����ӵ�����

	//·�ɱ����ӣ�ֱ��Ͷ������ǰ��ǰ׺������ǰ��
	void add(routeitem* a);
	
	//ɾ����type=0����ɾ��
	void remove(int index);
	//·�ɱ�Ĵ�ӡ mask net next type
	void print();
	//���ң��ǰ׺,������һ����ip
	DWORD lookup(DWORD ip);

};
#pragma pack()//�ָ�4bytes����



class arpitem
{
public:
	DWORD ip;
	BYTE mac[6];
};


class ipitem
{
public:
	DWORD sip, dip;
	BYTE smac[6], dmac[6];
};
//��־��
class log
{
public:
	int index;//����
	char type[5];//arp��ip
	//��������
	ipitem ip; arpitem arp;


	static int num;//����
	static log diary[50];//��־
	//д����־
	static void write2log_ip(Data_t*);//ip����
	static void write2log_arp(ARPFrame_t*);//arp����
	static void print();//��ӡ��־
};


pcap_if_t* alldevs;
pcap_if_t* d;
pcap_t* ahandle;//open������
pcap_addr* a;//������Ӧ�ĵ�ַ
char errbuf[PCAP_ERRBUF_SIZE];
char* pcap_src_if_string;


pcap_if_t* net[10];
char ip[10][20];
char mask[10][20];
BYTE selfmac[6];
char name[100];
//���߳�
//HANDLE hThread;
//DWORD dwThreadId;


int compare(BYTE a[], BYTE b[]);//�Ƚ����������Ƿ���ͬ




//��ȡ�Լ���IP
void find_alldevs();	//��ȡ�������豸�б�������ip����ip������,��ȡIP��mask��������������
DWORD getnet(DWORD ip,DWORD mask);//����ip�����������������
//������ӿ�
pcap_t* open(char* name);
//��ȡ�Լ���MAC
void getselfmac(DWORD ip);
//��ȡֱ�����ӵ�����mac
void getothermac(DWORD ip, BYTE mac[]);
//��ʾ������Ϣ ����ip��mac
void printbasicinfo();

//�������ݱ�,д����־
int ipsend(u_char*);
//�������ݱ���д����־
int iprecv(pcap_pkthdr* pkt_header,const u_char* pkt_data);


//���ݱ�ת��,�޸�Դmac��Ŀ��mac
void resend(const u_char* pkt_data,BYTE dmac[]);

//��ӡmac
void getmac(BYTE MAC[]);






//��
//pcap_t* open(char* name);	//������ӿ�
//void get_info(char* name);//��ȡ����ӡ������Ϣ



//�̺߳���
DWORD WINAPI handlerRequest(LPVOID lparam);


