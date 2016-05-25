#pragma once

#include "BaseApplication.h"

class CKeyboard
{
protected:
	//values
	Ogre::Vector3 mDir;

public:
	OIS::KeyCode key;
	CKeyboard();
	~CKeyboard();

	//functions
	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);

	Ogre::Vector3 GetDir() {return mDir;}
};