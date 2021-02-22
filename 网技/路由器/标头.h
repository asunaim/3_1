#pragma once
#include "pcap.h"


#pragma pack(1)//以1byte方式对齐
typedef struct FrameHeader_t {//帧首部
	BYTE DesMAC[6];//目的地址
	BYTE SrcMAC[6];//源地址
	WORD FrameType;//帧类型
}FrameHeader_t;
typedef struct ARPFrame_t {//IP首部
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


typedef struct IPHeader_t {//IP首部
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
typedef struct Data_t {//包含帧首部和IP首部的数据包
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



typedef struct ICMP {//包含帧首部和IP首部的数据包
	FrameHeader_t FrameHeader;
	IPHeader_t IPHeader;
	char buf[0x80];
}ICMP_t;







#pragma pack()//恢复4bytes对齐





#pragma pack(1)//以1byte方式对齐
class routeitem
{
public:
	DWORD mask;
	DWORD net;//目的网络
	DWORD nextip;//下一跳
	BYTE nextMAC[6];//下一跳的MAC地址
	int index;//第几条
	int type;//0为直接连接，1为用户添加，1不可删除
	routeitem* nextitem;
	routeitem()
	{
		memset(this, 0, sizeof(*this));
	}
	void printitem();

};

#pragma pack()//恢复4bytes对齐

#pragma pack(1)//恢复4bytes对齐
class routetable
{
public:
	routeitem* head, * tail;//支持最多添加50转发表
	int num;//条数
	routetable();//初始化，添加直接连接的网络

	//路由表的添加，直接投递在最前，前缀长的在前面
	void add(routeitem* a);

	//删除，type=0不能删除
	void remove(int index);
	//路由表的打印 mask net next type
	void print();
	//查找，最长前缀,返回下一跳的ip
	DWORD lookup(DWORD ip);

};
#pragma pack()//恢复4bytes对齐



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




//日志类
class log
{
public:
	int index;//索引
	char type[5];//arp和ip
	//具体内容
	ipitem ip; arpitem arp;

	log();
	~log();

	static int num;//数量
	static log diary[50];//日志
	static FILE* fp;
	//写入日志
	static void write2log_ip(Data_t*);//ip类型
	static void write2log_arp(ARPFrame_t*);//arp类型
	static void write2log_ip(const char* a,Data_t*);//ip类型
	
	static void print();//打印日志
};


pcap_if_t* alldevs;
pcap_if_t* d;
pcap_t* ahandle;//open的网卡
pcap_addr* a;//网卡对应的地址
char errbuf[PCAP_ERRBUF_SIZE];
char* pcap_src_if_string;


pcap_if_t* net[10];
char ip[10][20];
char mask[10][20];
BYTE selfmac[6];
char name[100];
//多线程
//HANDLE hThread;
//DWORD dwThreadId;

BYTE broadcast[6] = { 0xff,0xff,0xff,0xff,0xff,0xff };

int compare(BYTE a[], BYTE b[]);//比较两个数组是否相同




//获取自己的IP
void find_alldevs();	//获取本机的设备列表，将两个ip存入ip数组中,获取IP、mask，计算所在网段
DWORD getnet(DWORD ip, DWORD mask);//根据ip和掩码计算所在网络
//打开网络接口
pcap_t* open(char* name);
//获取自己的MAC
void getselfmac(DWORD ip);
//获取直接连接的网卡mac
void getothermac(DWORD mask_, DWORD ip, BYTE mac[]);
//显示基本信息 本机ip，mac
void printbasicinfo();


//接收数据报，写入日志
int iprecv(pcap_pkthdr* pkt_header, const u_char* pkt_data);


//数据报转发,修改源mac和目的mac
void resend(ICMP_t, BYTE dmac[]);

//打印mac
void getmac(BYTE MAC[]);



//函
//pcap_t* open(char* name);	//打开网络接口
//void get_info(char* name);//获取并打印所需信息

//线程函数
DWORD WINAPI handlerRequest(LPVOID lparam);

void ipprint(DWORD ip);


bool checkchecksum(Data_t*);
void setchecksum(Data_t*);