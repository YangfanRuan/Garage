#pragma once

#include <WinSock2.h>

#define			WM_RECVFILELIST		WM_USER + 1005		//接收文件列表
#define			WM_FILESTATUS		WM_USER + 1006		//反映文件下载状态
#define			WM_CONNECTERROR		WM_USER + 1007		//连接失败


#define			FILELIST		    1000				//请求文件列表
#define			FILEDATA			2000				//请求文件数据

#define			RECVSIZE			1024*16		    	//一次性发送/接收的数据块大小

//与服务器通信语言结构
typedef struct
{
	int  iCommand;
	long lFileOffset;
	long lFileLength;
	char sClientPath[128];
	char sServerPath[128];
}MSGREQUEST;

//请求线程参数
typedef struct
{
	char sIP[15];  //IP地址
	int  iPort;   //端口
	MSGREQUEST msgRequest; //通信语言结构
	LPVOID ptr;  //指针
}PARAMREQUEST;

//文件列表结构
typedef struct
{
	long lFileLength;
	char sServerPath[128];
}MSGFILELIST;

//反映文件状态的结构
typedef struct
{
	long lFileOffset;//偏移
	char sServerPath[128];//路径
}MSGFILESTATUS;

class CTCPClient
{
public:
	CTCPClient();
	virtual ~CTCPClient();

	//重载构造函数，添加指向窗口指针
	CTCPClient(LPVOID ptr);

	//设置服务器ip
	BOOL SetServerIP(char* sIP);

	//设置服务器端口
	void SetServerPort(int iPort);

	//创建请求线程
	void RequestFile(MSGREQUEST msgRequest);

	//发送请求并接收反馈信息的线程
	static DWORD WINAPI ThreadRequestFile(LPVOID lpParam);

	//停止所有接收线程
	static void StopAllRecvThread(void);

private:
	//服务器地址
	char m_sIP[15];

	//服务器端口
	int m_iServerPort;

	//指向窗口的指针
	CWnd* m_pWnd;

	//接收线程结束符
	static BOOL m_bEndRecvThread;
};
