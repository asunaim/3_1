#define _CRT_SECURE_NO_WARNINGS

using namespace std;
#include "标头.h"
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中
//宏定义
#define PACAP_ERRBUF_SIZE 10;

pcap_if_t* alldevs;
char errbuf[PCAP_ERRBUF_SIZE];
char* pcap_src_if_string;


//多线程
HANDLE hThread;
DWORD dwThreadId;

int main()
{
	//printf("123");
	//const char* 到char*的转换
	pcap_src_if_string = new char[strlen(PCAP_SRC_IF_STRING)];
	strcpy(pcap_src_if_string, PCAP_SRC_IF_STRING);

	//获取本机设备列表并打印
	find_alldevs();


	//打开网络接口卡
	while (1)
	{for (pcap_if_t* d = alldevs; d != NULL; d = d->next)
	{
			hThread = CreateThread(NULL, NULL, handlerRequest, LPVOID(d->name), 0, &dwThreadId);
			WaitForSingleObject(hThread, 500);
			//get_info(d->name);
		}
	}
		//打开网络接口
	//pcap_t* pcap_open()

	pcap_freealldevs(alldevs);
	return 0;
	
}
//函数定义
void find_alldevs()	//获取本机的设备列表
{

	if (pcap_findalldevs_ex(pcap_src_if_string, NULL, &alldevs, errbuf) == -1)
	{
		printf("%s", "error");
	}
}
//pcap_t* open(char* name)//打开网络接口
//{
//	pcap_t *temp=pcap_open(name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 100, NULL, errbuf);
//	if (temp==NULL)
//		printf("error");
//	return temp;
//}

void get_info(char* name)//获取并打印所需信息
{
	pcap_t* p = pcap_open(name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 100, NULL, errbuf);
	if (p == NULL)
	{
		printf("error");
		return;
	}
	pcap_pkthdr* pkt_header;
	const u_char* pkt_data;
	int rtn = pcap_next_ex(p, &pkt_header, &pkt_data);
	if (rtn == 1)
	{
		//printf("%s\t%s\n", d->name, d->description);
		Data_t* IPPacket;
		//WORD RecvChecksum;
		IPPacket = (Data_t*)pkt_data;

		//输出目的MAC地址
		printf( "目的MAC地址：");
		for (int i = 0; i < 6; i++)
		{
			printf("%02x", IPPacket->FrameHeader.DesMAC[i]);
		}
		printf( " 源MAC地址:");
		//输出源MAC地址
		for (int i = 0; i < 6; i++)
		{
			printf("%02x", IPPacket->FrameHeader.SrcMAC[i]);
		}
		printf( " 帧类型/长度：");
		//ntohs((u_short)IPPacket->FrameHeader.FrameType);
		printf("%02x", ntohs(IPPacket->FrameHeader.FrameType));
		printf("H\n");
	}
}

DWORD WINAPI handlerRequest(LPVOID lparam)
{
	char* name = (char*)lparam;
	get_info(name);
	return 0;
}
