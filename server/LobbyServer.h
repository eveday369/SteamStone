#pragma once

#include "common.h"
#include "Server.h"
#include "Lobby_Packet.pb.h"

class CServerMain;
class CLobbyServer : public CServer
{
private:
	
public:
	CLobbyServer(CServerMain* pParent = NULL);
	virtual ~CLobbyServer();

	void				AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType cmd, std::string data = "");

	//로비
	void				CMD_LOBBY_GetPlayerInfo(std::string szID);	//플레이어 정보 요청 안쓸듯
	void				CMD_LOBBY_GetPlayersInfo();					//플레이어들 정보 요청. 아마 안쓸듯
	void				CMD_LOBBY_GetRoomsInfo();					//방들 정보 요청.
	void				CMD_LOBBY_CreateRoom(UINT mapNum, UINT maxPlayer, std::string name);
	void				CMD_LOBBY_JoinRoom(UINT roomNum);

	//방
	void				CMD_ROOM_GetFirstInfo();			//방에 처음 접속했을때 방 정보 요청.
	void				CMD_ROOM_EXIT();					//방에 나갔을 때
	void				CMD_ROOM_Ready();					//레디?
	void				CMD_ROOM_Change_Job(int n);			//직업 바꿀때
	void				CMD_ROOM_Start();					//겜 시작?

	virtual void		AnalyzePacket(string szBuffer);
	virtual void		SendPacket(string szBuffer);
};