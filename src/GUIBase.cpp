#include "stdafx.h"
#include "..\include\GUIBase.h"

GUIBase::GUIBase(void)
{
	CreateCEGUI();
}


GUIBase::~GUIBase(void)
{
	
}

void GUIBase::CreateCEGUI(void)
{
	// Ogre용 CEGUI 사용 선언
	CEGUI::OgreRenderer &myRenderer = CEGUI::OgreRenderer::bootstrapSystem();

	// Resource.cfg에 저장되어 있는 리소스 그룹을 불러옴
	// 지정하지 않을 경우 CEGUI 기본 리소스 디렉터리 사용
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	// CEGUI에서 사용할 각각의 매니저를 생성(불러오기)함
	// 다른 매니저를 추가로 생성하는 것도 가능
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme");
	CEGUI::ImagesetManager::getSingleton().create("Title.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Title1.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Title2.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Title3.imageset");
	CEGUI::ImagesetManager::getSingleton().create("logo.imageset");
	CEGUI::ImagesetManager::getSingleton().create("aisha.imageset");
	CEGUI::ImagesetManager::getSingleton().create("eve.imageset");
	CEGUI::ImagesetManager::getSingleton().create("ara.imageset");
	CEGUI::ImagesetManager::getSingleton().create("ready.imageset");
	CEGUI::ImagesetManager::getSingleton().create("blue_frame.imageset");
	CEGUI::ImagesetManager::getSingleton().create("red_frame.imageset");
	CEGUI::ImagesetManager::getSingleton().create("login.imageset");
	CEGUI::ImagesetManager::getSingleton().create("lobby.imageset");
	CEGUI::ImagesetManager::getSingleton().create("room.imageset");
	CEGUI::ImagesetManager::getSingleton().create("ingame.imageset");
	CEGUI::ImagesetManager::getSingleton().create("win.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Walker.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Gunner.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Psyker.imageset");
	CEGUI::ImagesetManager::getSingleton().create("loading.imageset");
	CEGUI::ImagesetManager::getSingleton().create("credit.imageset");
	CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font");
	CEGUI::FontManager::getSingleton().create("Malgun-10.font");
	CEGUI::FontManager::getSingleton().create("Malgun-20.font");
	CEGUI::ImagesetManager::getSingleton().create("Credit_1.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Credit_2.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Credit_3.imageset");
	CEGUI::ImagesetManager::getSingleton().create("Credit_4.imageset");
	CEGUI::ImagesetManager::getSingleton().create("lobby_Gunner.imageset");
	CEGUI::ImagesetManager::getSingleton().create("character_example.imageset");
	CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML("guiAni.xml");

	// 기본적으로 사용할 Looknfeel을 설정함
	// 폰트, 마우스커서, 툴팁 등...
	CEGUI::System::getSingleton().setDefaultFont("Malgun-10");

	// 마우스 커서는 Imageset에 저장되어 있다
	CEGUI::System::getSingleton().setDefaultMouseCursor("Vanilla-Images", "MouseArrow");
	
	// 윈도우 매니저를 생성한다
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window *myRoot = wmgr.createWindow("DefaultWindow", "_MasterRoot");

	CEGUI::System::getSingleton().setGUISheet(myRoot);
}