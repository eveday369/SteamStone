#pragma once
#include "BaseApplication.h"
#include "stdafx.h"

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIcolour.h>
#include <OgreStringConverter.h>

#include <OgreUTFString.h>										// Ogre에서 한글을 UTF-8로 변환하기 위해 꼭 필요함!

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

	void setVisible(bool visible);								// 로그인 창을 표시하거나 감춘다
    bool isVisible();											// 로그인 창 표시 상황을 반환한다(true = 표시, false = 감춤)
	bool isMenu();
	void ShowLoadingImage(bool b);								// 로딩 이미지 표시

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg) {}
	void keyReleased(const OIS::KeyEvent &_arg){}

	virtual bool CheckData();

private:
	void CreateCEGUIWindow();									// 윈도우 생성
	void RegisterHandlers();									// 핸드러 함수 등록
	bool Handle_ExitButtonPressed(const CEGUI::EventArgs &e);	// 나가기 버튼 클릭
	bool Handle_ReturnButtonPressed(const CEGUI::EventArgs &e);	// 돌아가기 버튼 클릭

	CEGUI::Window *m_RunWindow;									// 게임 실행 윈도우에 대한 포인터
	CEGUI::String sNamePrefix;                                  // 레이아웃의 이름
	static int iInstanceNumber;                                 // 클래스의 인스턴스 번호
	bool			m_bRun;
	Ogre::UTFString sMessage;									// 한글 메시지(UTF-8)

	bool	bMenu;												// 메뉴창 표시 유무
	CEGUI::ImagesetManager*		imageset;
};