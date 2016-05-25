#include "stdafx.h"
#include "Energy.h"

Energy::Energy(Ogre::SceneManager *mgr)
{
	e_SceneMgr = mgr;
	e_HpBarSet = NULL;
	e_HpBar = NULL;
	e_HpDownSet = NULL;
	e_HpDown = NULL;
}

Energy::~Energy()
{
}

void Energy::LoadHp(Ogre::SceneNode *Node, float fMaxHP, int JobNum)
{
	Ogre::Vector3 FixedPosition;
	Ogre::Vector3 FixedPosition2;
	FixedPosition = Node->getPosition();
	FixedPosition2 = Node->getPosition();
	if(JobNum == 0)	FixedPosition.y += 5.0f;
	if(JobNum == 2)	FixedPosition.y += 6.2f;
	if(JobNum == 1) FixedPosition.y += 5.4f;

	if(JobNum == 0)	FixedPosition.z += 0.1f;
	if(JobNum == 2)	FixedPosition.z += 0.1f;
	if(JobNum == 1) FixedPosition.z += 0.1f;

	e_HpBarSet = e_SceneMgr->createBillboardSet();
	e_HpBarSet->setMaterialName("State/HpBar");
	e_HpBarSet->setCastShadows(false);
//  e_HpBar = e_HpBarSet->createBillboard(Ogre::Vector3(Node->getPosition().x, Node->getPosition().y + 5.0f, Node->getPosition().z));
	e_HpBar = e_HpBarSet->createBillboard(Ogre::Vector3(FixedPosition));
	e_HpBar->setTexcoordRect(0.0f,0.0f,0.5f,1.0f);
	e_HpBar->setDimensions(4.0f, 0.5f);
	Node->attachObject(e_HpBarSet);

	e_HpDownSet = e_SceneMgr->createBillboardSet();
	e_HpDownSet->setMaterialName("State/HpBackGround");
	e_HpDownSet->setCastShadows(false);
	//e_HpDown = e_HpDownSet->createBillboard(Ogre::Vector3(Node->getPosition().x, Node->getPosition().y + 5.0f, Node->getPosition().z));
	e_HpDown = e_HpDownSet->createBillboard(Ogre::Vector3(FixedPosition));
	e_HpDown->setDimensions(10.0f, 0.6f);
	Node->attachObject(e_HpDownSet);

}

void Energy::Update(float fHP, float fMaxHP)
{
	float _fHP = fHP;
	if(fHP <= 0.0f)
		_fHP = 0.1f;
	Ogre::Real Number = _fHP / fMaxHP;

	Ogre::Real u1 = (1.0f - Number) / 2.0f;
	Ogre::Real v1 = 0.0f;
	Ogre::Real u2 = 0.5f + (1.0f - Number) / 2.0f;
	Ogre::Real v2 = 1.0f;

	e_HpBar->setTexcoordRect(u1, v1, u2, v2);
	e_HpDown->setDimensions(4.2f, 0.6f);
}