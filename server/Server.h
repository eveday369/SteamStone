#pragma once

#include "common.h"
#include "Typedef_packet.h"
#include "packet.pb.h"

#define PACKET_SIZE		1024

class CGameState;
class CServerMain;
class CServer
{
private:
	
protected:
	SOCKETINFO			m_Socket;
	CServerMain*		m_pParent;
	CGameState*			m_pGameState;
	CGameState*			m_pGameUI;
	int					m_nOptVal;
public:
	CServer(CServerMain* pParent = NULL);
	virtual ~CServer();

	void				DisconnectToServer(bool bIsForce = 0);

	virtual void		AnalyzePacket(string szBuffer) = 0;
	virtual void		SendPacket(string szBuffer) = 0;

	inline SOCKETINFO*		GetSocket()						{ return &m_Socket; }
	inline CGameState*		GetGameState()					{ return m_pGameState; }
	inline void				SetGameState(CGameState* p)		{ m_pGameState = p; }
	inline CGameState*		GetGameUI()						{ return m_pGameUI; }
	inline void				SetGameUI(CGameState* p)		{ m_pGameUI = p; }
	inline int*				GetOptVal()						{ return &m_nOptVal; }
};