#pragma once

#include <winsock2.h>

#define			WM_BINDERROR		WM_USER + 1001	 
#define			WM_LISTENERROR		WM_USER + 1002		 
#define			WM_STARTLISTEN		WM_USER + 1003    
#define			WM_SENDFILELIST		WM_USER + 1004   	 

#define			FILELIST		    1000				 
#define			FILEDATA			2000				 

#define			SENDSIZE			1024*16				 

 
typedef struct
{
	int  iCommand;
	long lFileOffset;
	long lFileLength;
	char sClientPath[128];
	char sServerPath[128];
}MSGREQUEST;

 
typedef struct
{
	long lFileLength;
	char sServerPath[128];
}MSGFILELIST;

 
typedef struct
{
	SOCKET sock;
	LPVOID ptr;
}PARAMRECV;



class CTCPServer
{
public:
	char m_sIP[50];
	CTCPServer();
	virtual ~CTCPServer();

 
	CTCPServer(LPVOID ptr);

	 
	void SetListenPort(int iPort);
	void CTCPServer::SetSrvIP(char *szIP);
 
	void ListenRequest(void);

 
	void StopListen(void);

 
	static DWORD WINAPI ThreadListen(LPVOID lpParam);

 
	static DWORD WINAPI ThreadRecv(LPVOID lpParam);

private:
 
	BOOL m_bEndListenThread;

 
	int m_iListenPort;

 
	CWnd* m_pWnd;
};
