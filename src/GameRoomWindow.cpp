#include "stdafx.h"
#include "..\include\GameRoomWindow.h"
#include "..\include\Game.h"
#include "..\server\CServerMain.h"
#include "..\server\LobbyServer.h"

int GameRoomWindow::iInstanceNumber;

GameRoomWindow::GameRoomWindow(Game* pParent) : CGameState(pParent)
{
	m_RoomWindow = NULL;
	iInstanceNumber = 0;
	sNamePrefix = "";
	mCharacter = ROOM_CHARACTER_WALKER;
	mReady = false;
	mBangjang = true;
	mType = true;
	mGameType = GAME_TEAM;
	mMap = MAP_1;
	mInput = false;
	sMessage = "";
	CreateCEGUIWindow();
	setVisible(true);
	m_bRoom = false;
	m_RoomWindow->setZOrderingEnabled(true);
	m_RoomWindow->moveToBack();
}

GameRoomWindow::~GameRoomWindow(void)
{
}

void GameRoomWindow::CreateCEGUIWindow()
{
	// CEGUI ������ �������� ���� �����͸� �����´�. ������ �۾��� ���ϰ� �ϱ� �����̴�.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
 
    // �浹 ���� �ν��Ͻ� ���� �ø��� ���ؼ� ���� 1�� ������Ų��
	// ���� ������� ��� ���������� ���� ������� ��� �ʿ� ���� �����̴�.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // �κ� â�� ����ִ� layout ������ ����ϰ� sNamePrefix ��� �̸����� �����Ѵ�
	// ���� ����ڰ� ���� ��� ���������� ���� ������� ��� �ʿ����.
	m_RoomWindow = pWindowManager->loadWindowLayout("room.layout",sNamePrefix);
 
    // �κ� �����찡 ����� �����Ǿ����� Ȯ���Ѵ�
    if (m_RoomWindow)
    {
        // Root GUI �����쿡 �츮�� ���� ������ â�� �߰��Ѵ�
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_RoomWindow);
		// �̺�Ʈ ó���� ��� (Ŭ��, Ÿ����, etc)
        (this)->RegisterHandlers();
		//ShowPlayerImage(mCharacter, mReady);
		//m_LoginWindow->setUserData(this);
    }
    else
    {
        // ���������� �����찡 �������� ���� ��� �̺�Ʈ �α׿� ���������� ����Ѵ�.
        CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the LoginWindow from .layout");
    }
}

void GameRoomWindow::setVisible(bool visible)
{
	// �κ� �����츦 ȭ�鿡 ǥ���ϰų� ����
    m_RoomWindow->setVisible(visible);
    
	// ���� â�� ���¸� ������
	m_bRoom = visible;

	//ShowSubCharacterImage(mCharacter);
	if(visible)
	{
		CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();

		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player1")->setProperty("Image", "set:Room image:Player_Back");		// ��� ĳ���� �̹��� ����
		mCharacter = ROOM_CHARACTER_WALKER;
		//p->CMD_ROOM_Change_Job(mCharacter - ROOM_CHARACTER );																		// ���� �÷��̾ ������ ĳ���͸� ��û��
		p->CMD_ROOM_GetFirstInfo();																									// �� ������ �ǰ� ����(Ŭ�� �ȿ��� ���ϴ°�) ���� �� ������ �����޶�� ��û��.
	}
}

bool GameRoomWindow::isVisible()
{
	// ���� â�� ǥ�� ���¸� �˷��ش�
    return m_RoomWindow->isVisible();
}

void GameRoomWindow::RegisterHandlers()
{
	// ���� ĳ���� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player")
		->getChild(sNamePrefix + "Room/Main/Player/Button_Prev")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ChaPrevButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ���� ĳ���� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player")
		->getChild(sNamePrefix + "Room/Main/Player/Button_Next")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ChaNextButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// �غ�Ϸ� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
		->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ReadyButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ������ ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Exit")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ExitButtonPressed,
		this));

	// ���� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Change")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_RoomChangeButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// Ȯ�� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Confirm")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ConfirmButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ��� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Cancel")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_CancelButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
	// ä�� ���� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Button_Send")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_SendButtonPressed,
		this));

	// ä��â���� ���� Ű�� ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")
		->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_TextSubmitted,
		this));
}

bool GameRoomWindow::Handle_ChaPrevButtonPressed(const CEGUI::EventArgs &e)
{
	// �غ� ������ ��� �۵����� ����
	//if(mReady)
	//	return false;

	// ���� ĳ���͸� ǥ��
	if(mCharacter - 1 < ROOM_CHARACTER_WALKER)
		mCharacter = ROOM_CHARACTER_PSYKER;
	else
		mCharacter--;

	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
	p->CMD_ROOM_Change_Job(mCharacter - ROOM_CHARACTER );

	//ShowPlayerImage(mCharacter);
	return true;
}

bool GameRoomWindow::Handle_ChaNextButtonPressed(const CEGUI::EventArgs &e)
{
	// �غ� ������ ��� �۵����� ����
	//if(mReady)
	//	return false;

	// ���� ĳ���͸� ǥ��
	if(mCharacter + 1 > ROOM_CHARACTER_PSYKER)
		mCharacter = ROOM_CHARACTER_WALKER;
	else
		mCharacter++;

	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
	p->CMD_ROOM_Change_Job(mCharacter - ROOM_CHARACTER);

	//ShowPlayerImage(mCharacter);
	return true;
}

void GameRoomWindow::ShowPlayerImage(int ch, bool ready)
{
	// �÷��̾� â�� �̹��� ǥ��(ĳ����, �غ� ���� ��)
	switch(ch)
	{
		case ROOM_CHARACTER_WALKER:
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player")
				->setProperty("Image", "set:Walker image:Full");

			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player0")
				->setProperty("Image", "set:Walker image:Chest");
			break;

		case ROOM_CHARACTER_GUNNER:
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player")
				->setProperty("Image", "set:Gunner image:Full");

			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player0")
				->setProperty("Image", "set:Gunner image:Chest");

			break;

		case ROOM_CHARACTER_PSYKER:
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player")
				->setProperty("Image", "set:Psyker image:Full");

			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player0")
				->setProperty("Image", "set:Psyker image:Chest");

			break;
		default:
			break;
	}
	SetExampleImage(ch);
	if(ready)
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player0")
			->getChild(sNamePrefix + "Room/Main/Player0/Ready")
			->setVisible(true);
	}
	else
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player0")
			->getChild(sNamePrefix + "Room/Main/Player0/Ready")
			->setVisible(false);
	}
}

void GameRoomWindow::ShowEnemyImage(int ch, bool ready)
{
	// �� â�� �̹��� ǥ��(ĳ����, �غ� ���� ��)

	switch(ch)
	{
		case ROOM_CHARACTER_WALKER:
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player1")
				->setProperty("Image", "set:Walker image:Chest");
			break;

		case ROOM_CHARACTER_GUNNER:
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player1")
				->setProperty("Image", "set:Gunner image:Chest");
			break;

		case ROOM_CHARACTER_PSYKER:
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player1")
				->setProperty("Image", "set:Psyker image:Chest");
			break;
		default:
			break;
	}

	if(ready)
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player1")
			->getChild(sNamePrefix + "Room/Main/Player1/Ready")
			->setVisible(true);
	}
	else
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player1")
			->getChild(sNamePrefix + "Room/Main/Player1/Ready")
			->setVisible(false);
	}
}

void GameRoomWindow::SetExampleImage(int ch)
{
	switch(ch)
	{
	case ROOM_CHARACTER_WALKER:
		{
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/example")
				->setProperty("Image", "set:character_example image:walker");
		}
		break;
	case ROOM_CHARACTER_GUNNER:
		{
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/example")
				->setProperty("Image", "set:character_example image:gunner");
		}
		break;
	case ROOM_CHARACTER_PSYKER:
		{
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/example")
				->setProperty("Image", "set:character_example image:psyker");
		}
		break;
	default:
		{

		}
		break;
	}
}

bool GameRoomWindow::Handle_ReadyButtonPressed(const CEGUI::EventArgs &e)
{
	if(mBangjang)
	{
		// ���� ����
		CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
		p->CMD_ROOM_Start();
	//	SoundMan::getSingletonPtr()->playSound("button");
	}
	else
	{
		// �غ� ���� ����
		if(!mReady)
			mReady = true;
		else
			mReady = false;
	}

	return true;
}

bool GameRoomWindow::Handle_ExitButtonPressed(const CEGUI::EventArgs &e)
{
	// �غ� ������ ��� �۵����� ����
	//if(mReady)
	//	return false;

	// ROOM -> LOBBY
	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
	p->CMD_ROOM_EXIT();																	// �� ���� ��Ŷ ����

	GetGame()->GoNext(SS_TYPE::SS_LOBBY);												// ���� ���� ����

	return true;
}

bool GameRoomWindow::Handle_RoomChangeButtonPressed(const CEGUI::EventArgs &e)
{
	// ������ �ƴ� ��� �۵����� ����
	if(!mBangjang)
		return false;

	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setVisible(true);
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->moveToFront();
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setAlpha(0.5);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setEnabled(false);

	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Name")->setText("");
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Name")->activate();
	return true;
}

bool GameRoomWindow::Handle_ConfirmButtonPressed(const CEGUI::EventArgs &e)
{
	// �� ���� �Է� Ȯ��
	if(m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Name")
		->getText() == "")
	{
		SetMessage(ROOM_STATUS_NAME);
		return false;
	}

	// ���� â �ݱ�
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setVisible(false);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setEnabled(true);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setAlpha(1.0);

	return true;
}

bool GameRoomWindow::Handle_CancelButtonPressed(const CEGUI::EventArgs &e)
{
	// ��� ��ư Ŭ�� �� â ���߱�
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setVisible(false);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setEnabled(true);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setAlpha(1.0);
	return true;
}

void GameRoomWindow::SetMessage(ROOM s)
{
	// �޽��� â�� ǥ���ϰ� �ٸ� â���� ���İ� ���� �� ��Ȱ��ȭ
	m_RoomWindow->getChild(sNamePrefix + "Room/Message")->setVisible(true);
	m_RoomWindow->getChild(sNamePrefix + "Room/Message")->moveToFront();
	
	if(m_RoomWindow->getChild(sNamePrefix + "Room/Change")->isVisible() && m_RoomWindow->getChild(sNamePrefix + "Room/Main")->isVisible())
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setAlpha(0.5);
		m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setEnabled(false);
	}
	else
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setAlpha(0.5);
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setEnabled(false);
	}
	
	// �� ���¿� �´� �޽����� �����Ѵ�
	switch(s)
	{
	case ROOM_STATUS_NAME:
		sMessage = L"���� ������ �ּ���!";
		break;
	case ROOM_STATUS_CHANGEFAILED:
		sMessage = L"�� ������ �Է��� �ּ���!";
		break;
	case ROOM_STATUS_NOREADY:
		sMessage = L"���� ����!";
		break;
	}

	// �޽��� ���� ǥ��
	m_RoomWindow->getChild(sNamePrefix + "Room/Message")
		->getChild(sNamePrefix + "Room/Message/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

	// Ȯ�� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_RoomWindow->getChild(sNamePrefix + "Room/Message")->getChild(sNamePrefix + "Room/Message/Button_OK")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_MessageOKButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
}

bool GameRoomWindow::Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e)
{
	// �޽��� â�� ���߰� ������ â���� ���ư���
	m_RoomWindow->getChild(sNamePrefix + "Room/Message")->setVisible(false);

	if(m_RoomWindow->getChild(sNamePrefix + "Room/Change")->isVisible() && m_RoomWindow->getChild(sNamePrefix + "Room/Main")->isVisible())
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setAlpha(1.0);
		m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setEnabled(true);
		return true;
	}
	else
	{
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setAlpha(1.0);
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setEnabled(true);
	}

	return true;
}

bool GameRoomWindow::Handle_SendButtonPressed(const CEGUI::EventArgs &e)
{
	// ä��â���� ���� ��ư�� Ŭ���� ���

	CEGUI::String Msg = m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->getText();

	(this)->ParseText(Msg);

	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->setText("");

	return true;;
}

bool GameRoomWindow::Handle_TextSubmitted(const CEGUI::EventArgs &e)
{
	// ä��â���� Return Ű�� �Է��� ���
	CEGUI::String Msg = m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->getText();
	
	(this)->ParseText(Msg);

	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->setText("");

	return true;;
}

void GameRoomWindow::ParseText(CEGUI::String inMsg)
{
	// �ؽ�Ʈ �Ľ�(��ɾ� �� ó��)
 
	std::string inString = inMsg.c_str();
 
	if (inString.length() >= 1) 
	{
		if (inString.at(0) == '/') 
		{
			std::string::size_type commandEnd = inString.find(" ", 0);
			std::string command = inString.substr(1, commandEnd - 1);
			std::string commandArgs = inString.substr(commandEnd + 1, inString.length() - (commandEnd + 1));
			
			for(std::string::size_type i=0; i < command.length(); i++)
			{
				command[i] = tolower(command[i]);
			}
 
 
			if (command == "say")
			{
				std::string outString = "You:" + inString;
								OutputText((CEGUI::utf8*)outString.c_str());
			}
			else if (command == "quit")
			{
			}
			else if (command == "help")
			{
			}
			else if (command == "b")
			{
				Ogre::UTFString mStatus;
				// �÷��̾� �������
				if(!mBangjang)
				{
					mBangjang = true;
					mStatus = L"����";
					m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
						->setText((CEGUI::utf8*)mStatus.asUTF8_c_str());
				}
				else
				{
					mBangjang = false;
					mStatus = L"�غ�Ϸ�";
					m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
						->setText((CEGUI::utf8*)mStatus.asUTF8_c_str());
				}

			}
			else
			{
				std::string outString = "<" + inString + "> is an invalid command.";
				(this)->OutputText((CEGUI::utf8*)outString.c_str(),CEGUI::colour(1.0f,0.0f,0.0f));
			}
		}
		else
		{
	 (this)->OutputText((CEGUI::utf8*)inString.c_str());
		}
	} 
}

void GameRoomWindow::OutputText(CEGUI::String inMsg, CEGUI::colour colour)
{
	// �Էµ� ������ ä��â�� ���

	CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(m_RoomWindow->getChild(sNamePrefix + "Room/Main")
		->getChild(sNamePrefix + "Room/Main/Chat")->getChild(sNamePrefix + "Room/Main/Chat/List"));
 
	CEGUI::ListboxTextItem *newItem=0;
 
	newItem = new CEGUI::ListboxTextItem(inMsg,CEGUI::HTF_WORDWRAP_LEFT_ALIGNED); 
                                                                                               
	newItem->setTextColours(colour);
	outputWindow->addItem(newItem);
	outputWindow->ensureItemIsVisible(newItem);
}

void GameRoomWindow::ChangeRoomInfo(stRoom_RoomInfo* pRoomInfo)
{
	Ogre::UTFString mRoomName;
	Ogre::UTFString mMasterName;
	int nNum;

	if(pRoomInfo->szMasterName == GetPlayerName())
	{
		mBangjang = true;
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
			->setProperty("NormalImage", "set:Room image:Start_Normal");
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
			->setProperty("PushedImage", "set:Room image:Start_Push");
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
			->setProperty("HoverImage", "set:Room image:Start_Hover");
	}
	else
	{
		mBangjang = false;
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
			->setProperty("NormalImage", "set:Room image:Ready_Normal");
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
			->setProperty("PushedImage", "set:Room image:Ready_Push");
		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
			->setProperty("HoverImage", "set:Room image:Ready_Hover");
	}

	mRoomName = pRoomInfo->szRoomName;
	nNum = pRoomInfo->nRoomNum;
	mMasterName = pRoomInfo->szMasterName;

	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Title_Text")
		->setText((CEGUI::utf8*)mRoomName.asUTF8_c_str());
}

void GameRoomWindow::ChangePlayerInfo(stRoom_PlayerInfo* pPlayerInfo)
{
	if(pPlayerInfo->name == GetPlayerName())					// �÷��̾� ������ ���
	{
		ShowPlayerImage(pPlayerInfo->jobnum + ROOM_CHARACTER, pPlayerInfo->ready);
	}
	else
	{
		ShowEnemyImage(pPlayerInfo->jobnum + ROOM_CHARACTER, pPlayerInfo->ready);
	}

}

void GameRoomWindow::keyPressed(const OIS::KeyEvent &_arg)
{
	// Ű �Է¿� ���� ó��

	if(!m_RoomWindow->isVisible())
		return;

	if(_arg.key == OIS::KC_RETURN)
	{
		// Return Ű �Է�

		if(!mInput)
		{
			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
				->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->activate();

			mInput = true;
		}
		else
		{
			CEGUI::String Msg = m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
				->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->getText();
	
			(this)->ParseText(Msg);

			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
				->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->setText("");

			m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
				->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->deactivate();

			mInput = false;
		}
	}
}

void GameRoomWindow::keyReleased(const OIS::KeyEvent &_arg)
{

}

bool GameRoomWindow::CheckData()
{
	while(!m_RoomQueue.empty())
	{
		stRoom stroom = m_RoomQueue.front();
		m_RoomQueue.pop();
		switch(stroom.type)
		{
		case BS_TYPE::BS_ROOM_EXIT :
			{

			}
			break;
		case BS_TYPE::BS_CHANGE_INFO :
			{

			}
			break;
		case BS_TYPE::BS_CHANGE_PLAYER :
			{

			}
			break;
		case BS_TYPE::BS_ROOM_READY :
			{

			}
			break;
		case BS_TYPE::BS_ROOM_START :
			{
				//�� ������ ���߱�
				m_RoomWindow->setVisible(false);
				GetGame()->GoNext(SS_TYPE::SS_GAME);
			}
			break;
		}
	}

	while(!m_RoomQueue_PlayerInfo.empty())		//�÷��̾� ������ �ٲ���Ҷ�.
	{
		stRoom_PlayerInfo stPlayer = m_RoomQueue_PlayerInfo.front();
		m_RoomQueue_PlayerInfo.pop();

		ChangePlayerInfo(&stPlayer);
	}
	
	while(!m_RoomQueue_RoomInfo.empty())		//�� ������ �ٲ���Ҷ�. �濡 ó�� ���� ������
	{
		stRoom_RoomInfo stroom = m_RoomQueue_RoomInfo.front();
		m_RoomQueue_RoomInfo.pop();

		ChangeRoomInfo(&stroom);
	}
	return true;
}

void GameRoomWindow::PushData(BS_TYPE type)
{
	stRoom stroom;
	stroom.type = type;

	m_RoomQueue.push(stroom);
}

void GameRoomWindow::PushData(int nNum, std::string szName, int nJobNum, bool bReady)
{
	stRoom_PlayerInfo stroom;
	stroom.num = nNum;
	stroom.name = szName;
	stroom.jobnum = nJobNum;
	stroom.ready = bReady;

	m_RoomQueue_PlayerInfo.push(stroom);
}

void GameRoomWindow::PushData(std::string szMasterName, std::string szRoomName, int nRoomNum, int nRoomCurr, int nRoomMax, int nMapNum)
{
	stRoom_RoomInfo stroom;
	stroom.szMasterName = szMasterName;
	stroom.szRoomName = szRoomName;
	stroom.nRoomNum = nRoomNum;
	stroom.nRoomCurr = nRoomCurr;
	stroom.nRoomMax = nRoomMax;
	stroom.nMapNum = nMapNum;
	m_RoomQueue_RoomInfo.push(stroom);

}