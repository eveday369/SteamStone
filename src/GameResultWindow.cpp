#include "stdafx.h"
#include "GameResultWindow.h"
#include "..\include\Game.h"
#include "..\server\CServerMain.h"
#include "..\server\GameServer.h"

int GameResultWindow::iInstanceNumber;

GameResultWindow::GameResultWindow(Game* pParent) : CGameState(pParent)
//GameResultWindow::GameResultWindow()
{
	m_ResultWindow = NULL;
	iInstanceNumber = 0;
	sNamePrefix = "";
	CreateCEGUIWindow();
	setVisible(false);
	m_bRun = false;
}

GameResultWindow::~GameResultWindow()
{
	Ogre::Timer t;

	while(true)
	{
		if(t.getMilliseconds() >= 3000)
		{
			break;
		}
	}

	CEGUI::System::getSingleton().getGUISheet()->getChild("Result")->destroy();
}

void GameResultWindow::CreateCEGUIWindow()
{
	// CEGUI 윈도우 관리자의 로컬 포인터를 가져온다. 순전히 작업을 편하게 하기 위함이다.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
    // 충돌 없이 인스턴스 수를 늘리기 위해서 값을 1씩 증가시킨다
	// 다중 사용자일 경우 유용하지만 단일 사용자일 경우 필요 없는 변수이다.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // 로그인 창이 들어있는 layout 파일을 등록하고 sNamePrefix 라는 이름으로 지정한다
	// 다중 사용자가 있을 경우 유용하지만 단일 사용자일 경우 필요없다.
	m_ResultWindow = pWindowManager->loadWindowLayout("result.layout",sNamePrefix);
 
    // 로그인 윈도우가 제대로 생성되었는지 확인한다
    if (m_ResultWindow)
    {
        // Root GUI 윈도우에 우리가 새로 생성한 창을 추가한다
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_ResultWindow);
		// 이벤트 처리기 등록 (클릭, 타이핑, etc)
        (this)->RegisterHandlers();
		//m_LoginWindow->setUserData(this);
    }
    else
    {
        // 정상적으로 윈도우가 생성되지 않을 경우 이벤트 로그에 오류사항을 기록한다.
        CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the LoginWindow from .layout");
    }
}

void GameResultWindow::RegisterHandlers()
{
	m_ResultWindow->getChild(sNamePrefix + "Result/Return")->subscribeEvent(
		CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameResultWindow::Handle_ReturnButtonPressed, this));
	
	m_ResultWindow->getChild(sNamePrefix + "Result/Exit")->subscribeEvent(
		CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameResultWindow::Handle_ExitButtonPressed, this));
}

void GameResultWindow::setVisible(bool visible)
{
	// 로그인 창을 화면에 표시하거나 감춤
    m_ResultWindow->setVisible(visible);

	// 현재 창의 상태를 저장함
	m_bRun = visible;

}
 
bool GameResultWindow::isVisible()
{
	// 현재 창의 표시 상태를 알려준다
    return m_ResultWindow->isVisible();
}

bool GameResultWindow::Handle_ExitButtonPressed(const CEGUI::EventArgs &e)
{
	// 크레딧 이미지 윈도우 표시
	m_ResultWindow->getChild(sNamePrefix + "Result/Credit")->setVisible(true);
	m_ResultWindow->invalidate();

	GetGame()->GameQuit();

	return true;
}

bool GameResultWindow::Handle_ReturnButtonPressed(const CEGUI::EventArgs &e)
{
	// 크레딧 이미지 윈도우 표시
	m_ResultWindow->getChild(sNamePrefix + "Result/Credit")->setVisible(true);
	m_ResultWindow->invalidate();

	//GAME -> ROOM
	CGameServer* p = (CGameServer*)GetGame()->GetServerMain()->GetNowServer();
	p->DisconnectToServer();

	// 게임 상태 변경
	GetGame()->GoNext(SS_TYPE::SS_LOBBY);
	SoundMan::getSingletonPtr()->playSound("Menu");
	SoundMan::getSingletonPtr()->stopSound("Ingame");
	return true;
}

bool GameResultWindow::setWinLose(bool win)
{
	if(win)
		m_ResultWindow->getChild(sNamePrefix + "Result/WinLose")
			->setProperty("Image", "set:Win image:Win");

	else
		m_ResultWindow->getChild(sNamePrefix + "Result/WinLose")
			->setProperty("Image", "set:Win image:Lose");

	return true;
}

bool GameResultWindow::CheckData()
{
	return true;
}