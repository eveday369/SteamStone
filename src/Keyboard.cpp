#include "stdafx.h"
#include "Keyboard.h"

CKeyboard::CKeyboard()
{
	mDir = Ogre::Vector3::ZERO;
}

CKeyboard::~CKeyboard()
{
}

bool CKeyboard::keyPressed(const OIS::KeyEvent &arg)
{
	//if(arg.key == OIS::KC_LEFT)
	//	mDir.x = -1;
	//else if(arg.key == OIS::KC_RIGHT) mDir.x = 1;
	//if(arg.key == OIS::KC_X) mDir.y = 1;

	return true;
}

bool CKeyboard::keyReleased(const OIS::KeyEvent &arg)
{
	//if(arg.key == OIS::KC_LEFT && mDir.x == -1) mDir.x = 0;
	//if(arg.key == OIS::KC_RIGHT && mDir.x == 1) mDir.x = 0;
	//if(arg.key == OIS::KC_X && mDir.y == 1) mDir.y = 0;
	return true;
}