#pragma once
#include "BaseApplication.h"
#include "stdafx.h"
#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIcolour.h>

#include <OgreUTFString.h>				// Ogre에서 한글을 UTF-8로 변환하기 위해 꼭 필요함!

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

	void setVisible(bool visible);         // 로그인 창을 표시하거나 감춘다
    bool isVisible();                      // 로그인 창 표시 상황을 반환한다(true = 표시, false = 감춤)

	void Run(const Ogre::FrameEvent &evt) {}
	void keyPressed(const OIS::KeyEvent &_arg) {};
	void keyReleased(const OIS::KeyEvent &_arg) {};

private:
	void CreateCEGUIWindow();
	void RegisterHandlers();									// 핸드러 함수 등록
    bool Handle_IDSubmitted(const CEGUI::EventArgs &e);			// ID 입력창에서 Enter 키를 누를 때 처리
    bool Handle_PWSubmitted(const CEGUI::EventArgs &e);			// Password 입력창에서 Enter 키를 누를 때 처리
	bool Handle_ApplyButtonPressed(const CEGUI::EventArgs &e);  // Login 버튼을 눌렀을 때 처리        
	bool Handle_QuitButtonPressed(const CEGUI::EventArgs &e);	// Quit 버튼을 눌렀을 때 처리
	bool Handle_PWKeyDown(const CEGUI::EventArgs &e);			// Password 입력창에서 키를 누를 때 처리
	bool Handle_ProgressOKButtonPressed(const CEGUI::EventArgs &e);	// 진행상황 창에서 OK 버튼을 누를 때 처리
	bool Handle_ErrorOKButtonPressed(const CEGUI::EventArgs &e);// Error Window에서 OK 버튼을 누를 때 처리
	bool Handle_JoinButtonPressed(const CEGUI::EventArgs &e);	// Join 버튼을 눌렀을 때 처리
	bool Handle_JoinOKButtonPressed(const CEGUI::EventArgs &e);	// 가입 창에서 확인 버튼을 눌렀을 때 처리
	bool Handle_JoinCancelButtonPressed(const CEGUI::EventArgs &e);	// 가입 창에서 취소 버튼을 눌렀을 때 처리
	void ShowErrorWindow(STATUS);								// 오류 창 표시
	void ShowProgressWindow(STATUS);							// 진행 창 표시
	bool Login();												// 로그인 진행
	bool Join();												// 가입 진행

    CEGUI::Window *m_LoginWindow;								// 로그인 윈도우에 대한 포인터
    CEGUI::String sNamePrefix;                                  // 레이아웃의 이름
    static int iInstanceNumber;                                 // 클래스의 인스턴스 번호
    bool m_bLogin;
	Ogre::UTFString sMessage;									// 한글 메시지(UTF-8)
	void SetMessage(STATUS);									// 메시지 변경

};