#include "stdafx.h"
#include "..\include\GameLoginWindow.h"
#include "..\include\Game.h"
#include "..\server\CServerMain.h"
#include "..\server\LoginServer.h"

int GameLoginWindow::iInstanceNumber;

GameLoginWindow::GameLoginWindow(Game* pParent) : CGameState(pParent)
{
	m_LoginWindow = NULL;
   iInstanceNumber = 0;
   sNamePrefix = "";
   CreateCEGUIWindow();
   setVisible(true);
   m_bLogin = false;
   sMessage = L"";
}


GameLoginWindow::~GameLoginWindow(void)
{
}

void GameLoginWindow::CreateCEGUIWindow()
{
	// CEGUI 윈도우 관리자의 로컬 포인터를 가져온다. 순전히 작업을 편하게 하기 위함이다.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
 
    // 충돌 없이 인스턴스 수를 늘리기 위해서 값을 1씩 증가시킨다
	// 다중 사용자일 경우 유용하지만 단일 사용자일 경우 필요 없는 변수이다.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // 로그인 창이 들어있는 layout 파일을 등록하고 sNamePrefix 라는 이름으로 지정한다
	// 다중 사용자가 있을 경우 유용하지만 단일 사용자일 경우 필요없다.
	m_LoginWindow = pWindowManager->loadWindowLayout("login.layout",sNamePrefix);
 
    // 로그인 윈도우가 제대로 생성되었는지 확인한다
    if (m_LoginWindow)
    {
        // Root GUI 윈도우에 우리가 새로 생성한 창을 추가한다
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_LoginWindow);
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

void GameLoginWindow::RegisterHandlers()
{
    // Enter 키를 누르거나 버튼을 누를 때 발생하는 이벤트를 처리하는 핸들러를 등록한다
 
    // Login 버튼을 등록한다. 버튼 이름은 "LoginWindow/Button_Apply"이지만
	// 인스턴스가 앞에 붙는 것을 잊지 말아야 한다.
	// "LoginWindow"에서 로드 된 모든 윈도우는 이곳에 등록한다.
	// 하위 윈도우는 따로 핸들러를 등록해야 한다.
    m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Button_Apply")->subscribeEvent(
                        CEGUI::PushButton::EventClicked,    // Scheme의 유형 'CEGUI:PushButton'에 대한 EventClicked 행동을 인정한다
                        CEGUI::Event::Subscriber(           // 클릭할 때 호출하는 기능을 수행한다
                        &GameLoginWindow::Handle_ApplyButtonPressed,  // GameLoginWindow의 멤버 Handle_ApplyButtonPressed 호출
                        this));                             // 현재 사용중인 윈도우

	// Quit 버튼 등록
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Button_Quit")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLoginWindow::Handle_QuitButtonPressed,
		this));
	
	// Join 버튼 등록
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Button_Join")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLoginWindow::Handle_JoinButtonPressed,
		this));
	

	// ID와 Password 입력창에 대한 이벤트를 생성한다

	// ID 입력창에서 Enter 키를 누를 경우
    m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->subscribeEvent(CEGUI::Editbox::EventTextAccepted,
                        CEGUI::Event::Subscriber(&GameLoginWindow::Handle_IDSubmitted,this));

	// Password 입력창에서 Enter 키를 누를 경우
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->subscribeEvent(CEGUI::Editbox::EventTextAccepted,
                        CEGUI::Event::Subscriber(&GameLoginWindow::Handle_PWSubmitted,this));
	
	//// Password 입력창에서 문자를 입력할 경우
	//m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Edit_PW")->subscribeEvent(CEGUI::Editbox::EventKeyDown,
	//					CEGUI::Event::Subscriber(&GameLoginWindow::Handle_PWKeyDown, this));
}

bool GameLoginWindow::Handle_IDSubmitted(const CEGUI::EventArgs &e)
{
    // 자체 EventArg 생성한다. 제한된 용도를 가지고 있지만 이 이벤트를 호출한 윈도우에 대한
	// 구성원에 액세스 할 수 있게 된다.
    const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);

	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->activate();
 
    return true;
}

bool GameLoginWindow::Handle_PWSubmitted(const CEGUI::EventArgs &e)
{
    const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
 
    return Login();
}

bool GameLoginWindow::Handle_PWKeyDown(const CEGUI::EventArgs &e)
{
    // 자체 EventArg 생성
    const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);

    return true;
}

bool GameLoginWindow::Handle_ApplyButtonPressed(const CEGUI::EventArgs &e)
{
    return Login();
}

bool GameLoginWindow::Handle_QuitButtonPressed(const CEGUI::EventArgs &e)
{
	// 게임 종료

	//m_LoginWindow->setVisible(false);
	GetGame()->GameQuit();

    return true;
}

void GameLoginWindow::setVisible(bool visible)
{
	// 로그인 창을 화면에 표시하거나 감춤
    m_LoginWindow->setVisible(visible);
    
	// 현재 창의 상태를 저장함
	m_bLogin = visible;

	// ID 입력창을 활성화(입력 가능한 상태)한다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->activate();

	// 로그인 진행 창과 로그인 에러 창, 가입 창은 기본적으로 감춘 상태
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->setVisible(false);
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(false);
}
 
bool GameLoginWindow::isVisible()
{
	// 현재 창의 표시 상태를 알려준다
    return m_LoginWindow->isVisible();
}

bool GameLoginWindow::Handle_ErrorOKButtonPressed(const CEGUI::EventArgs &e)
{
	// 로그인 에러 창을 감춘다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->setVisible(false);

	// ID와 Password 입력창을 비운다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->setText("");

	// ID 입력창을 활성화(입력 가능한 상태)한다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->activate();
	return true;
}

void GameLoginWindow::ShowErrorWindow(STATUS s)
{
	// 오류 창 표시
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->setVisible(true);
	//이 창을 화면 맨 앞에 표시
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->moveToFront();
	// 오류 메시지 출력
	switch(s)
	{
	case IDPW:
		SetMessage(IDPW);
		break;
	case LOGINFAILED:
		SetMessage(LOGINFAILED);
		break;
	case JOINFAILED:
		SetMessage(JOINFAILED);
		break;
	}
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->getChild("LoginWindow/Error/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());
	// 오류 창에서 OK 버튼을 누르는 경우 발생하는 이벤트 핸들을 생성
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->getChild(sNamePrefix + "LoginWindow/Error/Button_OK")->subscribeEvent(CEGUI::PushButton::EventClicked,
                    CEGUI::Event::Subscriber(&GameLoginWindow::Handle_ErrorOKButtonPressed,this));
}

bool GameLoginWindow::Login()
{
	//ID와 Password 입력에 문제가 없을 경우 로그인 진행 창을 표시하고
	//이곳에 입력받은 ID와 Password를 서버로 전송하여 DB에 저장되어 있는 데이터와 비교 후
	//일치하는 데이터일 경우 로그인을 성공을 알리는 창을 띄우고 게임 로비로 입장하며
	//일치하지 않을 경우 오류창을 띄우고 오류 원인을 알림.

	CLoginServer* p = (CLoginServer*)GetGame()->GetServerMain()->GetNowServer();

	// 입력받은 ID와 Password를 저장함
    CEGUI::String ID = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->getText();
	CEGUI::String PW = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->getText();
	
	// ID와 Password 입력창을 비운다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->setText("");

	// ID 또는 Password 창에 입력이 없을 경우 오류 창을 띄운다
	if(ID == "" || PW == "")
	{
		ShowErrorWindow(IDPW);
		return false;
	}

	// 로그인 진행 창 표시
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(true);
	SetMessage(WAIT);
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild("LoginWindow/Progress/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

	p->CMD_Login(ID.c_str(), PW.c_str());

	return true;
}

bool GameLoginWindow::Handle_JoinButtonPressed(const CEGUI::EventArgs &e)
{
	// 가입 창 표시
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(true);

	// // ID 입력창을 활성화(입력 가능한 상태)한다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->activate();

	// 오류 창에서 OK 버튼을 누르는 경우 발생하는 이벤트 핸들을 생성
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Button_OK")->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&GameLoginWindow::Handle_JoinOKButtonPressed,this));

	// 오류 창에서 Cancel 버튼을 누르는 경우 발생하는 이벤트 핸들을 생성
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Button_Cancel")->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&GameLoginWindow::Handle_JoinCancelButtonPressed,this));

	return true;
}

bool GameLoginWindow::Handle_JoinOKButtonPressed(const CEGUI::EventArgs &e)
{
	return Join();
}
bool GameLoginWindow::Handle_JoinCancelButtonPressed(const CEGUI::EventArgs &e)
{
	// ID와 Password 입력창을 비운다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_PW")->setText("");

	// 가입 창을 감춤
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(false);

	return true;
}

bool GameLoginWindow::Join()
{
	CLoginServer* p = (CLoginServer*)GetGame()->GetServerMain()->GetNowServer();

	// 입력받은 ID와 Password를 저장함
	CEGUI::String ID = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->getText();
	CEGUI::String PW = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_PW")->getText();
	
	// ID와 Password 입력창을 비운다
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_PW")->setText("");

	// ID 또는 Password 창에 입력이 없을 경우 오류 창을 띄운다
	if(ID == "" || PW == "")
	{
		ShowErrorWindow(IDPW);
		return false;
	}

	ShowProgressWindow(WAIT);

	p->CMD_CreateAccount(ID.c_str(), PW.c_str());

	return true;
}

bool GameLoginWindow::Handle_ProgressOKButtonPressed(const CEGUI::EventArgs &e)
{
	// 진행 창을 감춤
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);

	return true;
}

void GameLoginWindow::SetMessage(STATUS s)
{
	// 각 상태에 맞는 메시지를 설정한다
	switch(s)
	{
	case IDPW:
		sMessage = L"아이디와 패스워드를 확인해 주세요!";
		break;
	case WAIT:
		sMessage = L"잠시 기다려 주세요...";
		break;
	case LOGINSUCCESS:
		sMessage = L"로그인 성공!";
		break;
	case LOGINFAILED:
		sMessage = L"로그인 실패!";
		break;
	case JOINSUCCESS:
		sMessage = L"가입 성공!";
		break;
	case JOINFAILED:
		sMessage = L"가입 실패!";
		break;
	}
}

void GameLoginWindow::ShowProgressWindow(STATUS s)
{
	// 각 상태에 맞는 진행창을 표시한다
	switch(s)
	{
	case LOGINSUCCESS:
		// 로그인 성공 메시지
		SetMessage(LOGINSUCCESS);
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild("LoginWindow/Progress/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

		// 로그인 창을 감춤
		m_LoginWindow->setVisible(false);
		GetGame()->GoNext(SS_TYPE::SS_LOBBY);
		break;
	case JOINSUCCESS:
		// 가입 창을 감춤
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(false);

		// 가입 성공 메시지
		SetMessage(JOINSUCCESS);
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild("LoginWindow/Progress/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

		// 진행 창에서 OK 버튼을 누르는 경우 발생하는 이벤트 핸들을 생성
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild(sNamePrefix + "LoginWindow/Progress/Button_OK")->subscribeEvent(CEGUI::PushButton::EventClicked,
						CEGUI::Event::Subscriber(&GameLoginWindow::Handle_ProgressOKButtonPressed,this));
		break;
	case WAIT:
		// 진행 창 표시
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->moveToFront();
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(true);
		SetMessage(WAIT);
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild("LoginWindow/Progress/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());
		break;
	}
}

void GameLoginWindow::PushData(LP_TYPE type, std::string id)
{
	stLogin stlogin;
	stlogin.type = type;
	stlogin.id = id;
	m_LoginQueue.push(stlogin);
}

bool GameLoginWindow::CheckData()
{
	while(!m_LoginQueue.empty())
	{
		stLogin stlogin = m_LoginQueue.front();
		m_LoginQueue.pop();
		switch(stlogin.type)
		{
		case LP_TYPE::LP_CREATEACCOUNT_SUCC :			//계정생성 성공
			{
				ShowProgressWindow(STATUS::JOINSUCCESS);
			}
			break;
		case LP_TYPE::LP_CREATEACCOUNT_FAIL :			//계정생성 실패
			{
				ShowProgressWindow(STATUS::JOINFAILED);
				m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);
				ShowErrorWindow(JOINFAILED);
			}
			break;
		case LP_TYPE::LP_LOGIN_SUCC :			//로그인 성공
			{
				ShowProgressWindow(STATUS::LOGINSUCCESS);
			}
			break;
		case LP_TYPE::LP_LOGIN_FAIL :			//로그인 실패
			{
				ShowProgressWindow(STATUS::LOGINFAILED);
				m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);
				// 오류 창 표시
				ShowErrorWindow(LOGINFAILED);
			}
			break;
		}
	}
	return false;
}