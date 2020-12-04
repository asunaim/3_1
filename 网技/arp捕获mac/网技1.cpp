#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "标头.h"
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中
//宏定义
#define PACAP_ERRBUF_SIZE 10
#define MAX_IP_NUM 10

pcap_if_t* alldevs;
char errbuf[PCAP_ERRBUF_SIZE];
char* pcap_src_if_string;
pcap_t* ahandle;//发送arp请求的网卡

pcap_if_t* net[10];
char ip[10][20];
char name[100] ;
//多线程
HANDLE hThread;
DWORD dwThreadId;

pcap_t* open(char* name);//打开网络接口
int compare(BYTE a[], BYTE b[]);

int main()
{
	//const char* 到char*的转换
	pcap_src_if_string = new char[strlen(PCAP_SRC_IF_STRING)];
	strcpy(pcap_src_if_string, PCAP_SRC_IF_STRING);

	//获取本机设备列表并打印
	find_alldevs();
	//strcpy(name, "rpcap://\Device\NPF_{7F205D8E-42A7-46FB-BBBC-C102D4E54C72}");
	//ahandle = open(name);
	int index;
	//选择ip地址
	printf("请选择IP序号: ");
	//scanf("%s", ips);
	scanf("%d", &index);
	//index = 4;

	//打开对应的网络接口卡
	


	ARPFrame_t ARPFrame;
	//将APRFrame.FrameHeader.DesMAC设置为广播地址
	for (int i = 0; i < 6; i++)
		ARPFrame.FrameHeader.DesMAC[i] = 0xff;
	//将APRFrame.FrameHeader.SrcMAC设置为本机网卡的MAC地址

	ARPFrame.FrameHeader.SrcMAC[0] = 0x0f;
	ARPFrame.FrameHeader.SrcMAC[1] = 0x0f;
	ARPFrame.FrameHeader.SrcMAC[2] = 0x0f;
	ARPFrame.FrameHeader.SrcMAC[3] = 0x0f;
	ARPFrame.FrameHeader.SrcMAC[4] = 0x0f;
	ARPFrame.FrameHeader.SrcMAC[5] = 0x0f;


	ARPFrame.FrameHeader.FrameType = htons(0x806);//帧类型为ARP
	ARPFrame.HardwareType = htons(0x0001);//硬件类型为以太网
	ARPFrame.ProtocolType = htons(0x0800);//协议类型为IP
	ARPFrame.HLen = 6;//硬件地址长度为6
	ARPFrame.PLen = 4;//协议地址长为4
	ARPFrame.Operation = htons(0x0001);//操作为ARP请求

	//将ARPFrame.SendHa设置为本机网卡的MAC地址
	/*for (int i = 0; i < 6; i++)
		ARPFrame.SendHa[i] = 0x66;*/
	ARPFrame.SendHa[0] = 0x0f;
	ARPFrame.SendHa[1] = 0x0f;
	ARPFrame.SendHa[2] = 0x0f;
	ARPFrame.SendHa[3] = 0x0f;
	ARPFrame.SendHa[4] = 0x0f;
	ARPFrame.SendHa[5] = 0x02;
	//将ARPFrame.SendIP设置为本机网卡上绑定的IP地址
	ARPFrame.SendIP = inet_addr("122.122.122.122");
	//将ARPFrame.RecvHa设置为0
	for(int i=0;i<6;i++)
		ARPFrame.RecvHa[i] = 0;
	//将ARPFrame.RecvIP设置为请求的IP地址
	ARPFrame.RecvIP = inet_addr(ip[index]);

	u_char* h = (u_char*)&ARPFrame;
	int len = sizeof(ARPFrame_t);
	printf("%s\t%s\n", "IP地址:", ip[index]);
	printf("%s\n", net[index]->name);
	ahandle = open(net[index]->name);
	
	if (ahandle == nullptr) printf("网卡接口打开错误\n");
	else
	{if (pcap_sendpacket(ahandle, (u_char*)&ARPFrame, sizeof(ARPFrame_t)) != 0)
	{
		//发送错误处理
		printf("senderror\n");
	}
	else
	{
		//发送成功
		while (1)
		{
			//printf("send\n");
			pcap_pkthdr* pkt_header;
			const u_char* pkt_data;
			int rtn = pcap_next_ex(ahandle, &pkt_header, &pkt_data);
			//pcap_sendpacket(ahandle, (u_char*)&ARPFrame, sizeof(ARPFrame_t));
			if (rtn == 1)
			{
				ARPFrame_t* IPPacket = (ARPFrame_t*)pkt_data;
				if (ntohs(IPPacket->FrameHeader.FrameType) == 0x806)
					//printf("%s\t%s\n", d->name, d->description);
					//Data_t* IPPacket;
					//WORD RecvChecksum;
					if (ntohs(IPPacket->FrameHeader.FrameType) == 0x806)
					{//输出目的MAC地址
						//printf("目的MAC地址：");
						//for (int i = 0; i < 6; i++)
						//{
						//	printf("%02x.", IPPacket->FrameHeader.DesMAC[i]);
						//}
						if (!compare(IPPacket->FrameHeader.SrcMAC, ARPFrame.FrameHeader.SrcMAC))
						{
							printf(" MAC地址为:");
							//输出源MAC地址
							for (int i = 0; i < 6; i++)
							{
								printf("%02x.", IPPacket->FrameHeader.SrcMAC[i]);
							}
						}
						//printf(" 帧类型/长度：");
						////ntohs((u_short)IPPacket->FrameHeader.FrameType);
						//printf("%02x", ntohs(IPPacket->FrameHeader.FrameType));
						//printf("H\n");
					}
			}
		}
	}
	}
	int i = 0;

	return 0;
	
}
//函数定义
void find_alldevs()	//获取本机的设备列表
{
	int op = 0;

	if (pcap_findalldevs_ex(pcap_src_if_string, NULL, &alldevs, errbuf) == -1)
	{
		printf("%s", "error");
	}
	else
	{
		int i = 0;
		for (pcap_if_t* d = alldevs; d != nullptr; d = d->next)//显示接口列表
		{

			//printf("%s\t", d->name, d->description);
			//获取该网络接口设备的ip地址信息
			
			for (pcap_addr* a = d->addresses; a != nullptr; a = a->next)
			{
				
				//输出所应用的协议族
				//printf("%s\t%s\n", "协议族:#", ((struct sockaddr_in*)a->addr)->sin_family);
				if (((struct sockaddr_in*)a->addr)->sin_family == AF_INET && a->addr)
				{//打印ipd地址
					/*if (i==2)
					{
						ahandle = open(d->name);
						printf("%s\t%s\n", "IP地址:", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
					}*/
					i++;
					printf("%d ", i);
					printf("%s\t", d->name, d->description);
					printf("%s\t%s\n", "IP地址:", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
					net[i] =d;
					strcpy(ip[i], inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
					int s = 0;
				}
			}
			//printf("=============================\n\n\n");
		}
	}
	pcap_freealldevs(alldevs);
}
pcap_t* open(char* name)//打开网络接口
{
	pcap_t *temp=pcap_open(name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 100, NULL, errbuf);
	if (temp==NULL)
		printf("error");
	return temp;
}


int compare(BYTE a[6], BYTE b[6])
{
	int index = 1;
	for (int i = 0; i < 6; i++)
	{
		if (a[i] != b[i])
			index = 0;
	}
	return index;
}

//void get_info(char* name)//获取并打印所需信息
//{
//	pcap_t* p = pcap_open(name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 100, NULL, errbuf);
//	if (p == NULL)
//	{
//		printf("error");
//		return;
//	}
//	pcap_pkthdr* pkt_header;
//	const u_char* pkt_data;
//	int rtn = pcap_next_ex(p, &pkt_header, &pkt_data);
//	if (rtn == 1)
//	{
//		//printf("%s\t%s\n", d->name, d->description);
//		Data_t* IPPacket;
//		//WORD RecvChecksum;
//		IPPacket = (Data_t*)pkt_data;
//
//		//输出目的MAC地址
//		printf( "目的MAC地址：");
//		for (int i = 0; i < 6; i++)
//		{
//			printf("%02x", IPPacket->FrameHeader.DesMAC[i]);
//		}
//		printf( " 源MAC地址:");
//		//输出源MAC地址
//		for (int i = 0; i < 6; i++)
//		{
//			printf("%02x", IPPacket->FrameHeader.SrcMAC[i]);
//		}
//		printf( " 帧类型/长度：");
//		//ntohs((u_short)IPPacket->FrameHeader.FrameType);
//		printf("%02x", ntohs(IPPacket->FrameHeader.FrameType));
//		printf("H\n");
//	}
//}

//DWORD WINAPI handlerRequest(LPVOID lparam)
//{
//	char* name = (char*)lparam;
//	get_info(name);
//	return 0;
//}