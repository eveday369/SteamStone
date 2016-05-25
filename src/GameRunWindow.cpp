#include "stdafx.h"
#include "GameRunWindow.h"
#include "..\include\Game.h"
#include "..\server\CServerMain.h"
#include "..\server\GameServer.h"

int GameRunWindow::iInstanceNumber;

GameRunWindow::GameRunWindow(Game* pParent) : CGameState(pParent)
//GameRunWindow::GameRunWindow()
{
    m_RunWindow = NULL;
    iInstanceNumber = 0;
    sNamePrefix = "";
	bMenu = false;
    CreateCEGUIWindow();
    setVisible(true);
    m_bRun = false;
}

GameRunWindow::~GameRunWindow()
{
	Ogre::Timer t;

	while(true)
	{
		if(t.getMilliseconds() >= 3000)
		{
			break;
		}
	}

	CEGUI::System::getSingleton().getGUISheet()->getChild("Run")->destroy();
	CEGUI::MouseCursor::getSingletonPtr()->setVisible(true);
}

void GameRunWindow::CreateCEGUIWindow()
{
    // CEGUI ������ �������� ���� �����͸� �����´�. ������ �۾��� ���ϰ� �ϱ� �����̴�.
    CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
    // �浹 ���� �ν��Ͻ� ���� �ø��� ���ؼ� ���� 1�� ������Ų��
    // ���� ������� ��� ���������� ���� ������� ��� �ʿ� ���� �����̴�.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // �α��� â�� ����ִ� layout ������ ����ϰ� sNamePrefix ��� �̸����� �����Ѵ�
    // ���� ����ڰ� ���� ��� ���������� ���� ������� ��� �ʿ����.
    m_RunWindow = pWindowManager->loadWindowLayout("ingame.layout",sNamePrefix);
 
    // �α��� �����찡 ����� �����Ǿ����� Ȯ���Ѵ�
    if (m_RunWindow)
    {
        // Root GUI �����쿡 �츮�� ���� ������ â�� �߰��Ѵ�
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_RunWindow);
        // �̺�Ʈ ó���� ��� (Ŭ��, Ÿ����, etc)
        (this)->RegisterHandlers();
        //m_LoginWindow->setUserData(this);
		imageset = CEGUI::ImagesetManager::getSingletonPtr();
    }
    else
    {
        // ���������� �����찡 �������� ���� ��� �̺�Ʈ �α׿� ���������� ����Ѵ�.
        CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the LoginWindow from .layout");
    }
}

void GameRunWindow::RegisterHandlers()
{
	// Ȯ�� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RunWindow->getChild(sNamePrefix + "Run/Menu")->getChild(sNamePrefix + "Run/Menu/Button_Return")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRunWindow::Handle_ReturnButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ��� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RunWindow->getChild(sNamePrefix + "Run/Menu")->getChild(sNamePrefix + "Run/Menu/Button_Exit")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRunWindow::Handle_ExitButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
}

void GameRunWindow::setVisible(bool visible)
{
    // �α��� â�� ȭ�鿡 ǥ���ϰų� ����
    m_RunWindow->setVisible(visible);

    // ���� â�� ���¸� ������
    m_bRun = visible;

}
 
bool GameRunWindow::isVisible()
{
    // ���� â�� ǥ�� ���¸� �˷��ش�
    return m_RunWindow->isVisible();
}

bool GameRunWindow::isMenu()
{
	return bMenu;
}

bool GameRunWindow::ShowMenu(bool show)
{
	// �޴�â ǥ��

	m_RunWindow->getChild(sNamePrefix + "Run/Menu")->setVisible(show);
	CEGUI::MouseCursor::getSingletonPtr()->setVisible(show);
	bMenu = show;

	return true;
}

bool GameRunWindow::Handle_ReturnButtonPressed(const CEGUI::EventArgs &e)
{
	ShowMenu(false);

	return true;
}

bool GameRunWindow::Handle_ExitButtonPressed(const CEGUI::EventArgs &e)
{
	// ũ���� �̹��� ������ ǥ��

	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();

	pWindowManager->getWindow("Result")->setVisible(true);
	pWindowManager->getWindow("Result")->getChild("Result/Credit")->setVisible(true);
	pWindowManager->getWindow("Result")->invalidate();
	pWindowManager->getWindow("Result")->moveToFront();

	ShowMenu(false);

	CGameServer* p = (CGameServer*)GetGame()->GetServerMain()->GetNowServer();
	p->DisconnectToServer();



	GetGame()->GoNext(SS_TYPE::SS_LOBBY);												// ���� ���� ����
	SoundMan::getSingletonPtr()->playSound("Menu");
	SoundMan::getSingletonPtr()->stopSound("Ingame");


	return true;
}

bool GameRunWindow::Init(int job)
{
	// �÷��̾� ĳ���Ϳ� ���� �̹��� �ʱ�ȭ(ĳ���� �̹���, ��ų �̹��� ��)
	switch(job)
	{
	case 0:
		m_RunWindow->getChild(sNamePrefix + "Run/MyCharacter")->getChild(sNamePrefix + "Run/MyCharacter/Image")
			->setProperty("Image", "set:Walker image:Circle");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill1")
			->setProperty("Image", "set:Ingame image:Walker_Skill1_Normal");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill2")
			->setProperty("Image", "set:Ingame image:Walker_Skill2_Normal");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill3")
			->setProperty("Image", "set:Ingame image:Walker_Skill3_Normal");
		break;
	case 1:
		m_RunWindow->getChild(sNamePrefix + "Run/MyCharacter")->getChild(sNamePrefix + "Run/MyCharacter/Image")
			->setProperty("Image", "set:Gunner image:Circle");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill1")
			->setProperty("Image", "set:Ingame image:Gunner_Skill1_Normal");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill2")
			->setProperty("Image", "set:Ingame image:Gunner_Skill2_Normal");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill3")
			->setProperty("Image", "set:Ingame image:Gunner_Skill3_Normal");
		break;
	case 2:
		m_RunWindow->getChild(sNamePrefix + "Run/MyCharacter")->getChild(sNamePrefix + "Run/MyCharacter/Image")
			->setProperty("Image", "set:Psyker image:Circle");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill1")
			->setProperty("Image", "set:Ingame image:Psyker_Skill1_Normal");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill2")
			->setProperty("Image", "set:Ingame image:Psyker_Skill2_Normal");
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill3")
			->setProperty("Image", "set:Ingame image:Psyker_Skill3_Normal");
		break;
	}

	return true;
}

bool GameRunWindow::TimeCheck(float fGameTime)
{
	// ���� �ð� ǥ��

	int time, hundred, ten, one;

	char std[3][10];
	CEGUI::String num[3];

	if(fGameTime >= 999.0f)
		time = 999;
	else if(fGameTime <= 0.0f)
	{
		time = 0;
	}
	else
		time = (int)fGameTime;
	one = time%10;
	ten = ((time%100) - one)/10;
	hundred = (time - ten - one)/100;
	
	
	itoa( one, std[0], 10);
	num[0] = std[0];
	m_RunWindow->getChild("Run/Time")->getChild("Run/Time/One")
		->setProperty("Image", "set:Ingame image:Time_" +num[0]);
	
	
	itoa( ten, std[1], 10);
	num[1] = std[1];
	m_RunWindow->getChild("Run/Time")->getChild("Run/Time/Ten")
		->setProperty("Image", "set:Ingame image:Time_" +num[1]);


	itoa( hundred, std[2], 10);
	num[2] = std[2];
	m_RunWindow->getChild("Run/Time")->getChild("Run/Time/Hundred")
		->setProperty("Image", "set:Ingame image:Time_" +num[2]);

	

	
	return true;

}
bool GameRunWindow::CheckStatus(int HP, int MaxHP, int MP, int MaxMP)
{
	// �÷��̾� ���� ǥ��(HP, MP)

	float _rHP = 0.0f, _rMP = 0.0f;
	float _MaxHP, _MaxMP;
	if(MaxHP <= 0.0f)
		_MaxHP = 0.01f;
	else
		_MaxHP = MaxHP;
	if(MaxMP <= 0.0f)
		_MaxMP = 0.01f;
	else
		_MaxMP = MaxMP;
	_rHP = HP/_MaxHP;
	_rMP = MP/_MaxMP;

	m_RunWindow->getChild(sNamePrefix + "Run/MyCharacter")->getChild(sNamePrefix + "Run/MyCharacter/HP")
		->setSize( CEGUI::UVector2(CEGUI::UDim(_rHP*0.690, 0), CEGUI::UDim(0.200, 0)));

	m_RunWindow->getChild(sNamePrefix + "Run/MyCharacter")->getChild(sNamePrefix + "Run/MyCharacter/MP")
		->setSize( CEGUI::UVector2(CEGUI::UDim(_rMP*0.725, 0), CEGUI::UDim(0.100, 0)));
	return true;
}

bool GameRunWindow::CheckSkill(GS_TYPE type, float mCool, float cCool)
{
	// ��ų ���� ��ų ���ð��� ���� ��ų �̹��� ����

	float cool, _mCool, _cCool;
	if(mCool <= 0.1f)
		_mCool = 0.1f;
	else
		_mCool = mCool;
	if(cCool <= 0.000f)
		_cCool = -0.001f;
	else
		_cCool = cCool;

	cool = _mCool / _cCool;

	switch(type)
	{
	case GS_TYPE::GS_SKILL1:
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill1")
			->getChild(sNamePrefix + "Run/Skills/Skill1/Skill1_Cool")
			->setProperty("UnifiedAreaRect", "{{0.0,0},{0.0,0},{1.0,0},{" + Ogre::StringConverter::toString(cool) + ",0}}");
		break;
	case GS_TYPE::GS_SKILL2:
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill2")
			->getChild(sNamePrefix + "Run/Skills/Skill2/Skill2_Cool")
			->setProperty("UnifiedAreaRect", "{{0.0,0},{0.0,0},{1.0,0},{" + Ogre::StringConverter::toString(cool) + ",0}}");
		break;
	case GS_TYPE::GS_SKILL3:
		m_RunWindow->getChild(sNamePrefix + "Run/Skills")->getChild(sNamePrefix + "Run/Skills/Skill3")
			->getChild(sNamePrefix + "Run/Skills/Skill3/Skill3_Cool")
			->setProperty("UnifiedAreaRect", "{{0.0,0},{0.0,0},{1.0,0},{" + Ogre::StringConverter::toString(cool) + ",0}}");
		break;
	}
	return true;
}

bool GameRunWindow::CheckData()
{
	return true;
}

void GameRunWindow::ShowLoadingImage(bool b)
{
	// �ε� �̹��� ǥ��

	m_RunWindow->getChild(sNamePrefix + "Run/Loading")->setVisible(b);
}