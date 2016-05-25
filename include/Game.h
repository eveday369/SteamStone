#pragma once

#ifndef __Game_h_
#define __Game_h_

#include "BaseApplication.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include "stdafx.h"

//#include "stdafx.h"
//#include "CServerMain.h"

class CServerMain;
class CGameState;
class Game : public BaseApplication
{
private :
	typedef std::queue<SS_TYPE> GAMEMAIN_QUEUE;
private:
	GAMEMAIN_QUEUE			m_GameMainQueue;

	CGameState				*GameState;
	GameLoginWindow			*loginwindow;
	GameLobbyWindow			*lobbywindow;
	GameRoomWindow			*roomwindow;
	GameRunWindow			*runwindow;
	GameResultWindow		*resultwindow;
	CGameRun				*gamerun;
	CResult					*result;
	CCharacter				*character;
	CKeyboard				*keyboard;
	GUIBase					*gui;

	CServerMain				*m_pServerMain;

	//추가 & 수정 요망
	//데이터가 모두 완료 되면 됐다는 것 표시
	//CEGUI 데이터 delete. 현재 메모리 누수 의심됨
public:
    Game(void);
    virtual ~Game(void);
	
	//Functions
    virtual void createScene(void);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);							//프레임이 시작할 때
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);					//프레임이 돌아가는 도중
	virtual bool frameEnded(const Ogre::FrameEvent& evt);							//프레임이 끝날 때

	// OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


	void					GoNext(SS_TYPE type);
	void					GameQuit() { mShutDown = true; }

	CGameState*				CreateLogin();
	CGameState*				CreateLobby();
	CGameState*				CreateRoom();
	CGameState*				CreateGame();

	void					DestroyLogin();
	void					DestroyLobby();
	void					DestroyRoom();
	void					DestroyGame();

	void					CheckData();

	inline CServerMain*		GetServerMain()					{ return m_pServerMain; }
	inline void				SetServerMain(CServerMain* p)	{ m_pServerMain = p; }
};

#endif // #ifndef __Game_h_
