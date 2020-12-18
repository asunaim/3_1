#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "标头.h"
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中
#include <stdio.h>
//宏定义
#define PACAP_ERRBUF_SIZE 10
#define MAX_IP_NUM 10


log ltable;

int main()
{


	//const char* 到char*的转换
	pcap_src_if_string = new char[strlen(PCAP_SRC_IF_STRING)];
	strcpy(pcap_src_if_string, PCAP_SRC_IF_STRING);

	//获取本机ip
	find_alldevs();

	//int i = 0;
	//for (d=alldevs; d != NULL; d = d->next)
	//{
	//	for (a = d->addresses; a != nullptr; a = a->next)
	//	{
	//		if (((struct sockaddr_in*)a->addr)->sin_family == AF_INET && a->addr)
	//		{//打印ip地址

	//			printf("%d ", i);
	//			printf("%s\t", d->name, d->description);
	//			printf("%s\t%s\n", "IP地址:", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
	//			printf("%s\t%s\n", "MASK地址:", inet_ntoa(((struct sockaddr_in*)a->netmask)->sin_addr));

	//			net[i] = d;
	//			strcpy(ip[i], inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));

	//			//int s = 0;

	//		}
	//	}
	//	i++;
	//}
	//打开网卡
	ahandle = open(net[1]->name);

	
	strcpy(ip[0],"192.168.89.1");
	strcpy(ip[1],"192.168.43.135");
	strcpy(mask[0], "255.255.255.0");
	for (int i = 0; i < 2; i++)
		printf("%s\n", ip[i]);
	getselfmac(inet_addr(ip[0]));
	getmac(selfmac);
	BYTE mac[6];

	int op;


	
	routetable rtable;

	while (1)
	{
		printf("1添加路由表项，2删除路由表项，3打印路由表，4开始路由转发，5打印日志: ");
		scanf("%d", &op);
		if (op == 1)
		{
			routeitem a;
			char t[30];
			printf("请输入掩码：");
			scanf("%s", &t);
			a.mask = inet_addr(t);
			printf("请输入目的网络：");
			scanf("%s", &t);
			a.net = inet_addr(t);
			printf("请输入下一跳地址：");
			scanf("%s", &t);
			a.nextip = inet_addr(t);
			a.type = 1;
			rtable.add(&a);
		}
		else if (op == 2)
		{
			printf("请输入删除表项编号：");
			int index;
			scanf("%d", &index);
			rtable.remove(index);
		}
		else if (op == 3)
		{
			rtable.print();
		}
		else if (op == 4)
		{
			while (1)
			{
				pcap_pkthdr* pkt_header; const u_char* pkt_data;
				while (1)
				{
					int rtn = pcap_next_ex(ahandle, &pkt_header, &pkt_data);
					if (rtn)break;
				}
				FrameHeader_t* header = (FrameHeader_t*)pkt_data;
				if (compare(header->DesMAC, selfmac))//目的mac是自己的mac
				{
					if (header->FrameType == 0x806)//收到arp内容
					{

					}
					else if (header->FrameType == 0x800)
					{
						Data_t* data = (Data_t*)pkt_data;
						if (data->IPHeader.DstIP != inet_addr(ip[0]) &&data->IPHeader.DstIP != inet_addr(ip[1]))
						{
							BYTE mac[6];
							DWORD ip1 = data->IPHeader.DstIP;
							DWORD ip = rtable.lookup(ip1);
							getothermac(ip, mac);
							resend(pkt_data, mac);
						}
						else//发给自己的
						{

						}

					}
				}
			}
		}
		else {
			printf("无效操作，请重新选择\n");
		}

	}


	//getothermac(inet_addr("192.168.43.135"), mac);
	//getmac(mac);
	//getothermac(inet_addr("192.168.43.1"), mac);
	//getmac(mac);

	routetable table;
	table.print();
	
	return 0;
	
}
//函数定义
void find_alldevs()	//获取网卡上的IP
{

	if (pcap_findalldevs_ex(pcap_src_if_string, NULL, &alldevs, errbuf) == -1)
	{
		printf("%s", "error");
	}
	else
	{
		int i = 0;
		d = alldevs;
		//获取该网络接口设备的ip地址信息
		for(; d != NULL; d = d->next)
		for (a = d->addresses; a != nullptr; a = a->next)
		{
			if (((struct sockaddr_in*)a->addr)->sin_family == AF_INET && a->addr)
			{//打印ip地址
				
				printf("%d ", i);
				printf("%s\t", d->name, d->description);
				printf("%s\t%s\n", "IP地址:", inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
				net[i] =d;
				strcpy(ip[i], inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
				strcpy(mask[i], inet_ntoa(((struct sockaddr_in*)a->netmask)->sin_addr));

				//int s = 0;
				i++;
			}
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


void getselfmac(DWORD ip)
{
	memset(selfmac, 0, sizeof(selfmac));
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
	ARPFrame.SendHa[5] = 0x0f;
	//将ARPFrame.SendIP设置为本机网卡上绑定的IP地址
	ARPFrame.SendIP = inet_addr("122.122.122.122");
	//将ARPFrame.RecvHa设置为0
	for (int i = 0; i < 6; i++)
		ARPFrame.RecvHa[i] = 0;
	//将ARPFrame.RecvIP设置为请求的IP地址
	//ARPFrame.RecvIP = inet_addr(ip[index]);


	ARPFrame.RecvIP =ip;

	u_char* h = (u_char*)&ARPFrame;
	int len = sizeof(ARPFrame_t);
	

	if (ahandle == nullptr) printf("网卡接口打开错误\n");
	else
	{
		if (pcap_sendpacket(ahandle, (u_char*)&ARPFrame, sizeof(ARPFrame_t)) != 0)
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
					{//输出目的MAC地址
						if (!compare(IPPacket->FrameHeader.SrcMAC, ARPFrame.FrameHeader.SrcMAC))
						{
							ltable.write2log_arp(IPPacket);
							//输出源MAC地址
							for (int i = 0; i < 6; i++)
							{
								selfmac[i] = IPPacket->FrameHeader.SrcMAC[i];
								//if (flag == 0)
								//printf("%02x.", IPPacket->FrameHeader.SrcMAC[i]);
								//ARPFrame.FrameHeader.SrcMAC[i] = IPPacket->FrameHeader.SrcMAC[i];
								//ARPFrame.SendHa[i] = IPPacket->FrameHeader.SrcMAC[i];
							}
							break;

						}

					}
				}
			}
		}
	}
}
void getothermac(DWORD ip,BYTE mac[])//获取ip对应的mac
{
	memset(mac, 0, sizeof(mac));
	ARPFrame_t ARPFrame;
	//将APRFrame.FrameHeader.DesMAC设置为广播地址
	for (int i = 0; i < 6; i++)
		ARPFrame.FrameHeader.DesMAC[i] = 0xff;
	//将APRFrame.FrameHeader.SrcMAC设置为本机网卡的MAC地址
	for (int i = 0; i < 6; i++)
	{
		ARPFrame.FrameHeader.SrcMAC[i] = mac[i];
		ARPFrame.SendHa[i] = mac[i];

	}


	ARPFrame.FrameHeader.FrameType = htons(0x806);//帧类型为ARP
	ARPFrame.HardwareType = htons(0x0001);//硬件类型为以太网
	ARPFrame.ProtocolType = htons(0x0800);//协议类型为IP
	ARPFrame.HLen = 6;//硬件地址长度为6
	ARPFrame.PLen = 4;//协议地址长为4
	ARPFrame.Operation = htons(0x0001);//操作为ARP请求

	//将ARPFrame.SendHa设置为本机网卡的MAC地址
	/*for (int i = 0; i < 6; i++)
		ARPFrame.SendHa[i] = 0x66;*/

	//将ARPFrame.SendIP设置为本机网卡上绑定的IP地址
	ARPFrame.SendIP = ip;
	//将ARPFrame.RecvHa设置为0
	for (int i = 0; i < 6; i++)
		ARPFrame.RecvHa[i] = 0;
	//将ARPFrame.RecvIP设置为请求的IP地址
	//ARPFrame.RecvIP = inet_addr(ip[index]);


	ARPFrame.RecvIP = ip;

	u_char* h = (u_char*)&ARPFrame;
	int len = sizeof(ARPFrame_t);


	if (ahandle == nullptr) printf("网卡接口打开错误\n");
	else
	{
		if (pcap_sendpacket(ahandle, (u_char*)&ARPFrame, sizeof(ARPFrame_t)) != 0)
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
					{//输出目的MAC地址
						if (!compare(IPPacket->FrameHeader.SrcMAC, ARPFrame.FrameHeader.SrcMAC))
						{
							ltable.write2log_arp(IPPacket);
							//输出源MAC地址
							for (int i = 0; i < 6; i++)
							{
								mac[i] = IPPacket->FrameHeader.SrcMAC[i];
								//if (flag == 0)
								//printf("%02x.", IPPacket->FrameHeader.SrcMAC[i]);
								//ARPFrame.FrameHeader.SrcMAC[i] = IPPacket->FrameHeader.SrcMAC[i];
								//ARPFrame.SendHa[i] = IPPacket->FrameHeader.SrcMAC[i];
							}
							//getmac(mac);
							break;

						}

					}
				}
			}
		}
	}
}



void getmac(BYTE MAC[])//打印mac
{
	printf("MAC地址为： ");
	for (int i = 0; i < 5; i++)
		printf("%02X-", MAC[i]);
	printf("%02X\n", MAC[5]);
}


void routetable:: add(routeitem* a)
{
	routeitem* pointer;
	//找到合适的地方
	//默认路由,一定是最开始的时候添加
	if(!a->type)//直接投递
	{
		a->nextitem = head->nextitem;
		head->nextitem = a;
		a->type = 0;
	}

	//其它，按照掩码由长至短找到合适的位置
	else
	{
	/*	for (pointer = head->nextitem; pointer->mask == 0xffffffff || pointer->mask == 0; pointer = pointer->nextitem)
		{
			int t;
		}*/

		for (pointer = head->nextitem;pointer!=tail&& pointer->nextitem != tail; pointer = pointer->nextitem)//head有内容，tail没有
		{
			if (a->mask < pointer->mask && a->mask >= pointer->nextitem->mask|| pointer->nextitem==tail)
				break;
		}
		a->nextitem = pointer->nextitem;
		pointer->nextitem = a;//插入到合适位置
		//a->type = 1;
	}
	routeitem* p = head->nextitem;
	for (int i = 0; p != tail; p = p->nextitem,i++)
	{
		p->index = i;
	}
	num++;
}

void routeitem::printitem()
{
	//inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr)
	/*pcap_addr a;
	a.netmask = (sockaddr*)&mask;*/

	//index mask net nextip
	in_addr addr;
	printf("%d   ", index);
	addr.s_addr = mask;
	char* pchar = inet_ntoa(addr);
	printf("%s\t", pchar);

	addr.s_addr = net;
	pchar = inet_ntoa(addr);
	printf("%s\t", pchar);

	addr.s_addr = nextip;
	pchar = inet_ntoa(addr);
	printf("%s\t\t", pchar);

	printf("%d\n", type);
}


void routetable::print()
{
	routeitem* p = head->nextitem;
	for (; p != tail; p = p->nextitem)
	{
		p->printitem();
	}
}


routetable::routetable()//初始化，添加直接连接的网络
{
	head = new routeitem;
	tail = new routeitem;
	head->nextitem = tail;
	num = 0;
	for (int i = 0; i < 2; i++)
	{
		routeitem* temp=new routeitem;
		temp->net = (inet_addr(ip[i])) & (inet_addr(mask[i]));
		temp->mask = inet_addr(mask[i]);
		temp->type = 0;
		this->add(temp);
	}
}

void routetable::remove(int index)
{
	
	for (routeitem* t = head; t->nextitem != tail; t = t->nextitem)
	{
		if (t->nextitem->index == index)
		{
			if (t->nextitem->type == 0)
			{
				printf("该项不可删除\n");
				return;
			}
			else
			{
				t->nextitem = t->nextitem->nextitem;
				return;
			}
		}
	}
	printf("无该表项\n");
}




//接收数据报
int iprecv(pcap_pkthdr* pkt_header,const u_char* pkt_data)
{
	int rtn = pcap_next_ex(ahandle, &pkt_header, &pkt_data);
	return rtn;
}

int ipsend(u_char* pkt)//发送数据报
{
	int rtn = pcap_sendpacket(ahandle, pkt, sizeof(pkt));
	return rtn;
}


//数据报转发,修改源mac和目的mac
void resend(const u_char* pkt_data, BYTE dmac[])
{
	Data_t* data = (Data_t*)pkt_data;
	ltable.write2log_ip(data);
	memcpy(data->FrameHeader.SrcMAC, data->FrameHeader.DesMAC, 6);
	memcpy(data->FrameHeader.DesMAC, dmac, 6);
	int rtn = ipsend((u_char*)data);
	if (rtn == 1) ltable.write2log_ip(data);
}

//查找路由表对应表项,并给出下一跳的ip地址
DWORD routetable::lookup(DWORD ip)
{
	routeitem* t = head->nextitem;
	for (; t != tail; t = t->nextitem)
	{
		if ((t->mask & ip) == t->net)
			return t->nextip;
	}
	printf("未找到对应跳转地址");
	return -1;
}






int log::num = 0;
log log::diary[50] = {};
void log::print()//打印日志
{
	int i;
	if (num > 50)
		i = (num + 1) % 50;
	else i = 0;
	for (; i < num % 50; i++)
	{
		printf("%d ",diary[i].index);
		printf("%s\t ",diary[i].type);
		//printf("%s\n",diary[i].detail);
		if (!strcmp(diary[i].type, "ARP"))
		{
			in_addr addr;
			addr.s_addr = diary[i].arp.ip;
			char* pchar = inet_ntoa(addr);
			printf("%s\t", pchar);
			for (int i = 0; i < 5; i++)
			{
				printf("%02X.", diary[i].arp.mac[i]);
			}
			printf("%02X\n", diary[i].arp.mac[5]);
		}
		else if (!strcmp(diary[i].type, "IP"))
		{
			in_addr addr;
			addr.s_addr = diary[i].ip.sip;
			char* pchar = inet_ntoa(addr);
			printf("源IP：%s\t", pchar);
			addr.s_addr = diary[i].ip.dip;
			pchar = inet_ntoa(addr);
			printf("目的IP：%s\t", pchar);
			printf("源MAC: ");
			for (int i = 0; i < 5; i++)
			{
				printf("%02X.", diary[i].ip.smac[i]);
			}
			printf("%02X\t", diary[i].ip.smac[5]);
			printf("目的MAC: ");
			for (int i = 0; i < 5; i++)
			{
				printf("%02X.", diary[i].ip.dmac[i]);
			}
			printf("%02X\n", diary[i].ip.dmac[5]);
		}
	}
}
void log::write2log_ip(Data_t* pkt)//ip类型
{
	diary[num % 100].index = num++;
	strcpy(diary[num % 100].type, "IP");
	diary[num % 100].ip.sip = pkt->IPHeader.SrcIP;
	diary[num % 100].ip.dip = pkt->IPHeader.DstIP;

	//memcpy(copy,arr,len*sizeof(int)); 
	memcpy(diary[num % 100].ip.smac, pkt->FrameHeader.SrcMAC, 6);
	memcpy(diary[num % 100].ip.dmac, pkt->FrameHeader.DesMAC, 6);
}

void log::write2log_arp(ARPFrame_t* pkt)//arp类型
{
	diary[num % 100].index = num++;
	strcpy(diary[num % 100].type, "ARP");

	diary[num % 100].arp.ip = pkt->SendIP;
	memcpy(diary[num % 100].arp.mac, pkt->SendHa,6);
}