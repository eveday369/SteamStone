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
	// CEGUI ������ �������� ���� �����͸� �����´�. ������ �۾��� ���ϰ� �ϱ� �����̴�.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
    // �浹 ���� �ν��Ͻ� ���� �ø��� ���ؼ� ���� 1�� ������Ų��
	// ���� ������� ��� ���������� ���� ������� ��� �ʿ� ���� �����̴�.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // �α��� â�� ����ִ� layout ������ ����ϰ� sNamePrefix ��� �̸����� �����Ѵ�
	// ���� ����ڰ� ���� ��� ���������� ���� ������� ��� �ʿ����.
	m_ResultWindow = pWindowManager->loadWindowLayout("result.layout",sNamePrefix);
 
    // �α��� �����찡 ����� �����Ǿ����� Ȯ���Ѵ�
    if (m_ResultWindow)
    {
        // Root GUI �����쿡 �츮�� ���� ������ â�� �߰��Ѵ�
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_ResultWindow);
		// �̺�Ʈ ó���� ��� (Ŭ��, Ÿ����, etc)
        (this)->RegisterHandlers();
		//m_LoginWindow->setUserData(this);
    }
    else
    {
        // ���������� �����찡 �������� ���� ��� �̺�Ʈ �α׿� ���������� ����Ѵ�.
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
	// �α��� â�� ȭ�鿡 ǥ���ϰų� ����
    m_ResultWindow->setVisible(visible);

	// ���� â�� ���¸� ������
	m_bRun = visible;

}
 
bool GameResultWindow::isVisible()
{
	// ���� â�� ǥ�� ���¸� �˷��ش�
    return m_ResultWindow->isVisible();
}

bool GameResultWindow::Handle_ExitButtonPressed(const CEGUI::EventArgs &e)
{
	// ũ���� �̹��� ������ ǥ��
	m_ResultWindow->getChild(sNamePrefix + "Result/Credit")->setVisible(true);
	m_ResultWindow->invalidate();

	GetGame()->GameQuit();

	return true;
}

bool GameResultWindow::Handle_ReturnButtonPressed(const CEGUI::EventArgs &e)
{
	// ũ���� �̹��� ������ ǥ��
	m_ResultWindow->getChild(sNamePrefix + "Result/Credit")->setVisible(true);
	m_ResultWindow->invalidate();

	//GAME -> ROOM
	CGameServer* p = (CGameServer*)GetGame()->GetServerMain()->GetNowServer();
	p->DisconnectToServer();

	// ���� ���� ����
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