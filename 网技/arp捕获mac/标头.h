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


#pragma pack()//恢复4bytes对齐





















//函数声明
void find_alldevs();	//获取本机的设备列表
//pcap_t* open(char* name);	//打开网络接口
//void get_info(char* name);//获取并打印所需信息



//线程函数
DWORD WINAPI handlerRequest(LPVOID lparam);