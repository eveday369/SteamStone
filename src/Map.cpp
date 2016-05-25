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
argument : BaseApplication�� �� �� �Ŵ���
description : �ʿ� ���� �迭, ��ƼƼ, ������ �ʱ�ȭ
-------------------------------------------------------------------------------------*/
bool CMap::Initialize(Ogre::SceneManager *sceneMgr)
{
	mSceneMgr = sceneMgr;

	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	//mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	//mSceneMgr->setShadowTextureSize(1024);
	//mSceneMgr->setShadowTextureCount(1);

	//printf("bbbbbbbbbbbb\n");
	//���� ���� �޽��� �ΰ��� �и��Ǿ� �ִ�
	//�ΰ��� �ҷ��ͼ� ���ľ� ����� �� ���� ���´�
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
	//Bg_Node->pitch(Ogre::Degree(90));				//�� ����� �÷����� �����ֱ� ������ ���� 90���� ������ �����


	printf("aaaaaaaaaaaaaaa\n");
	return true;
}