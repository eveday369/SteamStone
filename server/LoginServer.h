#pragma once

#include "common.h"
#include "Server.h"
#include "Login_Packet.pb.h"

class CServerMain;
class CLoginServer : public CServer
{
private:

public:
	CLoginServer(CServerMain* pParent = NULL);
	virtual ~CLoginServer();

	void				AddPacket(LOGIN_PACKET::Login_Packet_PacketType cmd, string szID, string szPS);

	void				CMD_CreateAccount(string szID, string szPS);
	void				CMD_Login(string szID, string szPS);

	virtual void		AnalyzePacket(string szBuffer);
	virtual void		SendPacket(string szBuffer);
};