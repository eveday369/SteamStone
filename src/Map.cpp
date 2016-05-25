#include "stdafx.h"
#include "Map.h"

CMap::CMap()
{
}

CMap::~CMap()
{
	mSceneMgr->setSkyBoxEnabled(false);
}


/*-------------------------------------------------------------------------------------
function : Initialize
argument : BaseApplication의 주 씬 매니저
description : 맵에 대한 배열, 엔티티, 씬노드들 초기화
-------------------------------------------------------------------------------------*/
bool CMap::Initialize(Ogre::SceneManager *sceneMgr)
{
	mSceneMgr = sceneMgr;

	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	//mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	//mSceneMgr->setShadowTextureSize(1024);
	//mSceneMgr->setShadowTextureCount(1);

	//printf("bbbbbbbbbbbb\n");
	//현재 맵의 메쉬가 두개로 분리되어 있다
	//두개를 불러와서 겹쳐야 제대로 된 맵이 나온다
	Ogre::Entity *FrontBlock = mSceneMgr->createEntity("FrontBlock", "mapObject012.mesh");		
	Ogre::Entity *BackBlock = mSceneMgr->createEntity("BackBlock", "mapObject011.mesh");
	//Ogre::Entity *BackGround = mSceneMgr->createEntity("BG", "mapPlane001.mesh");
	FrontBlock->setCastShadows(false);
	BackBlock->setCastShadows(false);
	//BackGround->setCastShadows(false);

	//BackBlock->setMaterialName("Light/SingleBump");

	Ogre::SceneNode	*map_node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	map_node->attachObject(FrontBlock);
	map_node->attachObject(BackBlock);

	Ogre::SceneNode *Bg_Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//Bg_Node->attachObject(BackGround);

	map_node->setScale(25, 25, 25);
	map_node->setPosition(0, 0, 0);

	//Bg_Node->setPosition(Ogre::Vector3(0, 0, -135));
	//Bg_Node->setScale(Ogre::Vector3(40, 40, 40));
	//Bg_Node->pitch(Ogre::Degree(90));				//뒷 배경의 플레인이 누워있기 때문에 가로 90도로 일으켜 세운다


	printf("aaaaaaaaaaaaaaa\n");
	return true;
}