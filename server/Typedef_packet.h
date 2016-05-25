#pragma once
#include "common.h"

typedef enum {
	SS_NONE, 
	SS_LOGIN,				//로그인창
	SS_LOBBY,				//로비창
	SS_ROOM,
	SS_GAME,				//게임
}SS_TYPE;					//ServerState_Type
typedef enum { 
	LP_NONE,
	LP_CREATEACCOUNT_SUCC,
	LP_CREATEACCOUNT_FAIL,
	LP_LOGIN_SUCC,
	LP_LOGIN_FAIL
}LP_TYPE;					//LoginPacket_Type
typedef enum {
	BS_NONE,
	BS_GET_PLAYER,
	BS_GET_PLAYERS,
	BS_GET_ROOMS,
	BS_ROOM_CREATE,
	BS_ROOM_CREATE_FAIL,
	BS_ROOM_JOIN,
	BS_ROOM_JOIN_FAIL,
	BS_ROOM_EXIT,
	BS_CHANGE_INFO,
	BS_CHANGE_PLAYER,
	BS_ROOM_READY,
	BS_ROOM_START
}BS_TYPE;					//LobbyState_Type
typedef enum {
	GS_NONE				= 0,
	GS_PLAYER_INSERT	= 1,
	GS_PLAYER_DATA		= 2,
	GS_PLAYER_DELETE	= 3,
	GS_IDLE				= 10,
	GS_MOVE_UP			= 11,
	GS_MOVE_DOWN		= 12,
	GS_MOVE_LEFT		= 13,
	GS_MOVE_RIGHT		= 14,
	GS_MOVE_RELEAESE	= 15,
	GS_HAS_DAMAGE		= 20,
	GS_ATTACK1			= 110,
	GS_ATTACK2			= 120,
	GS_ATTACK3			= 130,
	GS_SKILL1			= 140,
	GS_SKILL2			= 150,
	GS_SKILL3			= 160
}GS_TYPE;					//GameState_Type

typedef enum {
	JOB_WALKER			= 0,
	JOB_GUNNER,
	JOB_PSYKER
}JOB_TYPE;

typedef enum {
	OT_NONE	= 0,
	OT_DESTROY = 1,
	OT_HPUP = 10,
	OT_BULLET = 11,
	OT_STRAY = 12,
	OT_MINE = 202
}OBJECT_TYPE;

typedef enum {
	RT_NONE = 0,
	RT_START = 1,			//겜 시작
	RT_GAMING = 2,			//겜중
	RT_END = 3				//겜 끝남. 결과
}ROOM_TYPE;

class CMyPacket
{
private:
	int		size;
	char*	data;
public:
	CMyPacket(int size)
	{
		this->size = size;
		this->data = new char[size+sizeof(int)];
		memcpy(this->data, &size, sizeof(int));
	}
	~CMyPacket()
	{
		if(data != NULL)
		{
			delete[] data;
			data = NULL;
		}
	}
	int		GetDataSize()		{ return size; }
	int		GetPacketSize()		{ return size+sizeof(int); }
	char*	GetDataAddr()		{ return data+sizeof(int); }
	char*	GetPacket()			{ return data; }
};