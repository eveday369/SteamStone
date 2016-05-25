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

	//�κ�
	void				CMD_LOBBY_GetPlayerInfo(std::string szID);	//�÷��̾� ���� ��û �Ⱦ���
	void				CMD_LOBBY_GetPlayersInfo();					//�÷��̾�� ���� ��û. �Ƹ� �Ⱦ���
	void				CMD_LOBBY_GetRoomsInfo();					//��� ���� ��û.
	void				CMD_LOBBY_CreateRoom(UINT mapNum, UINT maxPlayer, std::string name);
	void				CMD_LOBBY_JoinRoom(UINT roomNum);

	//��
	void				CMD_ROOM_GetFirstInfo();			//�濡 ó�� ���������� �� ���� ��û.
	void				CMD_ROOM_EXIT();					//�濡 ������ ��
	void				CMD_ROOM_Ready();					//����?
	void				CMD_ROOM_Change_Job(int n);			//���� �ٲܶ�
	void				CMD_ROOM_Start();					//�� ����?

	virtual void		AnalyzePacket(string szBuffer);
	virtual void		SendPacket(string szBuffer);
};