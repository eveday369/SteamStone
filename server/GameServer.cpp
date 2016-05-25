#include "stdafx.h"
#include "GameServer.h"
#include "CServerMain.h"
#include "..\include\GameRun.h"
#include "..\include\GameRunWindow.h"

CGameServer::CGameServer(CServerMain* pParent) : CServer(pParent)
{
	m_nOptVal = TRUE;
}

CGameServer::~CGameServer()
{
}

void CGameServer::SendPacket(string szBuffer, GAME_PACKET::Game_Packet_PacketType type)
{
	GAME_PACKET::Game_Packet* pData = new GAME_PACKET::Game_Packet;
	string output;
	pData->set_cmd(type);
	pData->set_data(szBuffer);

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

void CGameServer::SendPacket(std::string szBuffer)
{
	int bufSize = 0;
	char* outputBuffer = NULL;
	PACKET::PACKET* pPacket = new PACKET::PACKET;

	pPacket->set_cmd(PACKET::PACKET_PacketType::PACKET_PacketType_GAME);
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

void CGameServer::AnalyzePacket(string szBuffer)
{
	GAME_PACKET::Game_Packet* pGame_Packet = new GAME_PACKET::Game_Packet;
	pGame_Packet->ParseFromString(szBuffer);
	
	switch(pGame_Packet->cmd())
	{
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_NONE :
		{

		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_FIRST_CONNECT :
		{
			AddPacket(GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_FIRST_CONNECT);
		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_START :
		{

		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_RESULT :
		{

		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_EXIT :
		{
			CGameRun* pGameRun = (CGameRun*)GetGameState();
			if(pGameRun != NULL)
			{
				GAME_PACKET::Player_Data* pPlayer = new GAME_PACKET::Player_Data;
				pPlayer->ParseFromString(pGame_Packet->data());
				DWORD pKey = pPlayer->pkey();
				delete pPlayer;
				pPlayer = NULL;
				
				pGameRun->PushData(GS_TYPE::GS_PLAYER_DELETE, pKey, 0, true, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0.0f, 0.0f, 0.0f);
			}
		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_PLAYER_DATA :
		{
			ParsePlayerData(pGame_Packet->data());
		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_FIRST_DATA :
		{
			printf("First Data\n");
			ParsePlayerDataFirst(pGame_Packet->data());
		}
		break;
	}

	delete pGame_Packet;
	pGame_Packet = NULL;
}

void CGameServer::ParsePlayerDataFirst(string szBuffer)
{
	CGameRun* pGameRun = (CGameRun*)GetGameState();
	if(pGameRun == NULL)
		return ;

	GAME_PACKET::Players_First_Data *pPlayers = new GAME_PACKET::Players_First_Data;
	pPlayers->ParseFromString(szBuffer);

	GAME_PACKET::Player_First_Data* pPlayer = new GAME_PACKET::Player_First_Data;
	for(int i=0; i < pPlayers->players_data().size(); i++)
	{
		pPlayer->ParseFromString(pPlayers->players_data(i));

		string name = pPlayer->name();
		DWORD pKey = pPlayer->pkey();
		int jobNum = pPlayer->jobnum();
		float dX = pPlayer->dx();
		float dY = pPlayer->dy();
		float pX = pPlayer->px();
		float pY = pPlayer->py();
		int maxHP = pPlayer->maxhp();
		int maxMP = pPlayer->maxmp();
		float skill1Cool = pPlayer->skill1_cool();
		float skill2Cool = pPlayer->skill2_cool();
		float skill3Cool = pPlayer->skill3_cool();
		pPlayer->Clear();

		pGameRun->PushData(name, pKey, jobNum, dX, dY, pX, pY, maxHP, maxMP, skill1Cool, skill2Cool, skill3Cool);
	}
	delete pPlayer;
	pPlayer = NULL;
	delete pPlayers;
	pPlayers = NULL;
}

void CGameServer::ParsePlayerData(string szBuffer)
{
	CGameRun* pGameRun = (CGameRun*)GetGameState();
	GameRunWindow* pRunWindow;

	if(pGameRun == NULL)
		return;

	GAME_PACKET::Players_Data* pPlayersData = new GAME_PACKET::Players_Data;
	pPlayersData->ParseFromString(szBuffer);

	GAME_PACKET::Room_Data* pRoomData = new GAME_PACKET::Room_Data;
	pRoomData->ParseFromString(pPlayersData->room_data());
	GAME_PACKET::Room_Data_Room_Type roomType = pRoomData->roomtype();
	float fGameTime = pRoomData->gametime();
	DWORD dwWinnerPKey = 0;
	if(pRoomData->has_winner_pkey())
	{
		dwWinnerPKey = pRoomData->winner_pkey();
	}
	delete pRoomData;
	pRoomData = NULL;
	pGameRun->PushData((ROOM_TYPE)roomType, fGameTime, dwWinnerPKey);

	if(pPlayersData->objects_data().size() > 0)
	{
		GAME_PACKET::Object_Data *pObject = new GAME_PACKET::Object_Data;
		for(int i=0; i<pPlayersData->objects_data().size(); i++)
		{
			pObject->ParseFromString(pPlayersData->objects_data(i));
			GAME_PACKET::Object_Data::Object_Type objType = pObject->objtype();
			DWORD pKey = pObject->pkey();
			float dX = pObject->dx();
			float pX = pObject->px();
			float pY = pObject->py();
			DWORD dwTime = pObject->dwtime();

			pGameRun->PushData((OBJECT_TYPE)objType, pKey, dwTime, dX, pX, pY);
			pObject->Clear();
		}
		delete pObject;
		pObject = NULL;
	}
	if(pPlayersData->players_data().size() > 0)
	{
		GAME_PACKET::Player_Data* pPlayerData = new GAME_PACKET::Player_Data;
		for(int i=0; i<pPlayersData->players_data().size(); i++)
		{
			pPlayerData->ParseFromString(pPlayersData->players_data(i));

			GS_TYPE cmdType = GS_TYPE::GS_NONE;
			if(pPlayerData->cmdtype())
			{
				cmdType = (GS_TYPE)pPlayerData->cmdtype();
			}

			DWORD dwTime = pPlayerData->dwtime();
			DWORD pKey = pPlayerData->pkey();
			bool bIsGound = pPlayerData->isground();
			float dX = pPlayerData->dx();
			float dY = pPlayerData->dy();
			float pX = pPlayerData->px();
			float pY = pPlayerData->py();
			int HP = pPlayerData->hp();
			int MP = pPlayerData->mp();
			float skill1NowCool = pPlayerData->skill1_nowcool();
			float skill2NowCool = pPlayerData->skill2_nowcool();
			float skill3NowCool = pPlayerData->skill3_nowcool();

			pGameRun->PushData(cmdType, pKey, dwTime, bIsGound, dX, dY, pX, pY, HP, MP, skill1NowCool, skill2NowCool, skill3NowCool);
			pPlayerData->Clear();
		}
		delete pPlayerData;
		pPlayerData = NULL;
	}
	delete pPlayersData;
	pPlayersData = NULL;
}

void CGameServer::AddPacket(GAME_PACKET::Game_Packet_PacketType cmd)
{
	string output, outputData;
	GAME_PACKET::Game_Packet* pData = new GAME_PACKET::Game_Packet;

	switch(cmd)
	{
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_NONE :
		{

		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_FIRST_CONNECT :
		{
			pData->set_cmd(GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_FIRST_CONNECT);
			GAME_PACKET::First_Connect* pFirst = new GAME_PACKET::First_Connect;
			pFirst->set_pkey(m_pParent->GetPkey());
			pFirst->set_name(m_pParent->GetName());
			pFirst->SerializeToString(&outputData);

			delete pFirst;
			pFirst = NULL;
			pData->set_data(outputData);
		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_START :
		{

		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_RESULT :
		{

		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_EXIT :
		{

		}
		break;
	case GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_PLAYER_DATA :
		{
			
		}
		break;
	default :
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

void CGameServer::CMD_Ready()
{
	SendPacket("", GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_READY);
}

void CGameServer::CMD_Character(GS_TYPE type)
{
	GAME_PACKET::Player_Data* pData = new GAME_PACKET::Player_Data;
	
	switch(type)
	{
	case GS_TYPE::GS_NONE :
		{

		}
		break;
	case GS_TYPE::GS_MOVE_UP :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_MOVE_UP);
		}
		break;
	case GS_TYPE::GS_MOVE_DOWN :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_MOVE_DOWN);
		}
		break;
	case GS_TYPE::GS_MOVE_LEFT :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_MOVE_LEFT);
		}
		break;
	case GS_TYPE::GS_MOVE_RIGHT :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_MOVE_RIGHT);
		}
		break;
	case GS_TYPE::GS_MOVE_RELEAESE :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_MOVE_RELEASE);
		}
		break;
	case GS_TYPE::GS_ATTACK1 :
	case GS_TYPE::GS_ATTACK2 :
	case GS_TYPE::GS_ATTACK3 :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_ATTACK1);
		}
		break;
	case GS_TYPE::GS_SKILL1 :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_SKILL1);
		}
		break;
	case GS_TYPE::GS_SKILL2 :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_SKILL2);
		}
		break;
	case GS_TYPE::GS_SKILL3 :
		{
			pData->set_cmdtype(GAME_PACKET::Player_Data::Player_CommandType::Player_Data_Player_CommandType_CT_SKILL3);
		}
		break;
	default:
		{

		}
		break;
	}

	int bufSize = pData->ByteSize();
	std::string output;
	bool rtn = pData->SerializeToString(&output);
	if(!rtn)
	{
		printf("[ERR]Encoding(AddPacket)\n");
		pData->Clear();
		delete pData;
		return;
	}
	delete pData;
	pData = NULL;
	SendPacket(output, GAME_PACKET::Game_Packet_PacketType::Game_Packet_PacketType_GAME_PLAYER_DATA);
}