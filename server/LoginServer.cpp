#include "stdafx.h"
#include "LoginServer.h"
#include "CServerMain.h"
#include "..\include\GameLoginWindow.h"

CLoginServer::CLoginServer(CServerMain* pParent) : CServer(pParent)
{
	m_nOptVal = FALSE;
}

CLoginServer::~CLoginServer()
{

}

void CLoginServer::AnalyzePacket(string szBuffer)
{
	LOGIN_PACKET::Login_Packet* pData = new LOGIN_PACKET::Login_Packet;
	bool rtn = pData->ParseFromString(szBuffer);
	if(!rtn)
	{
		printf("[ERR]Decoding");
		pData->Clear();
		delete pData;
		return;
	}
	switch(pData->cmd())
	{
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_NONE :						//아무것도 아님
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_CREATE_ACCOUNT :
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_LOGIN :
		break;
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_CREATE_ACCOUNT_SUCC :		//계정 생성 성공, 11번
		{
			GameLoginWindow* pLogin = (GameLoginWindow*)GetGameState();
			pLogin->PushData(LP_TYPE::LP_CREATEACCOUNT_SUCC, pData->id());
		}
		break;
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_CREATE_ACCOUNT_FAIL :		//계정 생성 실패, 12번
		{
			GameLoginWindow* pLogin = (GameLoginWindow*)GetGameState();
			pLogin->PushData(LP_TYPE::LP_CREATEACCOUNT_FAIL, pData->id());
		}
		break;
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_LOGIN_SUCC :				//로그인 성공, 21번
		{
			m_pParent->SetPKey(pData->pkey());
			m_pParent->SetName(pData->id());
			GameLoginWindow* pLogin = (GameLoginWindow*)GetGameState();
			pLogin->PushData(LP_TYPE::LP_LOGIN_SUCC, pData->id());
		}
		break;
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_LOGIN_FAIL :				//로그인 실패, 22번
		{
			GameLoginWindow* pLogin = (GameLoginWindow*)GetGameState();
			pLogin->PushData(LP_TYPE::LP_LOGIN_FAIL, pData->id());
		}
		break;
	}
	delete pData;
}

void CLoginServer::AddPacket(LOGIN_PACKET::Login_Packet_PacketType cmd, string szID, string szPS)
{
	LOGIN_PACKET::Login_Packet* pData = new LOGIN_PACKET::Login_Packet;
	std::string output;

	switch(cmd)
	{
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_NONE :
		{
			//printf("login NONE\n");
		}
		break;
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_CREATE_ACCOUNT :
		{
			//printf("login Create Account\n");
			pData->set_cmd(LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_CREATE_ACCOUNT);
		}
		break;
	case LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_LOGIN :
		{
			//printf("login\n");
			pData->set_cmd(LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_LOGIN);
		}
		break;
	}

	pData->set_id(szID);
	pData->set_pass(szPS);


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

void CLoginServer::SendPacket(string szBuffer)
{
	int bufSize = 0;
	char* outputBuffer = NULL;
	PACKET::PACKET* pPacket = new PACKET::PACKET;

	pPacket->set_cmd(PACKET::PACKET_PacketType::PACKET_PacketType_LOGIN);
	pPacket->set_pkey(0);
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
	//send(m_Socket.m_socket, outputBuffer, bufSize, 0);
	delete[] outputBuffer;
	outputBuffer = NULL;
	delete pPacket;
	pPacket = NULL;
}

void CLoginServer::CMD_CreateAccount(string szID, string szPS)
{
	AddPacket(LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_CREATE_ACCOUNT, szID, szPS);
}

void CLoginServer::CMD_Login(string szID, string szPS)
{
	AddPacket(LOGIN_PACKET::Login_Packet_PacketType::Login_Packet_PacketType_LOGIN, szID, szPS);
}