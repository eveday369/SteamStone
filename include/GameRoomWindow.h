#pragma once
#include "BaseApplication.h"
#include "stdafx.h"

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIcolour.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreUTFString.h>											// ���� UTF ��ȯ ���

//#define				MAP_1		1001								// �� 1
//#define				MAP_2		1002								// ��2
//#define				CHARACTER_1	3000								// ĳ����1
//#define				CHARACTER_2	3001								// ĳ����2
//#define				CHARACTER_3	3002								// ĳ����3

enum ROOM {
	ROOM_STATUS_NAME = 10,
	ROOM_STATUS_CHANGEFAILED = 11,
	ROOM_STATUS_NOREADY = 12,
	ROOM_MAP_MAP1 = 30,
	ROOM_MAP_MAP2 = 31,
	ROOM_TYPE_INDI = 51,
	ROOM_CHARACTER = 70,
	ROOM_CHARACTER_WALKER = 70,
	ROOM_CHARACTER_GUNNER = 71,
	ROOM_CHARACTER_PSYKER = 72,
};

class GameRoomWindow : public CGameState
{
private:
	typedef struct
	{
		BS_TYPE type;
	}stRoom;
	
	typedef struct
	{
		Ogre::UTFString		szMasterName;	//���� �̸�
		Ogre::UTFString		szRoomName;		//�� �̸�
		int					nRoomNum;		//�� ��ȣ
		int					nRoomCurr;		//���� �� �ο�
		int					nRoomMax;		//�� �ִ� �ο�
		int					nMapNum;		//�� ��ȣ
	}stRoom_RoomInfo;

	typedef struct
	{
		int num;				//���� �ȿ��� ���� ��ġ�ϰ� �ִ°�.
		std::string name;		//�� ��� �̸�
		int jobnum;				//���� ��ȣ
		bool ready;				//�غ����
	}stRoom_PlayerInfo;

	typedef std::queue<stRoom> ROOM_QUEUE;
	typedef std::queue<stRoom_RoomInfo> ROOM_QUEUE_ROOMINFO;
	typedef std::queue<stRoom_PlayerInfo> ROOM_QUEUE_PLAYERINFO;

	ROOM_QUEUE					m_RoomQueue;
	ROOM_QUEUE_ROOMINFO			m_RoomQueue_RoomInfo;
	ROOM_QUEUE_PLAYERINFO		m_RoomQueue_PlayerInfo;

public:
	virtual bool				CheckData();
	void						PushData(BS_TYPE type);
	void						PushData(std::string szMasterName, std::string szRoomName, int nRoomNum, int nRoomCurr, int nRoomMax, int nMapNum);	//�� ���� ����
	void						PushData(int nNum, std::string szName, int nJobNum, bool bReady);		//����� �÷��̾� ���� ������ �� ����.
	
public:
	GameRoomWindow(Game* pParent);
	virtual ~GameRoomWindow(void);

	void setVisible(bool visible);									// �����츦 ǥ���ϰų� �����
	bool isVisible();												// ������ ǥ�� ��Ȳ

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg);
	void keyReleased(const OIS::KeyEvent &_arg);

private:
	void CreateCEGUIWindow();
	void RegisterHandlers();										// �ڵ鷯 �Լ� ���
	bool Handle_ChaPrevButtonPressed(const CEGUI::EventArgs &e);	// ���� ĳ���� ��ư �̺�Ʈ
	bool Handle_ChaNextButtonPressed(const CEGUI::EventArgs &e);	// ���� ĳ���� ��ư �̺�Ʈ
	bool Handle_ReadyButtonPressed(const CEGUI::EventArgs &e);		// �غ� ��ư �̺�Ʈ
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);		// ������ ��ư �̺�Ʈ
	bool Handle_RoomChangeButtonPressed(const CEGUI::EventArgs &e); // �� ���� ���� ��ư �̺�Ʈ
	bool Handle_ConfirmButtonPressed(const CEGUI::EventArgs &e);	// Ȯ�� Ŭ�� �̺�Ʈ
	bool Handle_CancelButtonPressed(const CEGUI::EventArgs &e);		// ��� Ŭ�� �̺�Ʈ
	bool Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e);	// �޽��� �ڽ� Ȯ�� ��ư �̺�Ʈ
	bool Handle_SendButtonPressed(const CEGUI::EventArgs &e);		// ä�� ���� ��ư �̺�Ʈ
	bool Handle_TextSubmitted(const CEGUI::EventArgs &e);			// ä��â ���� Ű �̺�Ʈ

	void ShowPlayerImage(int ch, bool ready);						// ĳ���� �̹��� ǥ��
	void ShowEnemyImage(int ch, bool ready);						// ���� �̹��� ǥ��
	void SetMessage(ROOM s);										// �޽��� ǥ��
	void SetExampleImage(int ch);									// ĳ���� ���� ǥ��

	void ParseText(CEGUI::String inMsg);							// ä��â �Ľ�(Ư��Ű �Է� ��)
	void OutputText(CEGUI::String inMsg,
		CEGUI::colour colour = CEGUI::colour( 0xFFFFFFFF));			// �ؽ�Ʈ ���

	void ChangeRoomInfo(stRoom_RoomInfo* pRoomInfo);
	void ChangePlayerInfo(stRoom_PlayerInfo* pPlayerInfo);

	CEGUI::Window	*m_RoomWindow;									// �κ� ������
	CEGUI::String	sNamePrefix;									// ���̾ƿ��� �̸�
	static int		iInstanceNumber;								// �ν��Ͻ� ��ȣ
	bool			m_bRoom;
	Ogre::UTFString	sMessage;										// �ѱ� �޽���(UTF-8)
	Ogre::Entity		*mWalker;
	Ogre::Entity		*mGunner;
	Ogre::Entity		*mPsyker;
	Ogre::SceneNode		*mNode;

	int				mCharacter;										// ������ ĳ����
	bool			mReady;											// �غ� ����
	bool			mBangjang;										// ���� ����
	bool			mType;											// ���� ����
	int				mGameType;										// ���� Ÿ��
	int				mMap;											// ��
	bool			mInput;											// �Է� ����
};