#pragma once
#include "BaseApplication.h"
#include "stdafx.h"

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIcolour.h>

class GameResultWindow : public CGameState
//class GameResultWindow
{
public:
	//GameResultWindow();
	GameResultWindow(Game* pParent);
	virtual ~GameResultWindow();

	void setVisible(bool visible);								// �α��� â�� ǥ���ϰų� �����
    bool isVisible();											// �α��� â ǥ�� ��Ȳ�� ��ȯ�Ѵ�(true = ǥ��, false = ����)
	bool setWinLose(bool win);

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg){}
	void keyReleased(const OIS::KeyEvent &_arg){}
	
	virtual bool CheckData();

private:
	void CreateCEGUIWindow();									// ������ ����
	void RegisterHandlers();									// �ڵ巯 �Լ� ���
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);
	bool Handle_ReturnButtonPressed(const CEGUI::EventArgs &e);

	CEGUI::Window *m_ResultWindow;								// ���� ���� �����쿡 ���� ������
	CEGUI::String sNamePrefix;                                  // ���̾ƿ��� �̸�
	static int iInstanceNumber;                                 // Ŭ������ �ν��Ͻ� ��ȣ
	bool			m_bRun;
	Ogre::UTFString sMessage;									// �ѱ� �޽���(UTF-8)
};