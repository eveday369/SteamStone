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

#define				MAP_1		1001								// �� 1
#define				MAP_2		1002								// ��2
#define				GAME_TEAM	2001								// ����
#define				GAME_INDI	2002								// ������

enum LOBBY_STATUS { SELECT, NONAME, CREATEFAILED, ENTRYFAILED, FULL };	// �κ� ����

class GameLobbyWindow : public CGameState
{
private:
	typedef struct
	{
		BS_TYPE type;
	}stLobby;

	typedef struct
	{
		int				mRoomNo;										// �� ��ȣ
		Ogre::UTFString sRoomName;										// �� �̸�
		int				mRoomMap;										// ��
		int				mRoomType;										// ���� ����
		int				mRoomRun;										// ���� ���� ����
		int				mRoomCurr;										// ���� �� �ο�
		int				mRoomMax;										// �ִ� �� �ο�
	}room_list;

	typedef std::queue<stLobby>					LOBBY_QUEUE;
	LOBBY_QUEUE									m_LobbyQueue;
public:
	virtual bool	CheckData();
	void			PushData(int nRoomNum, int nNowCount, int nMaxCount, int nMapNum, bool bIsGame, std::string szMasterName, std::string szRoomName);	//�� ���� ����
	void			PushData(BS_TYPE type);
public:
	GameLobbyWindow(Game* pParent);
	virtual ~GameLobbyWindow(void);

	void setVisible(bool visible);									// �����츦 ǥ���ϰų� �����
	bool isVisible();												// ������ ǥ�� ��Ȳ

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg);
	void keyReleased(const OIS::KeyEvent &_arg){}

private:
	void CreateCEGUIWindow();
	void RegisterHandlers();
	bool Handle_CreateButtonPressed(const CEGUI::EventArgs &e);		// �� ����� ��ư �̺�Ʈ
	bool Handle_EntryButtonPressed(const CEGUI::EventArgs &e);		// �����ϱ� ��ư �̺�Ʈ
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);		// ������ ��ư �̺�Ʈ
	bool Handle_MessageOKButtonPressed(const CEGUI::EventArgs &e);	// �޽��� �ڽ� Ȯ�� ��ư �̺�Ʈ
	bool Handle_Room0ButtonPressed(const CEGUI::EventArgs &e);		// ��0 ��ư Ŭ�� �̺�Ʈ
	bool Handle_Room1ButtonPressed(const CEGUI::EventArgs &e);		// ��1 ��ư Ŭ�� �̺�Ʈ
	bool Handle_Room2ButtonPressed(const CEGUI::EventArgs &e);		// ��2 ��ư Ŭ�� �̺�Ʈ
	bool Handle_PrevButtonPressed(const CEGUI::EventArgs &e);		// ���� �� ����Ʈ Ŭ�� �̺�Ʈ
	bool Handle_NextButtonPressed(const CEGUI::EventArgs &e);		// ���� �� ����Ʈ Ŭ�� �̺�Ʈ
	bool Handle_RefreshButtonPressed(const CEGUI::EventArgs &e);	// ���ΰ�ħ Ŭ�� �̺�Ʈ
	bool Handle_ConfirmButtonPressed(const CEGUI::EventArgs &e);	// Ȯ�� ��ư Ŭ�� �̺�Ʈ
	bool Handle_CancelButtonPressed(const CEGUI::EventArgs &e);		// ��� ��ư Ŭ�� �̺�Ʈ
	bool Handle_SendButtonPressed(const CEGUI::EventArgs &e);		// ä�� ���� ��ư �̺�Ʈ
	bool Handle_TextSubmitted(const CEGUI::EventArgs &e);			// ä��â ���� Ű �̺�Ʈ
	
	void SetMessage(LOBBY_STATUS);									// �޽��� ����
	void Entry(int select);											// �� ���� ó��
	void Refresh();													// �� ����� ������� ������ ����.
	void Refresh_End(int i, room_list* roomList);					// �� ��� ���ΰ�ħ
	Ogre::UTFString getMap(int map);								// �� �̸� ��������
	Ogre::UTFString getType(int type);								// ���� Ÿ�� ��������
	void ParseText(CEGUI::String inMsg);							// ä��â �Ľ�(Ư��Ű �Է� ��)
	void OutputText(CEGUI::String inMsg,
		CEGUI::colour colour = CEGUI::colour( 0xFFFFFFFF));			// �ؽ�Ʈ ���

	CEGUI::Window	*m_LobbyWindow;									// �κ� ������
	CEGUI::String	sNamePrefix;									// ���̾ƿ��� �̸�
	static int		iInstanceNumber;								// �ν��Ͻ� ��ȣ
	bool			m_bLobby;
	Ogre::UTFString sTemp;											// �ӽ�
	Ogre::UTFString	sMessage;										// �ѱ� �޽���(UTF-8)
	bool			*mShutDown;										// ���� ����
	
	bool			mEntry;											// ���� ����
	bool			mFull;											// �� ���� �ʰ� ����
	int				mSelect;										// ���� ��
	int				mPage;											// �� ������ ��
	int				mCurrPage;										// ���� ������
	int				mRoomSize;										// �� ���� �� �� ����. ���� ���� ����� ���̴°�.
	int				mMap;											// �� ���� �� ����
	int				mType;											// �� ���� ���� ����
};

