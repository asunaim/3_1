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


//typedef struct timeex
//{
//	BYTE type;
//	BYTE code;
//	WORD checksum;
//	BYTE version;
//	BYTE length;
//	BYTE DSF;
//	WORD totallength;
//	WORD id;
//	BYTE flag;
//	BYTE fragment;
//	BYTE ttl;
//	BYTE protocol;
//	WORD hchecksum;
//	ULONG srcip;
//	ULONG dstip;
//	BYTE type2;
//	BYTE code2;
//	WORD checksum;
//	BYTE IDBE;
//	BYTE IDLE;
//
//	timeex()
//	{
//		memset(this,0,sizeof(timeex));
//		type=11;
//		version=4;length=5;
//		totallength=92;
//		id=4198;
//		ttl=1;
//		
//	}
//};



typedef struct ICMP {//����֡�ײ���IP�ײ������ݰ�
	FrameHeader_t FrameHeader;
	IPHeader_t IPHeader;
	char buf[0x80];
}ICMP_t;







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
	routeitem* head, * tail;//֧��������50ת����
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


class arptable
{
public:
	DWORD ip;
	BYTE mac[6];
	static int num;
	static void insert(DWORD ip,BYTE mac[6]);
	static int lookup(DWORD ip, BYTE mac[6]);
}atable[50];




//��־��
class log
{
public:
	int index;//����
	char type[5];//arp��ip
	//��������
	ipitem ip; arpitem arp;

	log();
	~log();

	static int num;//����
	static log diary[50];//��־
	static FILE* fp;
	//д����־
	static void write2log_ip(Data_t*);//ip����
	static void write2log_arp(ARPFrame_t*);//arp����
	static void write2log_ip(const char* a,Data_t*);//ip����
	
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

BYTE broadcast[6] = { 0xff,0xff,0xff,0xff,0xff,0xff };

int compare(BYTE a[], BYTE b[]);//�Ƚ����������Ƿ���ͬ




//��ȡ�Լ���IP
void find_alldevs();	//��ȡ�������豸�б�������ip����ip������,��ȡIP��mask��������������
DWORD getnet(DWORD ip, DWORD mask);//����ip�����������������
//������ӿ�
pcap_t* open(char* name);
//��ȡ�Լ���MAC
void getselfmac(DWORD ip);
//��ȡֱ�����ӵ�����mac
void getothermac(DWORD mask_, DWORD ip, BYTE mac[]);
//��ʾ������Ϣ ����ip��mac
void printbasicinfo();


//�������ݱ���д����־
int iprecv(pcap_pkthdr* pkt_header, const u_char* pkt_data);


//���ݱ�ת��,�޸�Դmac��Ŀ��mac
void resend(ICMP_t, BYTE dmac[]);

//��ӡmac
void getmac(BYTE MAC[]);



//��
//pcap_t* open(char* name);	//������ӿ�
//void get_info(char* name);//��ȡ����ӡ������Ϣ

//�̺߳���
DWORD WINAPI handlerRequest(LPVOID lparam);

void ipprint(DWORD ip);


bool checkchecksum(Data_t*);
void setchecksum(Data_t*);