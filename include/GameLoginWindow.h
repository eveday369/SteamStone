#pragma once
#include "BaseApplication.h"
#include "stdafx.h"
#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIcolour.h>

#include <OgreUTFString.h>				// Ogre���� �ѱ��� UTF-8�� ��ȯ�ϱ� ���� �� �ʿ���!

enum STATUS { IDPW, WAIT, LOGINSUCCESS, LOGINFAILED, JOINSUCCESS, JOINFAILED };

class GameLoginWindow : public CGameState
{
private :
	typedef struct
	{
		LP_TYPE type;
		Ogre::String id;
	}stLogin;


	typedef std::queue<stLogin> LOGIN_QUEUE;
	LOGIN_QUEUE			m_LoginQueue;
public :
	void			PushData(LP_TYPE type, std::string id);
	virtual bool	CheckData();
public:
	GameLoginWindow(Game* pParent);
	virtual ~GameLoginWindow(void);

	void setVisible(bool visible);         // �α��� â�� ǥ���ϰų� �����
    bool isVisible();                      // �α��� â ǥ�� ��Ȳ�� ��ȯ�Ѵ�(true = ǥ��, false = ����)

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg) {};
	void keyReleased(const OIS::KeyEvent &_arg) {};

private:
	void CreateCEGUIWindow();
	void RegisterHandlers();									// �ڵ巯 �Լ� ���
    bool Handle_IDSubmitted(const CEGUI::EventArgs &e);			// ID �Է�â���� Enter Ű�� ���� �� ó��
    bool Handle_PWSubmitted(const CEGUI::EventArgs &e);			// Password �Է�â���� Enter Ű�� ���� �� ó��
	bool Handle_ApplyButtonPressed(const CEGUI::EventArgs &e);  // Login ��ư�� ������ �� ó��        
	bool Handle_QuitButtonPressed(const CEGUI::EventArgs &e);	// Quit ��ư�� ������ �� ó��
	bool Handle_PWKeyDown(const CEGUI::EventArgs &e);			// Password �Է�â���� Ű�� ���� �� ó��
	bool Handle_ProgressOKButtonPressed(const CEGUI::EventArgs &e);	// �����Ȳ â���� OK ��ư�� ���� �� ó��
	bool Handle_ErrorOKButtonPressed(const CEGUI::EventArgs &e);// Error Window���� OK ��ư�� ���� �� ó��
	bool Handle_JoinButtonPressed(const CEGUI::EventArgs &e);	// Join ��ư�� ������ �� ó��
	bool Handle_JoinOKButtonPressed(const CEGUI::EventArgs &e);	// ���� â���� Ȯ�� ��ư�� ������ �� ó��
	bool Handle_JoinCancelButtonPressed(const CEGUI::EventArgs &e);	// ���� â���� ��� ��ư�� ������ �� ó��
	void ShowErrorWindow(STATUS);								// ���� â ǥ��
	void ShowProgressWindow(STATUS);							// ���� â ǥ��
	bool Login();												// �α��� ����
	bool Join();												// ���� ����

    CEGUI::Window *m_LoginWindow;								// �α��� �����쿡 ���� ������
    CEGUI::String sNamePrefix;                                  // ���̾ƿ��� �̸�
    static int iInstanceNumber;                                 // Ŭ������ �ν��Ͻ� ��ȣ
    bool m_bLogin;
	Ogre::UTFString sMessage;									// �ѱ� �޽���(UTF-8)
	void SetMessage(STATUS);									// �޽��� ����

};