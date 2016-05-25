#include "stdafx.h"
#include "ChatServer.h"
#include "CServerMain.h"
#include "..\include\GameLobbyWindow.h"
#include "..\include\GameRoomWindow.h"

CChatServer::CChatServer(CServerMain* pParent) : CServer(pParent)
{
	m_nOptVal = FALSE;

}

CChatServer::~CChatServer()
{
	
}

void CChatServer::Create()
{
	
}

void CChatServer::AnalyzePacket(std::string szBuffer)
{
	CHAT_PACKET::Chat_Packet *pChatPacket = new CHAT_PACKET::Chat_Packet;
	pChatPacket->ParseFromString(szBuffer);

	CHAT_PACKET::Chat_Packet_PacketType cmd = pChatPacket->cmd();
	switch(cmd)
	{
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_NONE:
		{

		}
		break;
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_FIRST_CONNECT :
		{
			AddPacket(CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_FIRST_CONNECT);
		}
		break;
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_BROADCAST :
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_LOBBY :
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_ROOM :
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_GAME_ALL :
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_GAME_ALLY :
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_WHISPER :
		{

		}
		break;
	}

	delete pChatPacket;
	pChatPacket = NULL;
	/*CHAT_PACKET::Chat_Data* pChatData = new CHAT_PACKET::Chat_Data;
	pChatData->ParseFromString(szBuffer);

	string szSender = pChatData->name();
	string szMsg = pChatData->chat_msg();

	delete pChatData;
	pChatData = NULL;*/
}

void CChatServer::AddPacket(CHAT_PACKET::Chat_Packet_PacketType cmd, std::string data)
{
	CHAT_PACKET::Chat_Packet* pData = new CHAT_PACKET::Chat_Packet;
	string output, outputData;

	switch(cmd)
	{
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_NONE :
		{
			
		}
		break;
	case CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_FIRST_CONNECT :
		{
			printf("Chat First Connect\n");
			
			pData->set_cmd(CHAT_PACKET::Chat_Packet_PacketType::Chat_Packet_PacketType_FIRST_CONNECT);
			CHAT_PACKET::First_Connect* pFirst = new CHAT_PACKET::First_Connect;
			pFirst->set_pkey(m_pParent->GetPkey());
			pFirst->set_name(m_pParent->GetName());
			pFirst->SerializeToString(&outputData);;
			
			delete pFirst;
			pFirst = NULL;
			pData->set_data(outputData);
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

void CChatServer::SendPacket(string szBuffer)
{
	int bufSize = 0;
	char* outputBuffer = NULL;
	PACKET::PACKET* pPacket = new PACKET::PACKET;

	pPacket->set_cmd(PACKET::PACKET_PacketType::PACKET_PacketType_CHAT);
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