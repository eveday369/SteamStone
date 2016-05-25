/*

프로젝트 가동일 : 2013-04-08
거하게 한번 만들어봅쉐!

현 버전 130424

*/
#include "stdafx.h"
#include "Game.h"
#include "..\Server\Server.h"
#include "..\Server\CServerMain.h"
#include "..\server\LobbyServer.h"

/*=======================================================================================
								 Program_Run - Do Not Touch!
=======================================================================================*/
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Game app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

// OIS 마우스 버튼 ID를 CEGUI 마우스 버튼 ID로 변환
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}

//-------------------------------------------------------------------------------------
Game::Game(void)
{
	GameState			= NULL;
	lobbywindow			= NULL;
	roomwindow			= NULL;
	gamerun				= NULL;
	runwindow			= NULL;
	resultwindow		= NULL;
	result				= NULL;
	gui					= NULL;
	loginwindow			= NULL;
	keyboard			= NULL;
	m_pServerMain		= NULL;
}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
	// 사운드 싱글톤 메모리 제거
	delete SoundMan::getSingletonPtr();

	DestroyLogin();
	DestroyLobby();
	DestroyGame();
	//DestroyGameUI();

	if(m_pServerMain != NULL)
	{
		delete m_pServerMain;
		m_pServerMain = NULL;
	}
	GameState = NULL;
}

//-------------------------------------------------------------------------------------

void Game::createScene(void)
{
	// 사운드 싱글톤 할당
	new SoundMan();

	gui = new GUIBase;
	keyboard = new CKeyboard;

	m_pServerMain = new CServerMain;
	m_pServerMain->InitServerMain();

	GoNext(SS_TYPE::SS_LOGIN);
}

//-------------------------------------------------------------------------------------
bool Game::frameStarted(const Ogre::FrameEvent& evt)
{
	CheckData();																// 첫 프레임에 CheckData() 한 번 실행

	return BaseApplication::frameStarted(evt);
}

//-------------------------------------------------------------------------------------
bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)					// 프레임이 그려지는 동안
{
	if(GameState != NULL)
		GameState->CheckData();													// 게임 상태에 따른 로직 실행
	if(GameState != NULL)
		GameState->Run(evt);

	return BaseApplication::frameRenderingQueued(evt);
}

//-------------------------------------------------------------------------------------
bool Game::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}

bool Game::keyPressed(const OIS::KeyEvent &arg)									// 마우스 버튼을 떼는 순간 CEGUI 이벤트 전달
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	if(GameState != NULL)
		GameState->keyPressed(arg);

	return true;
}

bool Game::keyReleased( const OIS::KeyEvent &arg )								// 키보드를 떼는 순간 CEGUI 이벤트 전달
{
	CEGUI::System::getSingleton().injectKeyUp(arg.key);

	if(GameState != NULL)
		GameState->keyReleased(arg);
	return true;
}

bool Game::mouseMoved( const OIS::MouseEvent &arg )								// 마우스를 이동을 CEGUI 이벤트 전달
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}

bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )	// 마우스 버튼을 누르는 순간 CEGUI 이벤트 전달
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));		
    return true;
}

bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )	// 마우스 버튼을 떼는 순간 CEGUI 이벤트 전달
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));		
    return true;
}

void Game::GoNext(SS_TYPE type)
{
	m_GameMainQueue.push(type);													// 메인 큐에 게임 상태 push
}

void Game::CheckData()
{
	while(!m_GameMainQueue.empty())
	{
		SS_TYPE type = m_GameMainQueue.front();									// 메인 큐에서 게임 상태를 가져옴
		m_GameMainQueue.pop();													// 메인 큐 pop

		switch(type)
		{
		case SS_LOGIN :
			{
				GetServerMain()->GoNextServer(SS_TYPE::SS_LOGIN);				// 로그인 서버로 이동
				GameState = CreateLogin();										// 로그인 윈도우 생성
				// 사운드 재생
				SoundMan::getSingletonPtr()->playSound("Menu");
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// 게임 상태 변경(로그인 -> 로비)
			}
			break;
		case SS_LOBBY :
			{
				GetServerMain()->GoNextServer(SS_TYPE::SS_LOBBY);
				DestroyGame();
				loginwindow->setVisible(false);
				if(roomwindow != NULL)
				{
					roomwindow->setVisible(false);
					lobbywindow->setVisible(true);
				}
				GameState = CreateLobby();
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// 로비 윈도우 생성
			}
			break;
		case SS_ROOM :
			{
				lobbywindow->setVisible(false);
				GameState = CreateRoom();										// 방 윈도우 생성
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// 게임 상태 변경(방 -> 게임, 방 -> 로비)
				GameState->SetPlayerName(GetServerMain()->GetName());
			}
			break;
		case SS_GAME :
			{
				GetServerMain()->GoNextServer(SS_TYPE::SS_GAME);					// 게임 서버로 이동
				SoundMan::getSingletonPtr()->stopSound("Menu");
				SoundMan::getSingletonPtr()->playSound("Ingame");
				roomwindow->setVisible(false);
				GameState = CreateGame();										// 게임 윈도우, 게임 클래스 생성
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// 게임 상태 변경(게임 -> 로비 or 결과?)
				GameState->SetPKey(GetServerMain()->GetPkey());					// 클라이언트 고유 번호
			}
			break;
		}
	}
}

CGameState*	Game::CreateLogin()													
{
	// 로그인 윈도우 생성

	if(loginwindow == NULL)
	{
		loginwindow = new GameLoginWindow(this);
	}
	return loginwindow;
}

CGameState*	Game::CreateLobby()													
{
	// 로비 윈도우 생성

	if(lobbywindow == NULL)
	{
		lobbywindow = new GameLobbyWindow(this);
	}
	else
		lobbywindow->setVisible(true);

	return lobbywindow;
}

CGameState*	Game::CreateRoom()													
{
	// 방 윈도우 생성

	if(roomwindow == NULL)
	{
		roomwindow = new GameRoomWindow(this);
	}
	else
		roomwindow->setVisible(true);

	return roomwindow;
}

CGameState*	Game::CreateGame()													
{
	// 인게임 클래스, 인게임 윈도우 생성

	if(gamerun == NULL)
	{
		gamerun = new CGameRun(this);
		printf("Create GameRun\n");
		runwindow = new GameRunWindow(this);
		printf("Create GameRunWindow\n");
		resultwindow = new GameResultWindow(this);
		printf("Create GameResultWindow\n");

		gamerun->SetRunWindow(runwindow);
		gamerun->SetResultWindow(resultwindow);
		gamerun->initialize(mSceneMgr, mCamera);
		CEGUI::MouseCursor::getSingletonPtr()->setVisible(false);
	}
	else if(runwindow != NULL)
		runwindow->setVisible(true);

	return gamerun;
}

void Game::DestroyLogin()														
{
	// 로그인 윈도우 제거

	if(loginwindow != NULL)
	{
		delete loginwindow;
		loginwindow = NULL;
	}
}

void Game::DestroyLobby()														
{
	// 로비 윈도우 제거

	if(lobbywindow != NULL)
	{
		delete lobbywindow;
		lobbywindow = NULL;
	}
}

void Game::DestroyRoom()														
{
	// 방 윈도우 제거

	if(roomwindow != NULL)
	{
		delete roomwindow;
		roomwindow = NULL;
	}
}

void Game::DestroyGame()										
{
	// 인게임, 인게임 윈도우 제거

	if(gamerun != NULL)
	{
		printf("Destroy ResultWindow\n");
		resultwindow->setVisible(false);
		delete resultwindow;
		resultwindow = NULL;

		printf("Destroy RunWindow\n");
		runwindow->setVisible(false);
		delete runwindow;
		runwindow = NULL;

		printf("Destroy GameRun\n");
		delete gamerun;
		gamerun = NULL;
	}
}