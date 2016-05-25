#pragma once

#include "common.h"
#include "Typedef_packet.h"
#include "PacketBuffer.h"
#include "packet.pb.h"

#define MAX_WORKERTHREAD	2

class CServer;
class CServerMain
{
private:
	char				m_szLoginIP[IP_LENTH];
	char				m_szLobbyIP[IP_LENTH];
	char				m_szChatIP[IP_LENTH];
	char				m_szGameIP[IP_LENTH];
	u_short				m_usLoginPORT;
	u_short				m_usLobbyPORT;
	u_short				m_usChatPORT;
	u_short				m_usGamePORT;

	CServer*			m_pNowServer;
	CServer*			m_pLoginServer;
	CServer*			m_pLobbyServer;
	CServer*			m_pChatServer;
	CServer*			m_pGameServer;
	
	unsigned long		m_dwpKey;
	std::string			m_szName;

	HANDLE				m_hWorkerThread[MAX_WORKERTHREAD];
	HANDLE				m_hBufferThread;
	HANDLE				m_hIOCP;
	HANDLE				m_hEvent;
	BOOL				m_bWorkerRun;
	BOOL				m_bBufferRun;

	CPacketBuffer*		m_pBuffer;

private:
	
public:
	CServerMain();
	~CServerMain();

	
	BOOL					CreateThread();
	BOOL					CreateWorkerThread();
	BOOL					CreateBufferThread();
	void					WorkerThread();
	void					BufferThread();
	void					DestroyThread();

	void					InitServerMain();
	BOOL					InitStartup();
	BOOL					ConnectServer(CServer* pServer, char* szIP, u_short usPORT);
	void					DisconnectServer(SOCKETINFO* pSocket, bool bIsForce = 0);

	CServer*				ConnectLoginServer();
	CServer*				ConnectLobbyServer();
	CServer*				ConnectGameServer();
	CServer*				ConnectChatServer();

	void					DisconnectNowServer();
	void					DisconnectLoginServer();
	void					DisconnectLobbyServer();
	void					DisconnectGameServer();
	void					DisconnectChatServer();

	bool					RecvPacket(SOCKETINFO* pSockInfo);
	bool					SendPacket(SOCKETINFO* pSockInfo, char* pPacket, int nSize);
	
	void					AnalyzePacket(BYTE* pPacket, int nSize);
	

	void					GoNextServer(SS_TYPE type);

	inline unsigned long	GetPkey()						{ return m_dwpKey; }
	inline void				SetPKey(unsigned long pKey)		{ m_dwpKey = pKey; }
	inline std::string		GetName()						{ return m_szName; }
	inline void				SetName(std::string name)		{ m_szName = name; }
	inline CServer*			GetNowServer()					{ return m_pNowServer; }
};