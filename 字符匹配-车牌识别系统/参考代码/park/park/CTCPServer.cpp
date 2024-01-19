#include "stdafx.h"
#include "CTCPServer.h"


CTCPServer::CTCPServer()
{

}

CTCPServer::~CTCPServer()
{

}
 
CTCPServer::CTCPServer(LPVOID ptr)
{
	m_iListenPort = 7000;
	m_bEndListenThread = TRUE; 
	m_pWnd = (CWnd*)ptr;
}
 
void CTCPServer::SetListenPort(int iPort)
{
	m_iListenPort = iPort;
}

void CTCPServer::SetSrvIP(char *szIP)
{
	strcpy(m_sIP, szIP);
}
 
void CTCPServer::ListenRequest()
{
	m_bEndListenThread = FALSE; 

	DWORD id;
	HANDLE h = CreateThread(NULL, 0, ThreadListen, this, 0, &id); 
	CloseHandle(h);
}

 
void CTCPServer::StopListen()
{
	m_bEndListenThread = TRUE;
}

 
DWORD WINAPI CTCPServer::ThreadListen(LPVOID lpParam)
{
	CTCPServer* pServer = (CTCPServer*)lpParam;

	SOCKET sockListen = socket(AF_INET, SOCK_STREAM, 0); 
	 

	SOCKADDR_IN sin;  
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(pServer->m_sIP); ;
	sin.sin_port = htons(pServer->m_iListenPort);
	if (bind(sockListen, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		closesocket(sockListen); 
		pServer->m_pWnd->SendMessage(WM_BINDERROR); 
		return 1;
	}

	if (listen(sockListen, 5) == SOCKET_ERROR)
	{ 
		closesocket(sockListen);
		pServer->m_pWnd->SendMessage(WM_LISTENERROR);
		return 1;
	}
 
	pServer->m_pWnd->SendMessage(WM_STARTLISTEN);

	fd_set fdListen; 
	timeval seltime;
	seltime.tv_sec = 0;  
	seltime.tv_usec = 10000; 

	while (!pServer->m_bEndListenThread)
	{ 
		FD_ZERO(&fdListen); 
		FD_SET(sockListen, &fdListen); 

		if (select(0, &fdListen, NULL, NULL, &seltime) <= 0 || !FD_ISSET(sockListen, &fdListen))
			continue;
		 

		int len = sizeof(sin);
		SOCKET sock = accept(sockListen, (SOCKADDR*)&sin, &len);
	 

		PARAMRECV* pParamRecv = new PARAMRECV; 
		pParamRecv->sock = sock;
		pParamRecv->ptr = pServer;

		DWORD id;
		HANDLE h = CreateThread(NULL, 0, ThreadRecv, pParamRecv, 0, &id);
 
		CloseHandle(h);
	}
	closesocket(sockListen);
	return 0;
}


 
DWORD WINAPI CTCPServer::ThreadRecv(LPVOID lpParam)
{
	PARAMRECV* pParam = (PARAMRECV*)lpParam; 
	CTCPServer* pServer = (CTCPServer*)pParam->ptr; 

	fd_set fdRecv; 
	timeval seltime; 
	seltime.tv_sec = 0;
	seltime.tv_usec = 10000;

	while (1)
	{
		FD_ZERO(&fdRecv); 
		FD_SET(pParam->sock, &fdRecv); 

		if (select(0, &fdRecv, NULL, NULL, &seltime) <= 0 || !FD_ISSET(pParam->sock, &fdRecv))
			continue;
 

		MSGREQUEST msgRequest; 
		int iRecvCnt = recv(pParam->sock, (char*)&msgRequest, sizeof(msgRequest), 0);
	 
		if (iRecvCnt <= 0) 
			break; 

	 
		if (msgRequest.iCommand == FILELIST)
		{
			pServer->m_pWnd->SendMessage(WM_SENDFILELIST, (WPARAM)&pParam->sock, 0);
		 
		}

	 
		else if (msgRequest.iCommand == FILEDATA)
		{
			long lFileOffset = msgRequest.lFileOffset; 

			CFile file;
			BOOL bResult = file.Open(msgRequest.sServerPath, CFile::modeRead | CFile::shareDenyNone, NULL);
		 
			if (!bResult)
			{
				break;		 
			}

			char sSendBuf[SENDSIZE]; 		
			while (lFileOffset < msgRequest.lFileLength)
			{ 
				int iSeek = file.Seek(lFileOffset, CFile::begin); 
				int iReadCnt = file.Read(sSendBuf, SENDSIZE); 
				if (iReadCnt == 0) 
					break;

				int iSendCnt = send(pParam->sock, sSendBuf, iReadCnt, 0);
	 
				if (iSendCnt == -1)
					break;
				else
					lFileOffset += iSendCnt; 
			}//end of while(lFileOffset < lFileLength)

			file.Close();
		}//end of else if(msgRequest.iCommand == FILEDATA)

		break;
	}//end of while(1)	
	closesocket(pParam->sock); 
	delete pParam; 
	return 0;
}



