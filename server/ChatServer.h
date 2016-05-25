#pragma once

#include "common.h"
#include "Server.h"
#include "Chat_Packet.pb.h"

class CServerMain;
class CChatServer : public CServer
{
private:
	
public:
	CChatServer(CServerMain* pParent = NULL);
	virtual ~CChatServer();

	void				AddPacket(CHAT_PACKET::Chat_Packet_PacketType cmd, std::string data = "");

	virtual void		Create();
	virtual void		AnalyzePacket(string szBuffer);
	virtual void		SendPacket(string szBuffer);
};