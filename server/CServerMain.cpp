#include "stdafx.h"
#include "CServerMain.h"
#include "Server.h"
#include "LoginServer.h"
#include "LobbyServer.h"
#include "GameServer.h"
#include "ChatServer.h"

UINT WINAPI CallWorkerThread(LPVOID p)
{
	CServerMain* pServer = (CServerMain*)p;
	pServer->WorkerThread();
	return 0;
}

UINT WINAPI CallBufferThread(LPVOID p)
{
	CServerMain* pServer = (CServerMain*)p;
	pServer->BufferThread();
	return 0;
}

CServerMain::CServerMain()
{
	strcpy(m_szLoginIP, "");
	strcpy(m_szLobbyIP, "");
	strcpy(m_szChatIP, "");
	strcpy(m_szGameIP, "");
	
	m_usLoginPORT		= 0;
	m_usLobbyPORT		= 0;
	m_usChatPORT		= 0;
	m_usGamePORT		= 0;

	m_pNowServer		= NULL;
	m_pLoginServer		= NULL;
	m_pLobbyServer		= NULL;
	m_pChatServer		= NULL;
	m_pGameServer		= NULL;

	m_dwpKey			= 0;

	for(int i=0; i<MAX_WORKERTHREAD; i++)
		m_hWorkerThread[i]		= NULL;
	m_bWorkerRun		= TRUE;
	m_bBufferRun		= TRUE;

	m_hBufferThread		= NULL;
	m_hIOCP				= NULL;
	m_pBuffer			= NULL;
	m_pBuffer			= new CPacketBuffer;
	m_hEvent			= CreateEvent(NULL, TRUE, FALSE, NULL);
}

CServerMain::~CServerMain()
{
	WSACleanup();
	DisconnectLoginServer();
	DisconnectLobbyServer();
	DisconnectGameServer();
	//DisconnectChatServer();
	DestroyThread();
	if(m_pBuffer != NULL)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
}

BOOL CServerMain::CreateThread()
{
	if(!CreateWorkerThread())
		return FALSE;
	if(!CreateBufferThread())
		return FALSE;
	return TRUE;
}

BOOL CServerMain::CreateWorkerThread()
{
	UINT uiThreadID = 0;
	for(int i=0; i<MAX_WORKERTHREAD; i++)
	{
		m_hWorkerThread[i] = (HANDLE)_beginthreadex(NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &uiThreadID);
		if(m_hWorkerThread[i] == NULL)
		{
			return FALSE;		//[ERR]CreateRecvThread
		}
		ResumeThread(m_hWorkerThread[i]);
	}
	return TRUE;
}

BOOL CServerMain::CreateBufferThread()
{
	UINT uiThreadID = 0;
	m_hBufferThread = (HANDLE)_beginthreadex(NULL, 0, &CallBufferThread, this, CREATE_SUSPENDED, &uiThreadID);
	if(m_hBufferThread == NULL)
	{
		return FALSE;
	}
	ResumeThread(m_hBufferThread);
	return TRUE;
}

void CServerMain::DestroyThread()
{
	m_bWorkerRun = FALSE;
	for(int i=0; i<MAX_WORKERTHREAD; i++)
		PostQueuedCompletionStatus(m_hIOCP, 0, 0, NULL);
	for(int i=0; i<MAX_WORKERTHREAD; i++)
	{
		CloseHandle(m_hWorkerThread[i]);
		WaitForSingleObject(m_hWorkerThread[i], INFINITE);
	}
	m_bBufferRun = FALSE;
	CloseHandle(m_hEvent);
	CloseHandle(m_hBufferThread);
	WaitForSingleObject(m_hBufferThread, INFINITE);
}

void CServerMain::InitServerMain()
{
	char szPath[1024];
	char szPort_Login[64];
	char szPort_Lobby[64];
	char szPort_Game[64];
	char szPort_Chat[64];

	GetCurrentDirectory(1024, szPath);
	std::string strINIPath = std::string(szPath);
	strINIPath += "\\ServerConfig.ini";
	GetPrivateProfileString("LOGIN", "IP", "127.0.0.1", m_szLoginIP, 256, strINIPath.c_str());
	GetPrivateProfileString("LOGIN", "PORT", "55555", szPort_Login, 256, strINIPath.c_str());
	GetPrivateProfileString("LOBBY", "IP", "127.0.0.1", m_szLobbyIP, 256, strINIPath.c_str());
	GetPrivateProfileString("LOBBY", "PORT", "55556", szPort_Lobby, 256, strINIPath.c_str());
	GetPrivateProfileString("GAME", "IP", "127.0.0.1", m_szGameIP, 256, strINIPath.c_str());
	GetPrivateProfileString("GAME", "PORT", "55557", szPort_Game, 256, strINIPath.c_str());
	GetPrivateProfileString("Chat", "IP", "127.0.0.1", m_szChatIP, 256, strINIPath.c_str());
	GetPrivateProfileString("Chat", "PORT", "55558", szPort_Chat, 256, strINIPath.c_str());

	m_usLoginPORT = atoi(szPort_Login);
	m_usLobbyPORT = atoi(szPort_Lobby);
	m_usGamePORT = atoi(szPort_Game);
	m_usChatPORT = atoi(szPort_Chat);
	InitStartup();

	CreateThread();
}

BOOL CServerMain::InitStartup()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(nRet != 0)		
	{
		return FALSE;	//[ERR]WSAStartup
	}
	
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	return TRUE;
}

BOOL CServerMain::ConnectServer(CServer* pServer, char* szIP, unsigned short usPORT)
{
	if(pServer->GetSocket()->m_socket != INVALID_SOCKET)
		return TRUE;

	pServer->GetSocket()->m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(pServer->GetSocket()->m_socket == INVALID_SOCKET)	
	{
		return FALSE;	//[ERR]socket
	}

	setsockopt(pServer->GetSocket()->m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)pServer->GetOptVal(), sizeof(*pServer->GetOptVal()));

	SOCKADDR_IN	stServerAddr;
	ZeroMemory(&stServerAddr, sizeof(stServerAddr));
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(usPORT);
	stServerAddr.sin_addr.s_addr = inet_addr(szIP);

	if(connect(pServer->GetSocket()->m_socket, (SOCKADDR*)&stServerAddr, sizeof(stServerAddr)) == SOCKET_ERROR)
	{
		return FALSE;	//[ERR]connect
	}

	HANDLE hIOCP = CreateIoCompletionPort((HANDLE)pServer->GetSocket()->m_socket, m_hIOCP, (DWORD)pServer->GetSocket(), 0);

	if(hIOCP == NULL || m_hIOCP != hIOCP)
	{
		return FALSE;
	}

	return TRUE;
}

void CServerMain::DisconnectServer(SOCKETINFO* pSocket, bool bIsForce)
{
	struct linger stLinger = {0,0};

	if(bIsForce)
		stLinger.l_onoff = 1;

	shutdown(pSocket->m_socket, SD_BOTH);
	setsockopt(pSocket->m_socket, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	closesocket(pSocket->m_socket);
	pSocket->m_socket = INVALID_SOCKET;
}

void CServerMain::GoNextServer(SS_TYPE type)
{
	switch(type)
	{
	case SS_LOGIN :
		{
			m_pNowServer = ConnectLoginServer();
		}
		break;
	case SS_LOBBY :
	case SS_ROOM :
		{
			DisconnectLoginServer();
			DisconnectGameServer();
			m_pNowServer = ConnectLobbyServer();
			//ConnectChatServer();
		}
		break;
	case SS_GAME :
		{
			m_pNowServer = ConnectGameServer();
		}
	}
}

CServer* CServerMain::ConnectLoginServer()
{
	if(m_pLoginServer == NULL)
	{
		m_pLoginServer = new CLoginServer(this);
		if(!ConnectServer(m_pLoginServer, m_szLoginIP, m_usLoginPORT))
		{
			printf("로그인 서버 연결 불가\n");
			return NULL;
		}
		RecvPacket(m_pLoginServer->GetSocket());
	}
	return m_pLoginServer;
}

CServer* CServerMain::ConnectLobbyServer()
{
	if(m_pLobbyServer == NULL)
	{
		m_pLobbyServer = new CLobbyServer(this);
		if(!ConnectServer(m_pLobbyServer, m_szLobbyIP, m_usLobbyPORT))
		{
			printf("로비 서버 연결 불가\n");
			return NULL;
		}
		RecvPacket(m_pLobbyServer->GetSocket());
	}
	return m_pLobbyServer;
}

CServer* CServerMain::ConnectGameServer()
{
	if(m_pGameServer == NULL)
	{
		m_pGameServer = new CGameServer(this);
		if(!ConnectServer(m_pGameServer, m_szGameIP, m_usGamePORT))
		{
			printf("게임 서버 연결 불가\n");
			return NULL;
		}
		RecvPacket(m_pGameServer->GetSocket());
	}
	return m_pGameServer;
}

CServer* CServerMain::ConnectChatServer()
{
	if(m_pChatServer == NULL)
	{
		m_pChatServer = new CChatServer(this);
		if(!ConnectServer(m_pChatServer, m_szChatIP, m_usChatPORT))
		{
			printf("채팅 서버 연결 불가\n");
			return NULL;
		}
		RecvPacket(m_pChatServer->GetSocket());
	}
	return m_pChatServer;
}

void CServerMain::DisconnectNowServer()
{
	if(m_pNowServer != NULL)
	{
		m_pNowServer->DisconnectToServer();
		delete m_pNowServer;
		m_pNowServer = NULL;
	}
}

void CServerMain::DisconnectLoginServer()
{
	if(m_pLoginServer != NULL)
	{
		m_pLoginServer->DisconnectToServer();
		delete m_pLoginServer;
		m_pLoginServer = NULL;
	}
}

void CServerMain::DisconnectLobbyServer()
{
	if(m_pLobbyServer != NULL)
	{
		m_pLobbyServer->DisconnectToServer();
		delete m_pLobbyServer;
		m_pLobbyServer = NULL;
	}
}

void CServerMain::DisconnectGameServer()
{
	if(m_pGameServer != NULL)
	{
		m_pGameServer->DisconnectToServer();
		delete m_pGameServer;
		m_pGameServer = NULL;
	}
}

void CServerMain::DisconnectChatServer()
{
	if(m_pChatServer != NULL)
	{
		m_pChatServer->DisconnectToServer();
		delete m_pChatServer;
		m_pChatServer = NULL;
	}
}

bool CServerMain::RecvPacket(SOCKETINFO* pSockInfo)
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;

	pSockInfo->m_stRecvOverlappedEx.m_wsaBuf.len = BUFFER_SIZE;
	pSockInfo->m_stRecvOverlappedEx.m_wsaBuf.buf = (char*)pSockInfo->m_stRecvOverlappedEx.m_szBuf;
	pSockInfo->m_stRecvOverlappedEx.m_eOperation = OP_RECV;

	if(WSARecv(pSockInfo->m_socket, &(pSockInfo->m_stRecvOverlappedEx.m_wsaBuf), 1, &dwRecvNumBytes, &dwFlag, (LPWSAOVERLAPPED)&(pSockInfo->m_stRecvOverlappedEx), NULL) == SOCKET_ERROR
		&& (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("WSARecv\n");
		return false;
	}
	return true;
}

bool CServerMain::SendPacket(SOCKETINFO* pSockInfo, char* pPacket, int nSize)
{
	DWORD dwSendNumBytes = 0;
	memcpy(pSockInfo->m_stSendOverlappedEx.m_szBuf, pPacket, nSize);

	pSockInfo->m_stSendOverlappedEx.m_wsaBuf.len = nSize;
	pSockInfo->m_stSendOverlappedEx.m_wsaBuf.buf = (char*)pSockInfo->m_stSendOverlappedEx.m_szBuf;
	pSockInfo->m_stSendOverlappedEx.m_eOperation = OP_SEND;

	if(WSASend(pSockInfo->m_socket, &(pSockInfo->m_stSendOverlappedEx.m_wsaBuf), 1, &dwSendNumBytes, 0, (LPWSAOVERLAPPED)&(pSockInfo->m_stSendOverlappedEx), NULL) == SOCKET_ERROR 
		&& (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("WSASend\n");
		return false;
	}
	
	return true;
}

void CServerMain::WorkerThread()
{
	SOCKETINFO* pSocketInfo = NULL;
	BOOL bSuccess = TRUE;
	DWORD dwIoSize = 0;
	LPOVERLAPPED lpOverlapped = NULL;

	while(m_bWorkerRun)
	{
		bSuccess = GetQueuedCompletionStatus(m_hIOCP, &dwIoSize, (LPDWORD)&pSocketInfo, &lpOverlapped, INFINITE);
		if(bSuccess == FALSE && dwIoSize == 0)
		{
			DisconnectServer(pSocketInfo);
			continue;
		}
		if(bSuccess == TRUE && dwIoSize == 0 && lpOverlapped != NULL)
		{
			DisconnectServer(pSocketInfo);
			continue;
		}
		if(bSuccess == TRUE && dwIoSize == 0 && lpOverlapped == NULL)
		{
			m_bWorkerRun = false;
			continue;
		}
		if(lpOverlapped == NULL)
			continue;

		OVERLAPPEDEX* pOverlappedEx = (OVERLAPPEDEX*)lpOverlapped;

		if(pOverlappedEx->m_eOperation == OP_RECV)
		{
			pOverlappedEx->m_szBuf[dwIoSize] = NULL;
			//AnalyzePacket(pOverlappedEx->m_szBuf, dwIoSize);
			m_pBuffer->push_back(pOverlappedEx->m_szBuf, dwIoSize);
			SetEvent(m_hEvent);
			RecvPacket(pSocketInfo);
		}
		else if(pOverlappedEx->m_eOperation == OP_SEND)
		{
		}
		else
		{
			
		}
		lpOverlapped = NULL;
	}
}

void CServerMain::BufferThread()
{
	BYTE* data = new BYTE[1024];
	int size = 0;
	while(m_bBufferRun)
	{
		WaitForSingleObject(m_hEvent, INFINITE);
		while(m_pBuffer->pop(data, &size) == 0)
		{
			AnalyzePacket(data, size);
		}
		ResetEvent(m_hEvent);
	}
	delete[] data;
	data = NULL;
}

void CServerMain::AnalyzePacket(BYTE* pPacket, int nSize)
{
	int bufSize = nSize;

	PACKET::PACKET*	pBuffer = new PACKET::PACKET;
	//pBuffer->ParseFromArray(pPacket, bufSize);
	bool bSucc = pBuffer->ParseFromArray(pPacket, nSize);

	if(bSucc == false)
		return;

	PACKET::PACKET_PacketType cmd = pBuffer->cmd();
	switch(cmd)
	{
	case PACKET::PACKET_PacketType::PACKET_PacketType_NONE :
		{
			printf("NONE\n");
		}
		break;
	case PACKET::PACKET_PacketType::PACKET_PacketType_LOGIN :
		{
			if(m_pLoginServer != NULL)
			{
				m_pLoginServer->AnalyzePacket(pBuffer->data());
			}
		}
		break;
	case PACKET::PACKET_PacketType::PACKET_PacketType_LOBBY :
		{
			if(m_pLobbyServer != NULL)
			{
				m_pLobbyServer->AnalyzePacket(pBuffer->data());
			}
		}
		break;
	case PACKET::PACKET_PacketType::PACKET_PacketType_GAME :
		{
			if(m_pGameServer != NULL)
			{
				m_pGameServer->AnalyzePacket(pBuffer->data());
			}
		}
		break;
	case PACKET::PACKET_PacketType::PACKET_PacketType_CHAT :
		{
			if(m_pChatServer != NULL)
			{
				m_pChatServer->AnalyzePacket(pBuffer->data());
			}
		}
		break;
	default :
		{
			printf("What the...\n");
		}
		break;
	}

	pBuffer->Clear();
	delete pBuffer;
}