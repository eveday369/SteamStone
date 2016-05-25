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

	//�߰� & ���� ���
	//�����Ͱ� ��� �Ϸ� �Ǹ� �ƴٴ� �� ǥ��
	//CEGUI ������ delete. ���� �޸� ���� �ǽɵ�
public:
    Game(void);
    virtual ~Game(void);
	
	//Functions
    virtual void createScene(void);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);							//�������� ������ ��
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);					//�������� ���ư��� ����
	virtual bool frameEnded(const Ogre::FrameEvent& evt);							//�������� ���� ��

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
