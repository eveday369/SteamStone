#pragma once
#include "BaseApplication.h"
#include "stdafx.h"

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIcolour.h>
#include <OgreStringConverter.h>

#include <OgreUTFString.h>										// Ogre���� �ѱ��� UTF-8�� ��ȯ�ϱ� ���� �� �ʿ���!

class GameRunWindow : public CGameState
//class GameRunWindow
{
public:
	bool	CheckStatus(int HP, int MaxHP, int MP, int MaxMP);
	bool	TimeCheck(float fGameTime);
	bool	Init(int job);
	bool	CheckSkill(GS_TYPE type, float mCool, float cCool);
	bool	ShowMenu(bool show);
public:
	//GameRunWindow();
	GameRunWindow(Game* pParent);
	virtual ~GameRunWindow();

	void setVisible(bool visible);								// �α��� â�� ǥ���ϰų� �����
    bool isVisible();											// �α��� â ǥ�� ��Ȳ�� ��ȯ�Ѵ�(true = ǥ��, false = ����)
	bool isMenu();
	void ShowLoadingImage(bool b);								// �ε� �̹��� ǥ��

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg) {}
	void keyReleased(const OIS::KeyEvent &_arg){}

	virtual bool CheckData();

private:
	void CreateCEGUIWindow();									// ������ ����
	void RegisterHandlers();									// �ڵ巯 �Լ� ���
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);	// ������ ��ư Ŭ��
	bool Handle_ReturnButtonPressed(const CEGUI::EventArgs &e);	// ���ư��� ��ư Ŭ��

	CEGUI::Window *m_RunWindow;									// ���� ���� �����쿡 ���� ������
	CEGUI::String sNamePrefix;                                  // ���̾ƿ��� �̸�
	static int iInstanceNumber;                                 // Ŭ������ �ν��Ͻ� ��ȣ
	bool			m_bRun;
	Ogre::UTFString sMessage;									// �ѱ� �޽���(UTF-8)

	bool	bMenu;												// �޴�â ǥ�� ����
	CEGUI::ImagesetManager*		imageset;
};