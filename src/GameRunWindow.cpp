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
    // CEGUI 윈도우 관리자의 로컬 포인터를 가져온다. 순전히 작업을 편하게 하기 위함이다.
    CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
    // 충돌 없이 인스턴스 수를 늘리기 위해서 값을 1씩 증가시킨다
    // 다중 사용자일 경우 유용하지만 단일 사용자일 경우 필요 없는 변수이다.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // 로그인 창이 들어있는 layout 파일을 등록하고 sNamePrefix 라는 이름으로 지정한다
    // 다중 사용자가 있을 경우 유용하지만 단일 사용자일 경우 필요없다.
    m_RunWindow = pWindowManager->loadWindowLayout("ingame.layout",sNamePrefix);
 
    // 로그인 윈도우가 제대로 생성되었는지 확인한다
    if (m_RunWindow)
    {
        // Root GUI 윈도우에 우리가 새로 생성한 창을 추가한다
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_RunWindow);
        // 이벤트 처리기 등록 (클릭, 타이핑, etc)
        (this)->RegisterHandlers();
        //m_LoginWindow->setUserData(this);
		imageset = CEGUI::ImagesetManager::getSingletonPtr();
    }
    else
    {
        // 정상적으로 윈도우가 생성되지 않을 경우 이벤트 로그에 오류사항을 기록한다.
        CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the LoginWindow from .layout");
    }
}

void GameRunWindow::RegisterHandlers()
{
	// 확인 버튼을 클릭할 경우 이벤트 발생
	m_RunWindow->getChild(sNamePrefix + "Run/Menu")->getChild(sNamePrefix + "Run/Menu/Button_Return")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRunWindow::Handle_ReturnButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 취소 버튼을 클릭할 경우 이벤트 발생
	m_RunWindow->getChild(sNamePrefix + "Run/Menu")->getChild(sNamePrefix + "Run/Menu/Button_Exit")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRunWindow::Handle_ExitButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
}

void GameRunWindow::setVisible(bool visible)
{
    // 로그인 창을 화면에 표시하거나 감춤
    m_RunWindow->setVisible(visible);

    // 현재 창의 상태를 저장함
    m_bRun = visible;

}
 
bool GameRunWindow::isVisible()
{
    // 현재 창의 표시 상태를 알려준다
    return m_RunWindow->isVisible();
}

bool GameRunWindow::isMenu()
{
	return bMenu;
}

bool GameRunWindow::ShowMenu(bool show)
{
	// 메뉴창 표시

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
	// 크레딧 이미지 윈도우 표시

	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();

	pWindowManager->getWindow("Result")->setVisible(true);
	pWindowManager->getWindow("Result")->getChild("Result/Credit")->setVisible(true);
	pWindowManager->getWindow("Result")->invalidate();
	pWindowManager->getWindow("Result")->moveToFront();

	ShowMenu(false);

	CGameServer* p = (CGameServer*)GetGame()->GetServerMain()->GetNowServer();
	p->DisconnectToServer();



	GetGame()->GoNext(SS_TYPE::SS_LOBBY);												// 게임 상태 변경
	SoundMan::getSingletonPtr()->playSound("Menu");
	SoundMan::getSingletonPtr()->stopSound("Ingame");


	return true;
}

bool GameRunWindow::Init(int job)
{
	// 플레이어 캐릭터에 대한 이미지 초기화(캐릭터 이미지, 스킬 이미지 등)
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
	// 게임 시간 표시

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
	// 플레이어 상태 표시(HP, MP)

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
	// 스킬 사용시 스킬 대기시간에 맞춰 스킬 이미지 변경

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
	// 로딩 이미지 표시

	m_RunWindow->getChild(sNamePrefix + "Run/Loading")->setVisible(b);
}