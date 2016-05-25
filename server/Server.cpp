#include "stdafx.h"
#include "Server.h"

CServer::CServer(CServerMain* pParent)
{
	m_pParent			= pParent;
	m_pGameState		= NULL;			
	m_pGameUI			= NULL;
	m_nOptVal			= FALSE;
}

CServer::~CServer()
{
	//DisconnectToServer();
}

void CServer::DisconnectToServer(bool bIsForce)
{
	struct linger stLinger = {0,0};

	if(bIsForce)
		stLinger.l_onoff = 1;

	shutdown(m_Socket.m_socket, SD_BOTH);
	setsockopt(m_Socket.m_socket, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	closesocket(m_Socket.m_socket);
	m_Socket.m_socket = INVALID_SOCKET;
}