/*

������Ʈ ������ : 2013-04-08
���ϰ� �ѹ� ������!

�� ���� 130424

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

// OIS ���콺 ��ư ID�� CEGUI ���콺 ��ư ID�� ��ȯ
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
	// ���� �̱��� �޸� ����
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
	// ���� �̱��� �Ҵ�
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
	CheckData();																// ù �����ӿ� CheckData() �� �� ����

	return BaseApplication::frameStarted(evt);
}

//-------------------------------------------------------------------------------------
bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)					// �������� �׷����� ����
{
	if(GameState != NULL)
		GameState->CheckData();													// ���� ���¿� ���� ���� ����
	if(GameState != NULL)
		GameState->Run(evt);

	return BaseApplication::frameRenderingQueued(evt);
}

//-------------------------------------------------------------------------------------
bool Game::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}

bool Game::keyPressed(const OIS::KeyEvent &arg)									// ���콺 ��ư�� ���� ���� CEGUI �̺�Ʈ ����
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	if(GameState != NULL)
		GameState->keyPressed(arg);

	return true;
}

bool Game::keyReleased( const OIS::KeyEvent &arg )								// Ű���带 ���� ���� CEGUI �̺�Ʈ ����
{
	CEGUI::System::getSingleton().injectKeyUp(arg.key);

	if(GameState != NULL)
		GameState->keyReleased(arg);
	return true;
}

bool Game::mouseMoved( const OIS::MouseEvent &arg )								// ���콺�� �̵��� CEGUI �̺�Ʈ ����
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}

bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )	// ���콺 ��ư�� ������ ���� CEGUI �̺�Ʈ ����
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));		
    return true;
}

bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )	// ���콺 ��ư�� ���� ���� CEGUI �̺�Ʈ ����
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));		
    return true;
}

void Game::GoNext(SS_TYPE type)
{
	m_GameMainQueue.push(type);													// ���� ť�� ���� ���� push
}

void Game::CheckData()
{
	while(!m_GameMainQueue.empty())
	{
		SS_TYPE type = m_GameMainQueue.front();									// ���� ť���� ���� ���¸� ������
		m_GameMainQueue.pop();													// ���� ť pop

		switch(type)
		{
		case SS_LOGIN :
			{
				GetServerMain()->GoNextServer(SS_TYPE::SS_LOGIN);				// �α��� ������ �̵�
				GameState = CreateLogin();										// �α��� ������ ����
				// ���� ���
				SoundMan::getSingletonPtr()->playSound("Menu");
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// ���� ���� ����(�α��� -> �κ�)
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
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// �κ� ������ ����
			}
			break;
		case SS_ROOM :
			{
				lobbywindow->setVisible(false);
				GameState = CreateRoom();										// �� ������ ����
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// ���� ���� ����(�� -> ����, �� -> �κ�)
				GameState->SetPlayerName(GetServerMain()->GetName());
			}
			break;
		case SS_GAME :
			{
				GetServerMain()->GoNextServer(SS_TYPE::SS_GAME);					// ���� ������ �̵�
				SoundMan::getSingletonPtr()->stopSound("Menu");
				SoundMan::getSingletonPtr()->playSound("Ingame");
				roomwindow->setVisible(false);
				GameState = CreateGame();										// ���� ������, ���� Ŭ���� ����
				GetServerMain()->GetNowServer()->SetGameState(GameState);		// ���� ���� ����(���� -> �κ� or ���?)
				GameState->SetPKey(GetServerMain()->GetPkey());					// Ŭ���̾�Ʈ ���� ��ȣ
			}
			break;
		}
	}
}

CGameState*	Game::CreateLogin()													
{
	// �α��� ������ ����

	if(loginwindow == NULL)
	{
		loginwindow = new GameLoginWindow(this);
	}
	return loginwindow;
}

CGameState*	Game::CreateLobby()													
{
	// �κ� ������ ����

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
	// �� ������ ����

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
	// �ΰ��� Ŭ����, �ΰ��� ������ ����

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
	// �α��� ������ ����

	if(loginwindow != NULL)
	{
		delete loginwindow;
		loginwindow = NULL;
	}
}

void Game::DestroyLobby()														
{
	// �κ� ������ ����

	if(lobbywindow != NULL)
	{
		delete lobbywindow;
		lobbywindow = NULL;
	}
}

void Game::DestroyRoom()														
{
	// �� ������ ����

	if(roomwindow != NULL)
	{
		delete roomwindow;
		roomwindow = NULL;
	}
}

void Game::DestroyGame()										
{
	// �ΰ���, �ΰ��� ������ ����

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