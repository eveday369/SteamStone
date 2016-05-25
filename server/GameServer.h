#pragma once

#include "common.h"
#include "Server.h"
#include "Game_Packet.pb.h"

class CGameServer : public CServer
{
private:

public:
	CGameServer(CServerMain* pParent = NULL);
	virtual ~CGameServer();

	void				AddPacket(GAME_PACKET::Game_Packet_PacketType cmd);
	void				ParsePlayerDataFirst(string szBuffer);
	void				ParsePlayerData(string szBuffer);
	
	void				SendPacket(string szBuffer, GAME_PACKET::Game_Packet_PacketType type);

	void				CMD_Ready();
	void				CMD_Character(GS_TYPE type);

	virtual void		AnalyzePacket(string szBuffer);
	virtual void		SendPacket(string szBuffer);
};