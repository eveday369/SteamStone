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
	// CEGUI ������ �������� ���� �����͸� �����´�. ������ �۾��� ���ϰ� �ϱ� �����̴�.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
 
    // �浹 ���� �ν��Ͻ� ���� �ø��� ���ؼ� ���� 1�� ������Ų��
	// ���� ������� ��� ���������� ���� ������� ��� �ʿ� ���� �����̴�.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // �α��� â�� ����ִ� layout ������ ����ϰ� sNamePrefix ��� �̸����� �����Ѵ�
	// ���� ����ڰ� ���� ��� ���������� ���� ������� ��� �ʿ����.
	m_LoginWindow = pWindowManager->loadWindowLayout("login.layout",sNamePrefix);
 
    // �α��� �����찡 ����� �����Ǿ����� Ȯ���Ѵ�
    if (m_LoginWindow)
    {
        // Root GUI �����쿡 �츮�� ���� ������ â�� �߰��Ѵ�
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_LoginWindow);
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

void GameLoginWindow::RegisterHandlers()
{
    // Enter Ű�� �����ų� ��ư�� ���� �� �߻��ϴ� �̺�Ʈ�� ó���ϴ� �ڵ鷯�� ����Ѵ�
 
    // Login ��ư�� ����Ѵ�. ��ư �̸��� "LoginWindow/Button_Apply"������
	// �ν��Ͻ��� �տ� �ٴ� ���� ���� ���ƾ� �Ѵ�.
	// "LoginWindow"���� �ε� �� ��� ������� �̰��� ����Ѵ�.
	// ���� ������� ���� �ڵ鷯�� ����ؾ� �Ѵ�.
    m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Button_Apply")->subscribeEvent(
                        CEGUI::PushButton::EventClicked,    // Scheme�� ���� 'CEGUI:PushButton'�� ���� EventClicked �ൿ�� �����Ѵ�
                        CEGUI::Event::Subscriber(           // Ŭ���� �� ȣ���ϴ� ����� �����Ѵ�
                        &GameLoginWindow::Handle_ApplyButtonPressed,  // GameLoginWindow�� ��� Handle_ApplyButtonPressed ȣ��
                        this));                             // ���� ������� ������

	// Quit ��ư ���
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Button_Quit")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLoginWindow::Handle_QuitButtonPressed,
		this));
	
	// Join ��ư ���
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Button_Join")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLoginWindow::Handle_JoinButtonPressed,
		this));
	

	// ID�� Password �Է�â�� ���� �̺�Ʈ�� �����Ѵ�

	// ID �Է�â���� Enter Ű�� ���� ���
    m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->subscribeEvent(CEGUI::Editbox::EventTextAccepted,
                        CEGUI::Event::Subscriber(&GameLoginWindow::Handle_IDSubmitted,this));

	// Password �Է�â���� Enter Ű�� ���� ���
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->subscribeEvent(CEGUI::Editbox::EventTextAccepted,
                        CEGUI::Event::Subscriber(&GameLoginWindow::Handle_PWSubmitted,this));
	
	//// Password �Է�â���� ���ڸ� �Է��� ���
	//m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Edit_PW")->subscribeEvent(CEGUI::Editbox::EventKeyDown,
	//					CEGUI::Event::Subscriber(&GameLoginWindow::Handle_PWKeyDown, this));
}

bool GameLoginWindow::Handle_IDSubmitted(const CEGUI::EventArgs &e)
{
    // ��ü EventArg �����Ѵ�. ���ѵ� �뵵�� ������ ������ �� �̺�Ʈ�� ȣ���� �����쿡 ����
	// �������� �׼��� �� �� �ְ� �ȴ�.
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
    // ��ü EventArg ����
    const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);

    return true;
}

bool GameLoginWindow::Handle_ApplyButtonPressed(const CEGUI::EventArgs &e)
{
    return Login();
}

bool GameLoginWindow::Handle_QuitButtonPressed(const CEGUI::EventArgs &e)
{
	// ���� ����

	//m_LoginWindow->setVisible(false);
	GetGame()->GameQuit();

    return true;
}

void GameLoginWindow::setVisible(bool visible)
{
	// �α��� â�� ȭ�鿡 ǥ���ϰų� ����
    m_LoginWindow->setVisible(visible);
    
	// ���� â�� ���¸� ������
	m_bLogin = visible;

	// ID �Է�â�� Ȱ��ȭ(�Է� ������ ����)�Ѵ�
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->activate();

	// �α��� ���� â�� �α��� ���� â, ���� â�� �⺻������ ���� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->setVisible(false);
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(false);
}
 
bool GameLoginWindow::isVisible()
{
	// ���� â�� ǥ�� ���¸� �˷��ش�
    return m_LoginWindow->isVisible();
}

bool GameLoginWindow::Handle_ErrorOKButtonPressed(const CEGUI::EventArgs &e)
{
	// �α��� ���� â�� �����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->setVisible(false);

	// ID�� Password �Է�â�� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->setText("");

	// ID �Է�â�� Ȱ��ȭ(�Է� ������ ����)�Ѵ�
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->activate();
	return true;
}

void GameLoginWindow::ShowErrorWindow(STATUS s)
{
	// ���� â ǥ��
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->setVisible(true);
	//�� â�� ȭ�� �� �տ� ǥ��
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->moveToFront();
	// ���� �޽��� ���
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
	// ���� â���� OK ��ư�� ������ ��� �߻��ϴ� �̺�Ʈ �ڵ��� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Error")->getChild(sNamePrefix + "LoginWindow/Error/Button_OK")->subscribeEvent(CEGUI::PushButton::EventClicked,
                    CEGUI::Event::Subscriber(&GameLoginWindow::Handle_ErrorOKButtonPressed,this));
}

bool GameLoginWindow::Login()
{
	//ID�� Password �Է¿� ������ ���� ��� �α��� ���� â�� ǥ���ϰ�
	//�̰��� �Է¹��� ID�� Password�� ������ �����Ͽ� DB�� ����Ǿ� �ִ� �����Ϳ� �� ��
	//��ġ�ϴ� �������� ��� �α����� ������ �˸��� â�� ���� ���� �κ�� �����ϸ�
	//��ġ���� ���� ��� ����â�� ���� ���� ������ �˸�.

	CLoginServer* p = (CLoginServer*)GetGame()->GetServerMain()->GetNowServer();

	// �Է¹��� ID�� Password�� ������
    CEGUI::String ID = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->getText();
	CEGUI::String PW = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->getText();
	
	// ID�� Password �Է�â�� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Login")->getChild(sNamePrefix + "LoginWindow/Login/Edit_PW")->setText("");

	// ID �Ǵ� Password â�� �Է��� ���� ��� ���� â�� ����
	if(ID == "" || PW == "")
	{
		ShowErrorWindow(IDPW);
		return false;
	}

	// �α��� ���� â ǥ��
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(true);
	SetMessage(WAIT);
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild("LoginWindow/Progress/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

	p->CMD_Login(ID.c_str(), PW.c_str());

	return true;
}

bool GameLoginWindow::Handle_JoinButtonPressed(const CEGUI::EventArgs &e)
{
	// ���� â ǥ��
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(true);

	// // ID �Է�â�� Ȱ��ȭ(�Է� ������ ����)�Ѵ�
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->activate();

	// ���� â���� OK ��ư�� ������ ��� �߻��ϴ� �̺�Ʈ �ڵ��� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Button_OK")->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&GameLoginWindow::Handle_JoinOKButtonPressed,this));

	// ���� â���� Cancel ��ư�� ������ ��� �߻��ϴ� �̺�Ʈ �ڵ��� ����
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
	// ID�� Password �Է�â�� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_PW")->setText("");

	// ���� â�� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(false);

	return true;
}

bool GameLoginWindow::Join()
{
	CLoginServer* p = (CLoginServer*)GetGame()->GetServerMain()->GetNowServer();

	// �Է¹��� ID�� Password�� ������
	CEGUI::String ID = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->getText();
	CEGUI::String PW = m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_PW")->getText();
	
	// ID�� Password �Է�â�� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_ID")->setText("");
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->getChild(sNamePrefix + "LoginWindow/Join/Edit_PW")->setText("");

	// ID �Ǵ� Password â�� �Է��� ���� ��� ���� â�� ����
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
	// ���� â�� ����
	m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);

	return true;
}

void GameLoginWindow::SetMessage(STATUS s)
{
	// �� ���¿� �´� �޽����� �����Ѵ�
	switch(s)
	{
	case IDPW:
		sMessage = L"���̵�� �н����带 Ȯ���� �ּ���!";
		break;
	case WAIT:
		sMessage = L"��� ��ٷ� �ּ���...";
		break;
	case LOGINSUCCESS:
		sMessage = L"�α��� ����!";
		break;
	case LOGINFAILED:
		sMessage = L"�α��� ����!";
		break;
	case JOINSUCCESS:
		sMessage = L"���� ����!";
		break;
	case JOINFAILED:
		sMessage = L"���� ����!";
		break;
	}
}

void GameLoginWindow::ShowProgressWindow(STATUS s)
{
	// �� ���¿� �´� ����â�� ǥ���Ѵ�
	switch(s)
	{
	case LOGINSUCCESS:
		// �α��� ���� �޽���
		SetMessage(LOGINSUCCESS);
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild("LoginWindow/Progress/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

		// �α��� â�� ����
		m_LoginWindow->setVisible(false);
		GetGame()->GoNext(SS_TYPE::SS_LOBBY);
		break;
	case JOINSUCCESS:
		// ���� â�� ����
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Join")->setVisible(false);

		// ���� ���� �޽���
		SetMessage(JOINSUCCESS);
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild("LoginWindow/Progress/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

		// ���� â���� OK ��ư�� ������ ��� �߻��ϴ� �̺�Ʈ �ڵ��� ����
		m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->getChild(sNamePrefix + "LoginWindow/Progress/Button_OK")->subscribeEvent(CEGUI::PushButton::EventClicked,
						CEGUI::Event::Subscriber(&GameLoginWindow::Handle_ProgressOKButtonPressed,this));
		break;
	case WAIT:
		// ���� â ǥ��
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
		case LP_TYPE::LP_CREATEACCOUNT_SUCC :			//�������� ����
			{
				ShowProgressWindow(STATUS::JOINSUCCESS);
			}
			break;
		case LP_TYPE::LP_CREATEACCOUNT_FAIL :			//�������� ����
			{
				ShowProgressWindow(STATUS::JOINFAILED);
				m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);
				ShowErrorWindow(JOINFAILED);
			}
			break;
		case LP_TYPE::LP_LOGIN_SUCC :			//�α��� ����
			{
				ShowProgressWindow(STATUS::LOGINSUCCESS);
			}
			break;
		case LP_TYPE::LP_LOGIN_FAIL :			//�α��� ����
			{
				ShowProgressWindow(STATUS::LOGINFAILED);
				m_LoginWindow->getChild(sNamePrefix + "LoginWindow/Progress")->setVisible(false);
				// ���� â ǥ��
				ShowErrorWindow(LOGINFAILED);
			}
			break;
		}
	}
	return false;
}