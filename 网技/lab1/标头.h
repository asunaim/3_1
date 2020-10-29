#pragma once
#include "pcap.h"


#pragma pack(1)//以1byte方式对齐
typedef struct FrameHeader_t {//帧首部
	BYTE DesMAC[6];//目的地址
	BYTE SrcMAC[6];//源地址
	WORD FrameType;//帧类型
}frameHeader_t;
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

#pragma pack()//恢复4bytes对齐


//函数声明
void find_alldevs();	//获取本机的设备列表
pcap_t* open(char* name);	//打开网络接口
void get_info(char* name);//获取并打印所需信息
