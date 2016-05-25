#include "stdafx.h"
#include "..\include\GameLobbyWindow.h"
#include "..\include\Game.h"
#include "..\server\CServerMain.h"
#include "..\server\LobbyServer.h"

int GameLobbyWindow::iInstanceNumber;

GameLobbyWindow::GameLobbyWindow(Game* pParent) : CGameState(pParent)
{
	m_LobbyWindow = NULL;
	iInstanceNumber = 0;
	sNamePrefix = "";
	sMessage = L"";
	sTemp = L"";
	mEntry = 1;
	mSelect = -1;
	mCurrPage = 0;
	mPage = 0;
	mFull = 0;
	mRoomSize = 0;

	CreateCEGUIWindow();
	Refresh();
	setVisible(true);
	m_bLobby = false;
}


GameLobbyWindow::~GameLobbyWindow(void)
{
}

void GameLobbyWindow::CreateCEGUIWindow()
{
	// CEGUI ������ �������� ���� �����͸� �����´�. ������ �۾��� ���ϰ� �ϱ� �����̴�.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
 
    // �浹 ���� �ν��Ͻ� ���� �ø��� ���ؼ� ���� 1�� ������Ų��
	// ���� ������� ��� ���������� ���� ������� ��� �ʿ� ���� �����̴�.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // �κ� â�� ����ִ� layout ������ ����ϰ� sNamePrefix ��� �̸����� �����Ѵ�
	// ���� ����ڰ� ���� ��� ���������� ���� ������� ��� �ʿ����.
	m_LobbyWindow = pWindowManager->loadWindowLayout("lobby.layout",sNamePrefix);
 
    // �κ� �����찡 ����� �����Ǿ����� Ȯ���Ѵ�
    if (m_LobbyWindow)
    {
        // Root GUI �����쿡 �츮�� ���� ������ â�� �߰��Ѵ�
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_LobbyWindow);
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

void GameLobbyWindow::setVisible(bool visible)
{
	// �κ� �����츦 ȭ�鿡 ǥ���ϰų� ����
    m_LobbyWindow->setVisible(visible);
    
	// ���� â�� ���¸� ������
	m_bLobby = visible;
}

bool GameLobbyWindow::isVisible()
{
	// ���� â�� ǥ�� ���¸� �˷��ش�
    return m_LobbyWindow->isVisible();
}

void GameLobbyWindow::RegisterHandlers()
{
	// �����ϱ� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Button_Entry")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_EntryButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// �游��� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Button_Create")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_CreateButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ������ ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Button_Exit")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_ExitButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ��0 ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room0")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_Room0ButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ��1 ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room1")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_Room1ButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ��2 ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room2")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_Room2ButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ���� �� ����Ʈ Ŭ�� �̺�Ʈ
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Prev")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_PrevButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ���� �� ����Ʈ Ŭ�� �̺�Ʈ
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Next")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_NextButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
	
	// ���ΰ�ħ Ŭ�� �̺�Ʈ
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Refresh")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_RefreshButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// Ȯ�� Ŭ�� �̺�Ʈ
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Confirm")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_ConfirmButtonPressed, this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ��� Ŭ�� �̺�Ʈ
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Cancel")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_CancelButtonPressed, this));
	SoundMan::getSingletonPtr()->playSound("button");

	// ä�� ���� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Button_Send")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_SendButtonPressed,
		this));

	// ä��â���� ���� Ű�� ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Edit_Input")
		->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_TextSubmitted,
		this));
}

bool GameLobbyWindow::Handle_EntryButtonPressed(const CEGUI::EventArgs &e)
{
	// ���� �������� ���� ������ ��� �޽��� ǥ��
	if(mSelect == -1)
	{
		SetMessage(SELECT);
		return false;
	}

	// �� ����
	Entry(mSelect);

	return true;
}

bool GameLobbyWindow::Handle_CreateButtonPressed(const CEGUI::EventArgs &e)
{
	//�� ����� ��ư�� Ŭ���� ��� �� ���� ����� �����츦 ����
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setVisible(true);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->moveToFront();
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setAlpha(0.5);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setEnabled(false);

	mMap = MAP_1;
	mType = GAME_INDI;
	
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Name")->setText("");
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Name")->activate();

	return true;
}

bool GameLobbyWindow::Handle_ExitButtonPressed(const CEGUI::EventArgs &e)
{
	// ������ ��ư�� ���� ��� ���� ����
	GetGame()->GameQuit();

	return true;
}

void GameLobbyWindow::SetMessage(LOBBY_STATUS s)
{
	// �޽��� â�� ǥ���ϰ� �ٸ� â���� ���İ� ���� �� ��Ȱ��ȭ
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Message")->setVisible(true);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Message")->moveToFront();
	
	if(m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->isVisible() && m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->isVisible())
	{
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setAlpha(0.5);
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setEnabled(false);
	}
	else
	{
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setAlpha(0.5);
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setEnabled(false);
	}
	
	// �� ���¿� �´� �޽����� �����Ѵ�
	switch(s)
	{
	case SELECT:
		sMessage = L"���� ������ �ּ���!";
		break;
	case NONAME:
		sMessage = L"�� ������ �Է��� �ּ���!";
		break;
	case CREATEFAILED:
		sMessage = L"�� ���� ����!";
		break;
	case ENTRYFAILED:
		sMessage = L"���� ����!";
		break;
	case FULL:
		sMessage = L"�ο��� ���� á���ϴ�!";
		break;
	}

	// �޽��� ���� ǥ��
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Message")
		->getChild(sNamePrefix + "Lobby/Message/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

	// Ȯ�� ��ư�� Ŭ���� ��� �̺�Ʈ �߻�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Message")->getChild(sNamePrefix + "Lobby/Message/Button_OK")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_MessageOKButtonPressed,
		this));
}

bool GameLobbyWindow::Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e)
{
	// �޽��� â�� ���߰� ������ â���� ���ư���
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Message")->setVisible(false);

	if(m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->isVisible() && m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->isVisible())
	{
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setAlpha(1.0);
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setEnabled(true);
		return true;
	}
	else
	{
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setAlpha(1.0);
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setEnabled(true);
	}

	return true;
}

bool GameLobbyWindow::Handle_Room0ButtonPressed(const CEGUI::EventArgs &e)
{
	// ������ ���� �ٽ� ������ ���
	if(mSelect == 0)
	{
		Entry(mSelect);
		return true;
	}

	if(mSelect != -1)
	{
		sTemp = "Lobby/Main/List/Room" + Ogre::StringConverter::toString(mSelect);
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + sTemp)->setAlpha(1.0);
	}
	mSelect = 0;
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room0")->setAlpha(0.5);

	return true;
}

bool GameLobbyWindow::Handle_Room1ButtonPressed(const CEGUI::EventArgs &e)
{
	if(mSelect == 1)
	{
		Entry(mSelect);
		return true;
	}

	if(mSelect != -1)
	{
		sTemp = "Lobby/Main/List/Room" + Ogre::StringConverter::toString(mSelect);
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + sTemp)->setAlpha(1.0);
	}
	mSelect = 1;
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room1")->setAlpha(0.5);

	return true;
}

bool GameLobbyWindow::Handle_Room2ButtonPressed(const CEGUI::EventArgs &e)
{
	if(mSelect == 2)
	{
		Entry(mSelect);
		return true;
	}

	if(mSelect != -1)
	{
		sTemp = "Lobby/Main/List/Room" + Ogre::StringConverter::toString(mSelect);
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + sTemp)->setAlpha(1.0);
	}
	mSelect = 2;
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room2")->setAlpha(0.5);

	return true;
}

void GameLobbyWindow::Entry(int select)
{
	// ���� ������ ��� ������ ����(�κ�->��)
	CEGUI::String szRoomNum = m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + "Lobby/Main/List/Room" + Ogre::StringConverter::toString(select))
			->getChild(sNamePrefix+ "Lobby/Main/List/Room" + Ogre::StringConverter::toString(select) + "/RoomNum")->getText();
	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
	p->CMD_LOBBY_JoinRoom(atoi(szRoomNum.c_str()));

}

bool GameLobbyWindow::Handle_PrevButtonPressed(const CEGUI::EventArgs &e)
{
	if(mCurrPage == 0)
		return false;

	mCurrPage--;
	Refresh();

	return true;
}

bool GameLobbyWindow::Handle_NextButtonPressed(const CEGUI::EventArgs &e)
{
	if(mCurrPage == mPage)
		return false;

	mCurrPage++;
	Refresh();

	return true;
}

bool GameLobbyWindow::Handle_RefreshButtonPressed(const CEGUI::EventArgs &e)
{
	Refresh();

	return true;
}

void GameLobbyWindow::Refresh()
{
	//�������� �� ����Ʈ�� ���� ������
	for(int i = 0; i<mRoomSize; i++)
	{
		sTemp = " ";
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + "Lobby/Main/List/Room" + Ogre::StringConverter::toString(i))
			->getChild(sNamePrefix+ "Lobby/Main/List/Room" + Ogre::StringConverter::toString(i) + "/RoomNum")->setText((CEGUI::utf8*)sTemp.asUTF8_c_str());
		m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + "Lobby/Main/List/Room" + Ogre::StringConverter::toString(i))->setText((CEGUI::utf8*)sTemp.asUTF8_c_str());
	}
	mRoomSize = 0;
	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
	p->CMD_LOBBY_GetRoomsInfo();
}

void GameLobbyWindow::Refresh_End(int i, room_list* roomList)
{
	//�� ����Ʈ ����
	sTemp = Ogre::StringConverter::toString(roomList->mRoomNo);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + "Lobby/Main/List/Room" + Ogre::StringConverter::toString(i))
			->getChild(sNamePrefix+ "Lobby/Main/List/Room" + Ogre::StringConverter::toString(i) + "/RoomNum")->setText((CEGUI::utf8*)sTemp.asUTF8_c_str());
	sTemp = roomList->sRoomName + "\n" +
		getMap(roomList->mRoomMap) + "\n" + Ogre::StringConverter::toString(roomList->mRoomCurr) + " / " + Ogre::StringConverter::toString(roomList->mRoomMax);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
			->getChild(sNamePrefix + "Lobby/Main/List/Room" + Ogre::StringConverter::toString(i))->setText((CEGUI::utf8*)sTemp.asUTF8_c_str());
}

Ogre::UTFString GameLobbyWindow::getMap(int map)
{
	switch(map)
	{
	case MAP_1:
		return L"����";
		break;
	case MAP_2:
		return L"��û";
		break;
	default:
		return L"�˼�����";
		break;
	}
}

Ogre::UTFString GameLobbyWindow::getType(int type)
{
	switch(type)
	{
	case GAME_TEAM:
		return L"����";
		break;
	case GAME_INDI:
		return L"������";
		break;
	default:
		return L"�˼�����";
		break;
	}
}

bool GameLobbyWindow::Handle_ConfirmButtonPressed(const CEGUI::EventArgs &e)
{
	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();

	// �� ���� �Է� Ȯ��
	CEGUI::String RoomName = m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Name")->getText();
	if(RoomName == "")
	{
		SetMessage(NONAME);
		return false;
	}

	// �� ���� ���� ���� ����
	
	//�߰� ���
	UINT mapNum = MAP_1;		//�� ��ȣ
	UINT maxPlayer = 2;			//�ִ� �ο�

	p->CMD_LOBBY_CreateRoom(mapNum, maxPlayer, RoomName.c_str());


	// �� ���� ������ ���߱�
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setVisible(false);

	// �κ� ������ �ǵ�����
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setEnabled(true);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setAlpha(1.0);


	return true;
}

bool GameLobbyWindow::Handle_CancelButtonPressed(const CEGUI::EventArgs &e)
{
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setVisible(false);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setEnabled(true);
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->setAlpha(1.0);
	return true;
}


bool GameLobbyWindow::Handle_SendButtonPressed(const CEGUI::EventArgs &e)
{
	CEGUI::String Msg = m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Edit_Input")->getText();

	(this)->ParseText(Msg);

	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Edit_Input")->setText("");

	return true;;
}

bool GameLobbyWindow::Handle_TextSubmitted(const CEGUI::EventArgs &e)
{
	CEGUI::String Msg = m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Edit_Input")->getText();
	
	(this)->ParseText(Msg);

	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Edit_Input")->setText("");

	return true;;
}

void GameLobbyWindow::ParseText(CEGUI::String inMsg)
{
 
	// I personally like working with std::string. So i'm going to convert it here.
	std::string inString = inMsg.c_str();
 
	if (inString.length() >= 1) // Be sure we got a string longer than 0
	{
		if (inString.at(0) == '/') // Check if the first letter is a 'command'
		{
			std::string::size_type commandEnd = inString.find(" ", 0);
			std::string command = inString.substr(1, commandEnd - 1);
			std::string commandArgs = inString.substr(commandEnd + 1, inString.length() - (commandEnd + 1));
			//convert command to lower case
			for(std::string::size_type i=0; i < command.length(); i++)
			{
				command[i] = tolower(command[i]);
			}
 
			// Begin processing
 
			if (command == "say")
			{
				std::string outString = "You:" + inString; // Append our 'name' to the message we'll display in the list
								OutputText((CEGUI::utf8*)outString.c_str());
			}
			else if (command == "quit")
			{
				// do a /quit 
			}
			else if (command == "help")
			{
				// do a /help
			}
			else
			{
				std::string outString = "<" + inString + "> is an invalid command.";
				(this)->OutputText((CEGUI::utf8*)outString.c_str(),CEGUI::colour(1.0f,0.0f,0.0f)); // With red ANGRY colors!
			}
		} // End if /
		else
		{
	 (this)->OutputText((CEGUI::utf8*)inString.c_str()); // no commands, just output what they wrote
		}
	} 
}

void GameLobbyWindow::OutputText(CEGUI::String inMsg, CEGUI::colour colour)
{
 
	// Get a pointer to the ChatBox so we don't have to use this ugly getChild function everytime.
	CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")
		->getChild(sNamePrefix + "Lobby/Main/Chat")->getChild(sNamePrefix + "Lobby/Main/Chat/List"));
 
	CEGUI::ListboxTextItem *newItem=0; // This will hold the actual text and will be the listbox segment / item
 
	newItem = new CEGUI::ListboxTextItem(inMsg,CEGUI::HTF_WORDWRAP_LEFT_ALIGNED); // Instance the Item with Left
                                                                                                   //   wordwrapped alignment
	newItem->setTextColours(colour); // Set the text color
	outputWindow->addItem(newItem); // Add the new ListBoxTextItem to the ListBox
	outputWindow->ensureItemIsVisible(newItem);
}

void GameLobbyWindow::PushData(int nRoomNum, int nNowCount, int nMaxCount, int nMapNum, bool bIsGame, std::string szMasterName, std::string szRoomName)
{
	room_list roomList;
	roomList.mRoomNo = nRoomNum;
	roomList.sRoomName = szRoomName;
	roomList.mRoomMap = nMapNum;
	roomList.mRoomCurr = nNowCount;
	roomList.mRoomMax = nMaxCount;

	Refresh_End(mRoomSize, &roomList);
	mRoomSize++;
}

void GameLobbyWindow::PushData(BS_TYPE type)
{
	stLobby stlobby;
	stlobby.type = type;

	m_LobbyQueue.push(stlobby);
}

bool GameLobbyWindow::CheckData()
{
	while(!m_LobbyQueue.empty())
	{
		stLobby stlobby = m_LobbyQueue.front();
		m_LobbyQueue.pop();
		switch(stlobby.type)
		{
		case BS_TYPE::BS_ROOM_CREATE :						//�����
			{
				
			}
			break;
		case BS_TYPE::BS_ROOM_CREATE_FAIL :					//����� ����
			{
				SetMessage(CREATEFAILED);
			}
			break;
		case BS_TYPE::BS_ROOM_JOIN :						//������
			{
				m_LobbyWindow->setVisible(false);
				GetGame()->GoNext(SS_TYPE::SS_ROOM);
				return true;
			}
			break;
		case BS_TYPE::BS_ROOM_JOIN_FAIL :					//������ ����
			{
				SetMessage(ENTRYFAILED);
			}
			break;
		}
	}
	return false;
}

void GameLobbyWindow::keyPressed(const OIS::KeyEvent &_arg)
{
	if(!m_LobbyWindow->isVisible())
		return;

	if(_arg.key == OIS::KC_F5)
	{
		Refresh();
	}
}