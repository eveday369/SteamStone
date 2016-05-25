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

	void setVisible(bool visible);								// 로그인 창을 표시하거나 감춘다
    bool isVisible();											// 로그인 창 표시 상황을 반환한다(true = 표시, false = 감춤)
	bool setWinLose(bool win);

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg){}
	void keyReleased(const OIS::KeyEvent &_arg){}
	
	virtual bool CheckData();

private:
	void CreateCEGUIWindow();									// 윈도우 생성
	void RegisterHandlers();									// 핸드러 함수 등록
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);
	bool Handle_ReturnButtonPressed(const CEGUI::EventArgs &e);

	CEGUI::Window *m_ResultWindow;								// 게임 실행 윈도우에 대한 포인터
	CEGUI::String sNamePrefix;                                  // 레이아웃의 이름
	static int iInstanceNumber;                                 // 클래스의 인스턴스 번호
	bool			m_bRun;
	Ogre::UTFString sMessage;									// 한글 메시지(UTF-8)
};