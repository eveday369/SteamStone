#pragma once
#include "BaseApplication.h"
#include "stdafx.h"

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIcolour.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreUTFString.h>											// 문자 UTF 변환 헤더

//#define				MAP_1		1001								// 맵 1
//#define				MAP_2		1002								// 맵2
//#define				CHARACTER_1	3000								// 캐릭터1
//#define				CHARACTER_2	3001								// 캐릭터2
//#define				CHARACTER_3	3002								// 캐릭터3

enum ROOM {
	ROOM_STATUS_NAME = 10,
	ROOM_STATUS_CHANGEFAILED = 11,
	ROOM_STATUS_NOREADY = 12,
	ROOM_MAP_MAP1 = 30,
	ROOM_MAP_MAP2 = 31,
	ROOM_TYPE_INDI = 51,
	ROOM_CHARACTER = 70,
	ROOM_CHARACTER_WALKER = 70,
	ROOM_CHARACTER_GUNNER = 71,
	ROOM_CHARACTER_PSYKER = 72,
};

class GameRoomWindow : public CGameState
{
private:
	typedef struct
	{
		BS_TYPE type;
	}stRoom;
	
	typedef struct
	{
		Ogre::UTFString		szMasterName;	//방장 이름
		Ogre::UTFString		szRoomName;		//방 이름
		int					nRoomNum;		//방 번호
		int					nRoomCurr;		//현재 방 인원
		int					nRoomMax;		//방 최대 인원
		int					nMapNum;		//맵 번호
	}stRoom_RoomInfo;

	typedef struct
	{
		int num;				//대기방 안에서 현재 위치하고 있는곳.
		std::string name;		//이 사람 이름
		int jobnum;				//직업 번호
		bool ready;				//준비상태
	}stRoom_PlayerInfo;

	typedef std::queue<stRoom> ROOM_QUEUE;
	typedef std::queue<stRoom_RoomInfo> ROOM_QUEUE_ROOMINFO;
	typedef std::queue<stRoom_PlayerInfo> ROOM_QUEUE_PLAYERINFO;

	ROOM_QUEUE					m_RoomQueue;
	ROOM_QUEUE_ROOMINFO			m_RoomQueue_RoomInfo;
	ROOM_QUEUE_PLAYERINFO		m_RoomQueue_PlayerInfo;

public:
	virtual bool				CheckData();
	void						PushData(BS_TYPE type);
	void						PushData(std::string szMasterName, std::string szRoomName, int nRoomNum, int nRoomCurr, int nRoomMax, int nMapNum);	//방 정보 갱신
	void						PushData(int nNum, std::string szName, int nJobNum, bool bReady);		//방안의 플레이어 정보 갱신할 때 쓰임.
	
public:
	GameRoomWindow(Game* pParent);
	virtual ~GameRoomWindow(void);

	void setVisible(bool visible);									// 윈도우를 표시하거나 감춘다
	bool isVisible();												// 윈도우 표시 상황

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg);
	void keyReleased(const OIS::KeyEvent &_arg);

private:
	void CreateCEGUIWindow();
	void RegisterHandlers();										// 핸들러 함수 등록
	bool Handle_ChaPrevButtonPressed(const CEGUI::EventArgs &e);	// 이전 캐릭터 버튼 이벤트
	bool Handle_ChaNextButtonPressed(const CEGUI::EventArgs &e);	// 다음 캐릭터 버튼 이벤트
	bool Handle_ReadyButtonPressed(const CEGUI::EventArgs &e);		// 준비 버튼 이벤트
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);		// 나가기 버튼 이벤트
	bool Handle_RoomChangeButtonPressed(const CEGUI::EventArgs &e); // 방 설정 변경 버튼 이벤트
	bool Handle_ConfirmButtonPressed(const CEGUI::EventArgs &e);	// 확인 클릭 이벤트
	bool Handle_CancelButtonPressed(const CEGUI::EventArgs &e);		// 취소 클릭 이벤트
	bool Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e);	// 메시지 박스 확인 버튼 이벤트
	bool Handle_SendButtonPressed(const CEGUI::EventArgs &e);		// 채팅 전송 버튼 이벤트
	bool Handle_TextSubmitted(const CEGUI::EventArgs &e);			// 채팅창 엔터 키 이벤트

	void ShowPlayerImage(int ch, bool ready);						// 캐릭터 이미지 표시
	void ShowEnemyImage(int ch, bool ready);						// 상대방 이미지 표시
	void SetMessage(ROOM s);										// 메시지 표시
	void SetExampleImage(int ch);									// 캐릭터 설명 표시

	void ParseText(CEGUI::String inMsg);							// 채팅창 파싱(특수키 입력 등)
	void OutputText(CEGUI::String inMsg,
		CEGUI::colour colour = CEGUI::colour( 0xFFFFFFFF));			// 텍스트 출력

	void ChangeRoomInfo(stRoom_RoomInfo* pRoomInfo);
	void ChangePlayerInfo(stRoom_PlayerInfo* pPlayerInfo);

	CEGUI::Window	*m_RoomWindow;									// 로비 윈도우
	CEGUI::String	sNamePrefix;									// 레이아웃의 이름
	static int		iInstanceNumber;								// 인스턴스 번호
	bool			m_bRoom;
	Ogre::UTFString	sMessage;										// 한글 메시지(UTF-8)
	Ogre::Entity		*mWalker;
	Ogre::Entity		*mGunner;
	Ogre::Entity		*mPsyker;
	Ogre::SceneNode		*mNode;

	int				mCharacter;										// 선택한 캐릭터
	bool			mReady;											// 준비 상태
	bool			mBangjang;										// 방장 여부
	bool			mType;											// 팀전 여부
	int				mGameType;										// 게임 타입
	int				mMap;											// 맵
	bool			mInput;											// 입력 상태
};