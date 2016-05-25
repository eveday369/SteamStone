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
	// CEGUI 윈도우 관리자의 로컬 포인터를 가져온다. 순전히 작업을 편하게 하기 위함이다.
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
 
    // 충돌 없이 인스턴스 수를 늘리기 위해서 값을 1씩 증가시킨다
	// 다중 사용자일 경우 유용하지만 단일 사용자일 경우 필요 없는 변수이다.
    sNamePrefix = ++iInstanceNumber + "_";
 
    // 로비 창이 들어있는 layout 파일을 등록하고 sNamePrefix 라는 이름으로 지정한다
	// 다중 사용자가 있을 경우 유용하지만 단일 사용자일 경우 필요없다.
	m_RoomWindow = pWindowManager->loadWindowLayout("room.layout",sNamePrefix);
 
    // 로비 윈도우가 제대로 생성되었는지 확인한다
    if (m_RoomWindow)
    {
        // Root GUI 윈도우에 우리가 새로 생성한 창을 추가한다
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(m_RoomWindow);
		// 이벤트 처리기 등록 (클릭, 타이핑, etc)
        (this)->RegisterHandlers();
		//ShowPlayerImage(mCharacter, mReady);
		//m_LoginWindow->setUserData(this);
    }
    else
    {
        // 정상적으로 윈도우가 생성되지 않을 경우 이벤트 로그에 오류사항을 기록한다.
        CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the LoginWindow from .layout");
    }
}

void GameRoomWindow::setVisible(bool visible)
{
	// 로비 윈도우를 화면에 표시하거나 감춤
    m_RoomWindow->setVisible(visible);
    
	// 현재 창의 상태를 저장함
	m_bRoom = visible;

	//ShowSubCharacterImage(mCharacter);
	if(visible)
	{
		CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();

		m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player1")->setProperty("Image", "set:Room image:Player_Back");		// 상대 캐릭터 이미지 삭제
		mCharacter = ROOM_CHARACTER_WALKER;
		//p->CMD_ROOM_Change_Job(mCharacter - ROOM_CHARACTER );																		// 현재 플레이어가 선택한 캐릭터를 요청함
		p->CMD_ROOM_GetFirstInfo();																									// 방 생성이 되고 나서(클라 안에서 말하는거) 최초 방 정보를 보내달라고 요청함.
	}
}

bool GameRoomWindow::isVisible()
{
	// 현재 창의 표시 상태를 알려준다
    return m_RoomWindow->isVisible();
}

void GameRoomWindow::RegisterHandlers()
{
	// 이전 캐릭터 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player")
		->getChild(sNamePrefix + "Room/Main/Player/Button_Prev")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ChaPrevButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 다음 캐릭터 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Player")
		->getChild(sNamePrefix + "Room/Main/Player/Button_Next")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ChaNextButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 준비완료 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
		->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ReadyButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 나가기 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Exit")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ExitButtonPressed,
		this));

	// 변경 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Change")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_RoomChangeButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 확인 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Confirm")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_ConfirmButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");

	// 취소 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Cancel")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_CancelButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
	// 채팅 전송 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Button_Send")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_SendButtonPressed,
		this));

	// 채팅창에서 엔터 키를 누를 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")
		->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_TextSubmitted,
		this));
}

bool GameRoomWindow::Handle_ChaPrevButtonPressed(const CEGUI::EventArgs &e)
{
	// 준비 상태일 경우 작동하지 않음
	//if(mReady)
	//	return false;

	// 이전 캐릭터를 표시
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
	// 준비 상태일 경우 작동하지 않음
	//if(mReady)
	//	return false;

	// 다음 캐릭터를 표시
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
	// 플레이어 창에 이미지 표시(캐릭터, 준비 여부 등)
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
	// 적 창에 이미지 표시(캐릭터, 준비 여부 등)

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
		// 게임 시작
		CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
		p->CMD_ROOM_Start();
	//	SoundMan::getSingletonPtr()->playSound("button");
	}
	else
	{
		// 준비 여부 전송
		if(!mReady)
			mReady = true;
		else
			mReady = false;
	}

	return true;
}

bool GameRoomWindow::Handle_ExitButtonPressed(const CEGUI::EventArgs &e)
{
	// 준비 상태일 경우 작동하지 않음
	//if(mReady)
	//	return false;

	// ROOM -> LOBBY
	CLobbyServer* p = (CLobbyServer*)GetGame()->GetServerMain()->GetNowServer();
	p->CMD_ROOM_EXIT();																	// 방 종료 패킷 전송

	GetGame()->GoNext(SS_TYPE::SS_LOBBY);												// 게임 상태 변경

	return true;
}

bool GameRoomWindow::Handle_RoomChangeButtonPressed(const CEGUI::EventArgs &e)
{
	// 방장이 아닐 경우 작동하지 않음
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
	// 방 제목 입력 확인
	if(m_RoomWindow->getChild(sNamePrefix + "Room/Change")->getChild(sNamePrefix + "Room/Change/Name")
		->getText() == "")
	{
		SetMessage(ROOM_STATUS_NAME);
		return false;
	}

	// 변경 창 닫기
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setVisible(false);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setEnabled(true);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setAlpha(1.0);

	return true;
}

bool GameRoomWindow::Handle_CancelButtonPressed(const CEGUI::EventArgs &e)
{
	// 취소 버튼 클릭 시 창 감추기
	m_RoomWindow->getChild(sNamePrefix + "Room/Change")->setVisible(false);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setEnabled(true);
	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->setAlpha(1.0);
	return true;
}

void GameRoomWindow::SetMessage(ROOM s)
{
	// 메시지 창을 표시하고 다른 창들은 알파값 조정 및 비활성화
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
	
	// 각 상태에 맞는 메시지를 설정한다
	switch(s)
	{
	case ROOM_STATUS_NAME:
		sMessage = L"방을 선택해 주세요!";
		break;
	case ROOM_STATUS_CHANGEFAILED:
		sMessage = L"방 제목을 입력해 주세요!";
		break;
	case ROOM_STATUS_NOREADY:
		sMessage = L"입장 실패!";
		break;
	}

	// 메시지 내용 표시
	m_RoomWindow->getChild(sNamePrefix + "Room/Message")
		->getChild(sNamePrefix + "Room/Message/Text_Status")->setText((CEGUI::utf8*)sMessage.asUTF8_c_str());

	// 확인 버튼을 클릭할 경우 이벤트 발생
	m_RoomWindow->getChild(sNamePrefix + "Room/Message")->getChild(sNamePrefix + "Room/Message/Button_OK")->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(
		&GameRoomWindow::Handle_MessageOKButtonPressed,
		this));
	SoundMan::getSingletonPtr()->playSound("button");
}

bool GameRoomWindow::Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e)
{
	// 메시지 창을 감추고 이전의 창으로 돌아간다
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
	// 채팅창에서 전송 버튼을 클릭할 경우

	CEGUI::String Msg = m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->getText();

	(this)->ParseText(Msg);

	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->setText("");

	return true;;
}

bool GameRoomWindow::Handle_TextSubmitted(const CEGUI::EventArgs &e)
{
	// 채팅창에서 Return 키를 입력할 경우
	CEGUI::String Msg = m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->getText();
	
	(this)->ParseText(Msg);

	m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Chat")
		->getChild(sNamePrefix + "Room/Main/Chat/Edit_Input")->setText("");

	return true;;
}

void GameRoomWindow::ParseText(CEGUI::String inMsg)
{
	// 텍스트 파싱(명령어 등 처리)
 
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
				// 플레이어 방장권한
				if(!mBangjang)
				{
					mBangjang = true;
					mStatus = L"시작";
					m_RoomWindow->getChild(sNamePrefix + "Room/Main")->getChild(sNamePrefix + "Room/Main/Button_Ready")
						->setText((CEGUI::utf8*)mStatus.asUTF8_c_str());
				}
				else
				{
					mBangjang = false;
					mStatus = L"준비완료";
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
	// 입력된 내용을 채팅창에 출력

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
	if(pPlayerInfo->name == GetPlayerName())					// 플레이어 본인일 경우
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
	// 키 입력에 대한 처리

	if(!m_RoomWindow->isVisible())
		return;

	if(_arg.key == OIS::KC_RETURN)
	{
		// Return 키 입력

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
				//룸 윈도우 감추기
				m_RoomWindow->setVisible(false);
				GetGame()->GoNext(SS_TYPE::SS_GAME);
			}
			break;
		}
	}

	while(!m_RoomQueue_PlayerInfo.empty())		//플레이어 정보를 바꿔야할때.
	{
		stRoom_PlayerInfo stPlayer = m_RoomQueue_PlayerInfo.front();
		m_RoomQueue_PlayerInfo.pop();

		ChangePlayerInfo(&stPlayer);
	}
	
	while(!m_RoomQueue_RoomInfo.empty())		//방 정보를 바꿔야할때. 방에 처음 접속 했을때
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