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
	// CEGUI 윈도우 관리자의 로컬 포인터를 가져온다. 순전히 작업을 편하게 하기 위함이다.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
 
    // 충돌 없이 인스턴스 수를 늘리기 위해서 값을 1씩 증가시킨다
	// 다중 사용자일 경우 유용하지만 단일 사용자일 경우 필요 없는 변수이다.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // 로비 창이 들어있는 layout 파일을 등록하고 sNamePrefix 라는 이름으로 지정한다
	// 다중 사용자가 있을 경우 유용하지만 단일 사용자일 경우 필요없다.
	m_LobbyWindow = pWindowManager->loadWindowLayout("lobby.layout",sNamePrefix);
 
    // 로비 윈도우가 제대로 생성되었는지 확인한다
    if (m_LobbyWindow)
    {
        // Root GUI 윈도우에 우리가 새로 생성한 창을 추가한다
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_LobbyWindow);
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

void GameLobbyWindow::setVisible(bool visible)
{
	// 로비 윈도우를 화면에 표시하거나 감춤
    m_LobbyWindow->setVisible(visible);
    
	// 현재 창의 상태를 저장함
	m_bLobby = visible;
}

bool GameLobbyWindow::isVisible()
{
	// 현재 창의 표시 상태를 알려준다
    return m_LobbyWindow->isVisible();
}

void GameLobbyWindow::RegisterHandlers()
{
	// 입장하기 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Button_Entry")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_EntryButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 방만들기 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Button_Create")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_CreateButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 나가기 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Button_Exit")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_ExitButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 방0 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room0")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_Room0ButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 방1 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room1")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_Room1ButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 방2 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Room2")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_Room2ButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 이전 방 리스트 클릭 이벤트
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Prev")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_PrevButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 다음 방 리스트 클릭 이벤트
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Next")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_NextButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
	
	// 새로고침 클릭 이벤트
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/List")
		->getChild(sNamePrefix + "Lobby/Main/List/Refresh")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_RefreshButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 확인 클릭 이벤트
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Confirm")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_ConfirmButtonPressed, this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 취소 클릭 이벤트
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Cancel")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_CancelButtonPressed, this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 채팅 전송 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Button_Send")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_SendButtonPressed,
		this));

	// 채팅창에서 엔터 키를 누를 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Main")->getChild(sNamePrefix + "Lobby/Main/Chat")
		->getChild(sNamePrefix + "Lobby/Main/Chat/Edit_Input")
		->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_TextSubmitted,
		this));
}

bool GameLobbyWindow::Handle_EntryButtonPressed(const CEGUI::EventArgs &e)
{
	// 방을 선택하지 않은 상태인 경우 메시지 표시
	if(mSelect == -1)
	{
		SetMessage(SELECT);
		return false;
	}

	// 방 입장
	Entry(mSelect);

	return true;
}

bool GameLobbyWindow::Handle_CreateButtonPressed(const CEGUI::EventArgs &e)
{
	//방 만들기 버튼을 클릭할 경우 새 방을 만드는 윈도우를 띄운다
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
	// 나가기 버튼을 누를 경우 게임 종료
	GetGame()->GameQuit();

	return true;
}

void GameLobbyWindow::SetMessage(LOBBY_STATUS s)
{
	// 메시지 창을 표시하고 다른 창들은 알파값 조정 및 비활성화
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
	
	// 각 상태에 맞는 메시지를 설정한다
	switch(s)
	{
	case SELECT:
		sMessage = L"방을 선택해 주세요!";
		break;
	case NONAME:
		sMessage = L"방 제목을 입력해 주세요!";
		break;
	case CREATEFAILED:
		sMessage = L"방 생성 실패!";
		break;
	case ENTRYFAILED:
		sMessage = L"입장 실패!";
		break;
	case FULL:
		sMessage = L"인원이 가득 찼습니다!";
		break;
	}

	// 메시지 내용 표시
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Message")
		->getChild(sNamePrefix + "Lobby/Message/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

	// 확인 버튼을 클릭할 경우 이벤트 발생
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/Message")->getChild(sNamePrefix + "Lobby/Message/Button_OK")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameLobbyWindow::Handle_MessageOKButtonPressed,
		this));
}

bool GameLobbyWindow::Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e)
{
	// 메시지 창을 감추고 이전의 창으로 돌아간다
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
	// 선택한 방을 다시 선택할 경우
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
	// 입장 가능할 경우 윈도우 변경(로비->룸)
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
	//서버에서 방 리스트를 새로 가져옴
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
	//방 리스트 갱신
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
		return L"광장";
		break;
	case MAP_2:
		return L"시청";
		break;
	default:
		return L"알수없음";
		break;
	}
}

Ogre::UTFString GameLobbyWindow::getType(int type)
{
	switch(type)
	{
	case GAME_TEAM:
		return L"팀전";
		break;
	case GAME_INDI:
		return L"개인전";
		break;
	default:
		return L"알수없음";
		break;
	}
}

bool GameLobbyWindow::Handle_ConfirmButtonPressed(const CEGUI::EventArgs &e)
{
	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();

	// 방 제목 입력 확인
	CEGUI::String RoomName = m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->getChild(sNamePrefix + "Lobby/New/Name")->getText();
	if(RoomName == "")
	{
		SetMessage(NONAME);
		return false;
	}

	// 방 생성 내용 서버 전송
	
	//추가 요망
	UINT mapNum = MAP_1;		//맵 번호
	UINT maxPlayer = 2;			//최대 인원

	p->CMD_LOBBY_CreateRoom(mapNum, maxPlayer, RoomName.c_str());


	// 방 생성 윈도우 감추기
	m_LobbyWindow->getChild(sNamePrefix + "Lobby/New")->setVisible(false);

	// 로비 윈도우 되돌리기
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
		case BS_TYPE::BS_ROOM_CREATE :						//방생성
			{
				
			}
			break;
		case BS_TYPE::BS_ROOM_CREATE_FAIL :					//방생성 실패
			{
				SetMessage(CREATEFAILED);
			}
			break;
		case BS_TYPE::BS_ROOM_JOIN :						//방입장
			{
				m_LobbyWindow->setVisible(false);
				GetGame()->GoNext(SS_TYPE::SS_ROOM);
				return true;
			}
			break;
		case BS_TYPE::BS_ROOM_JOIN_FAIL :					//방입장 실패
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