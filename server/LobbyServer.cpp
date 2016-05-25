#include "stdafx.h"
#include "LobbyServer.h"
#include "CServerMain.h"
#include "..\include\GameLobbyWindow.h"
#include "..\include\GameRoomWindow.h"

CLobbyServer::CLobbyServer(CServerMain* pParent) : CServer(pParent)
{
	
	m_nOptVal = FALSE;
}

CLobbyServer::~CLobbyServer()
{
	
}

void CLobbyServer::AnalyzePacket(std::string szBuffer)
{
	LOBBY_PACKET::Lobby_Packet* pLobby_Packet = new LOBBY_PACKET::Lobby_Packet;
	pLobby_Packet->ParseFromString(szBuffer);

	switch(pLobby_Packet->cmd())
	{
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_NONE :
		{

		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_FIRST_CONNECT :
		{
			AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_FIRST_CONNECT);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYER :
		{
			printf("GetPlayer\n");
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYERS :
		{
			printf("GetPlayers\n");
			GameLobbyWindow* pLobby = (GameLobbyWindow*)GetGameState();

			LOBBY_PACKET::LOBBY_Players_Data* pPlayersData = new LOBBY_PACKET::LOBBY_Players_Data;
			pPlayersData->ParseFromString(pLobby_Packet->data());
			LOBBY_PACKET::LOBBY_Player_Data* pPlayerData = new LOBBY_PACKET::LOBBY_Player_Data;

			for(int i=0; i<pPlayersData->players_data().size(); i++)
			{
				pPlayerData->ParseFromString(pPlayersData->players_data(i));

				pPlayerData->Clear();
			}

			delete pPlayerData;
			pPlayerData = NULL;
			delete pPlayersData;
			pPlayersData = NULL;
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_ROOMS :
		{
			GameLobbyWindow* pLobby = (GameLobbyWindow*)GetGameState();
			
			LOBBY_PACKET::LOBBY_Room_Data* pRoomData = new LOBBY_PACKET::LOBBY_Room_Data;
			pRoomData->ParseFromString(pLobby_Packet->data());

			pLobby->PushData(pRoomData->roomnum(), pRoomData->nowplayer(), 
				pRoomData->maxplayer(), pRoomData->mapnum(), pRoomData->isgame(), 
				pRoomData->mastername(), pRoomData->name());
				
			delete pRoomData;
			pRoomData = NULL;
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_CREATE :
		{
			GameLobbyWindow* pLobby = (GameLobbyWindow*)GetGameState();
			pLobby->PushData(BS_TYPE::BS_ROOM_CREATE);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_CREATE_FAIL :
		{
			GameLobbyWindow* pLobby = (GameLobbyWindow*)GetGameState();
			pLobby->PushData(BS_TYPE::BS_ROOM_CREATE_FAIL);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_JOIN :
		{
			GameLobbyWindow* pLobby = (GameLobbyWindow*)GetGameState();
			pLobby->PushData(BS_TYPE::BS_ROOM_JOIN);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_JOIN_FAIL :
		{
			GameLobbyWindow* pLobby = (GameLobbyWindow*)GetGameState();
			pLobby->PushData(BS_TYPE::BS_ROOM_JOIN_FAIL);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_GET_FIRST_INFO :
		{
			GameRoomWindow* pRoom = (GameRoomWindow*)GetGameState();
			LOBBY_PACKET::Room_First_Data* pFirstData = new LOBBY_PACKET::Room_First_Data;
			pFirstData->ParseFromString(pLobby_Packet->data());

			LOBBY_PACKET::Room_Data* pRoomData = new LOBBY_PACKET::Room_Data;
			pRoomData->ParseFromString(pFirstData->roomdata());

			string szMasterName = pRoomData->mastername();
			string szRoomName = pRoomData->name();
			UINT nRoomNum = pRoomData->roomnum();
			UINT nNowPlayer = pRoomData->nowplayer();
			UINT nMaxPlayer = pRoomData->maxplayer();
			UINT nMapNum = pRoomData->mapnum();

			delete pRoomData;
			pRoomData = NULL;

			pRoom->PushData(szMasterName, szRoomName, nRoomNum, nNowPlayer, nMaxPlayer, nMapNum);

			LOBBY_PACKET::Room_Player_Data *pPlayerData = new LOBBY_PACKET::Room_Player_Data;
			for(int i=0; i<pFirstData->playersdata().size(); i++)
			{
				pPlayerData->ParseFromString(pFirstData->playersdata(i));
				int num = pPlayerData->num();
				string name = pPlayerData->name();
				int jobnum = pPlayerData->jobnum();
				bool ready = pPlayerData->ready();
				pRoom->PushData(num, name, jobnum, ready);
				pPlayerData->Clear();
			}
			delete pPlayerData;
			pPlayerData = NULL;
			delete pFirstData;
			pFirstData = NULL;
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_EXIT :
		{
			
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_CHANGE_INFO :
		{
			GameRoomWindow* pRoom = (GameRoomWindow*)GetGameState();
			LOBBY_PACKET::Room_Data* pRoomData = new LOBBY_PACKET::Room_Data;
			pRoomData->ParseFromString(pLobby_Packet->data());

			string szMasterName = pRoomData->mastername();
			string szRoomName = pRoomData->name();
			UINT nRoomNum = pRoomData->roomnum();
			UINT nNowPlayer = pRoomData->nowplayer();
			UINT nMaxPlayer = pRoomData->maxplayer();
			UINT nMapNum = pRoomData->mapnum();

			delete pRoomData;
			pRoomData = NULL;

			pRoom->PushData(szMasterName, szRoomName, nRoomNum, nNowPlayer, nMaxPlayer, nMapNum);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_CHANGE_PLAYER :
		{
			GameRoomWindow* pRoom = (GameRoomWindow*)GetGameState();
			LOBBY_PACKET::Room_Players_Data* pPlayersData = new LOBBY_PACKET::Room_Players_Data;
			pPlayersData->ParseFromString(pLobby_Packet->data());
			LOBBY_PACKET::Room_Player_Data* pPlayerData = new LOBBY_PACKET::Room_Player_Data;

			for(int i=0; i<pPlayersData->players_data().size(); i++)
			{
				pPlayerData->ParseFromString(pPlayersData->players_data(i));

				int num = pPlayerData->num();
				string name = pPlayerData->name();
				int jobnum = pPlayerData->jobnum();
				bool ready = pPlayerData->ready();
				pRoom->PushData(num, name, jobnum, ready);
				pPlayerData->Clear();
			}
			delete pPlayerData;
			pPlayerData = NULL;
			
			delete pPlayersData;
			pPlayersData = NULL;
			
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_READY :
		{
			GameRoomWindow* pRoom = (GameRoomWindow*)GetGameState();
			pRoom->PushData(BS_TYPE::BS_ROOM_READY);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_START :
		{
			GameRoomWindow* pRoom = (GameRoomWindow*)GetGameState();
			pRoom->PushData(BS_TYPE::BS_ROOM_START);
		}
		break;
	}
	delete pLobby_Packet;
	pLobby_Packet = NULL;
}

void CLobbyServer::AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType cmd, std::string data)
{
	LOBBY_PACKET::Lobby_Packet* pData = new LOBBY_PACKET::Lobby_Packet;
	string output, outputData;

	switch(cmd)
	{
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_NONE :
		{
			
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_FIRST_CONNECT :
		{
			printf("Lobby First Connect\n");
			
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_FIRST_CONNECT);
			LOBBY_PACKET::First_Connect* pFirst = new LOBBY_PACKET::First_Connect;
			pFirst->set_pkey(m_pParent->GetPkey());
			pFirst->set_name(m_pParent->GetName());
			pFirst->SerializeToString(&outputData);
			
			delete pFirst;
			pFirst = NULL;
			pData->set_data(outputData);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYER :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYER);
			pData->set_data(data);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYERS :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYERS);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_ROOMS :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_ROOMS);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_CREATE :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_CREATE);
			pData->set_data(data);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_JOIN :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_JOIN);
			pData->set_data(data);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_GET_FIRST_INFO :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_GET_FIRST_INFO);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_EXIT :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_EXIT);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_CHANGE_INFO :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_CHANGE_INFO);
			pData->set_data(data);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_CHANGE_PLAYER :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_CHANGE_PLAYER);
			pData->set_data(data);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_READY :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_READY);
		}
		break;
	case LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_START :
		{
			pData->set_cmd(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_START);
		}
		break;
	}

	bool rtn = pData->SerializeToString(&output);
	if(!rtn)
	{
		printf("[ERR]Encoding\n");
		delete pData;
		pData = NULL;
		return;
	}

	delete pData;
	pData = NULL;
	SendPacket(output);
}

void CLobbyServer::SendPacket(string szBuffer)
{
	int bufSize = 0;
	char* outputBuffer = NULL;
	PACKET::PACKET* pPacket = new PACKET::PACKET;

	pPacket->set_cmd(PACKET::PACKET_PacketType::PACKET_PacketType_LOBBY);
	pPacket->set_pkey(m_pParent->GetPkey());
	pPacket->set_data(szBuffer);
	bufSize = pPacket->ByteSize();

	outputBuffer = new char[bufSize];
	bool rtn = pPacket->SerializeToArray(outputBuffer, bufSize);
	if(!rtn)
	{
		printf("[ERR]Encoding\n");
		delete[] outputBuffer;
		outputBuffer = NULL;
		delete pPacket;
		pPacket = NULL;
		return;
	}

	m_pParent->SendPacket(&m_Socket, outputBuffer, bufSize);
	delete[] outputBuffer;
	outputBuffer = NULL;
	delete pPacket;
	pPacket = NULL;
}

void CLobbyServer::CMD_LOBBY_GetPlayerInfo(std::string szID)
{
	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYER, szID);
}

void CLobbyServer::CMD_LOBBY_GetPlayersInfo()
{
	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_PLAYERS, "");
}

void CLobbyServer::CMD_LOBBY_GetRoomsInfo()
{
	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_GET_ROOMS, "");
}

void CLobbyServer::CMD_LOBBY_CreateRoom(UINT mapNum, UINT maxPlayer, std::string name)
{
	std::string output;
	LOBBY_PACKET::LOBBY_Room_Data* pRoom = new LOBBY_PACKET::LOBBY_Room_Data;
	pRoom->set_mapnum(mapNum);
	pRoom->set_maxplayer(maxPlayer);
	pRoom->set_name(name);

	pRoom->SerializeToString(&output);

	delete pRoom;
	pRoom = NULL;

	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_CREATE, output);
}

void CLobbyServer::CMD_LOBBY_JoinRoom(UINT roomNum)
{
	string output;
	LOBBY_PACKET::LOBBY_Room_Data* pRoom = new LOBBY_PACKET::LOBBY_Room_Data;
	pRoom->set_roomnum(roomNum);

	pRoom->SerializeToString(&output);

	delete pRoom;
	pRoom = NULL;

	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_LOBBY_ROOM_JOIN, output);
}

void CLobbyServer::CMD_ROOM_GetFirstInfo()
{
	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_GET_FIRST_INFO, "");
}

void CLobbyServer::CMD_ROOM_EXIT()
{
	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_EXIT, "");
}

void CLobbyServer::CMD_ROOM_Ready()
{
	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_READY, "");
}

void CLobbyServer::CMD_ROOM_Start()
{
	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_START, "");
}

void CLobbyServer::CMD_ROOM_Change_Job(int n)
{
	string output;
	LOBBY_PACKET::Room_Player_Data* pPlayer = new LOBBY_PACKET::Room_Player_Data;
	pPlayer->set_jobnum(n);
	pPlayer->SerializeToString(&output);
	delete pPlayer;
	pPlayer = NULL;

	AddPacket(LOBBY_PACKET::Lobby_Packet_PacketType::Lobby_Packet_PacketType_ROOM_CHANGE_PLAYER, output);
}