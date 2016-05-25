#pragma once

#include "BaseApplication.h"

class Energy
{
protected:
	Ogre::SceneManager *e_SceneMgr;
	
	Ogre::BillboardSet *e_HpBarSet;
	Ogre::Billboard	   *e_HpBar;

	Ogre::BillboardSet *e_HpDownSet;
	Ogre::Billboard    *e_HpDown;



public:
	Energy(Ogre::SceneManager *mgr);
	~Energy();

	void LoadHp(Ogre::SceneNode* Node, float fMaxHP, int JobNum);
	void Update(float fHP, float fMaxHP);
};


