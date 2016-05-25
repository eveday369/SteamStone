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

#define				MAP_1		1001								// 맵 1
#define				MAP_2		1002								// 맵2
#define				GAME_TEAM	2001								// 팀전
#define				GAME_INDI	2002								// 개인전

enum LOBBY_STATUS { SELECT, NONAME, CREATEFAILED, ENTRYFAILED, FULL };	// 로비 상태

class GameLobbyWindow : public CGameState
{
private:
	typedef struct
	{
		BS_TYPE type;
	}stLobby;

	typedef struct
	{
		int				mRoomNo;										// 방 번호
		Ogre::UTFString sRoomName;										// 방 이름
		int				mRoomMap;										// 맵
		int				mRoomType;										// 게임 유형
		int				mRoomRun;										// 게임 실행 여부
		int				mRoomCurr;										// 현재 방 인원
		int				mRoomMax;										// 최대 방 인원
	}room_list;

	typedef std::queue<stLobby>					LOBBY_QUEUE;
	LOBBY_QUEUE									m_LobbyQueue;
public:
	virtual bool	CheckData();
	void			PushData(int nRoomNum, int nNowCount, int nMaxCount, int nMapNum, bool bIsGame, std::string szMasterName, std::string szRoomName);	//방 정보 갱신
	void			PushData(BS_TYPE type);
public:
	GameLobbyWindow(Game* pParent);
	virtual ~GameLobbyWindow(void);

	void setVisible(bool visible);									// 윈도우를 표시하거나 감춘다
	bool isVisible();												// 윈도우 표시 상황

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg);
	void keyReleased(const OIS::KeyEvent &_arg){}

private:
	void CreateCEGUIWindow();
	void RegisterHandlers();
	bool Handle_CreateButtonPressed(const CEGUI::EventArgs &e);		// 방 만들기 버튼 이벤트
	bool Handle_EntryButtonPressed(const CEGUI::EventArgs &e);		// 입장하기 버튼 이벤트
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);		// 나가기 버튼 이벤트
	bool Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e);	// 메시지 박스 확인 버튼 이벤트
	bool Handle_Room0ButtonPressed(const CEGUI::EventArgs &e);		// 방0 버튼 클릭 이벤트
	bool Handle_Room1ButtonPressed(const CEGUI::EventArgs &e);		// 방1 버튼 클릭 이벤트
	bool Handle_Room2ButtonPressed(const CEGUI::EventArgs &e);		// 방2 버튼 클릭 이벤트
	bool Handle_PrevButtonPressed(const CEGUI::EventArgs &e);		// 이전 방 리스트 클릭 이벤트
	bool Handle_NextButtonPressed(const CEGUI::EventArgs &e);		// 다음 방 리스트 클릭 이벤트
	bool Handle_RefreshButtonPressed(const CEGUI::EventArgs &e);	// 새로고침 클릭 이벤트
	bool Handle_ConfirmButtonPressed(const CEGUI::EventArgs &e);	// 확인 버튼 클릭 이벤트
	bool Handle_CancelButtonPressed(const CEGUI::EventArgs &e);		// 취소 버튼 클릭 이벤트
	bool Handle_SendButtonPressed(const CEGUI::EventArgs &e);		// 채팅 전송 버튼 이벤트
	bool Handle_TextSubmitted(const CEGUI::EventArgs &e);			// 채팅창 엔터 키 이벤트
	
	void SetMessage(LOBBY_STATUS);									// 메시지 변경
	void Entry(int select);											// 방 입장 처리
	void Refresh();													// 방 목록을 보내라고 서버에 보냄.
	void Refresh_End(int i, room_list* roomList);					// 방 목록 새로고침
	Ogre::UTFString getMap(int map);								// 맵 이름 가져오기
	Ogre::UTFString getType(int type);								// 게임 타입 가져오기
	void ParseText(CEGUI::String inMsg);							// 채팅창 파싱(특수키 입력 등)
	void OutputText(CEGUI::String inMsg,
		CEGUI::colour colour = CEGUI::colour( 0xFFFFFFFF));			// 텍스트 출력

	CEGUI::Window	*m_LobbyWindow;									// 로비 윈도우
	CEGUI::String	sNamePrefix;									// 레이아웃의 이름
	static int		iInstanceNumber;								// 인스턴스 번호
	bool			m_bLobby;
	Ogre::UTFString sTemp;											// 임시
	Ogre::UTFString	sMessage;										// 한글 메시지(UTF-8)
	bool			*mShutDown;										// 게임 종료
	
	bool			mEntry;											// 입장 여부
	bool			mFull;											// 방 정원 초과 여부
	int				mSelect;										// 선택 방
	int				mPage;											// 방 페이지 수
	int				mCurrPage;										// 현재 페이지
	int				mRoomSize;										// 방 생성 할 때 쓰임. 현재 방이 몇개까지 보이는가.
	int				mMap;											// 방 생성 맵 정보
	int				mType;											// 방 생성 유형 정보
};

