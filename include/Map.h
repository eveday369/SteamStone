/*

	���� �ε��ϴ� Ŭ����

*/

#pragma once

#include "BaseApplication.h"

class CMap{
protected:
	//values
	enum QueryFlags
	{
		STAND = 1,
		FLOOR = 2
	};

	Ogre::SceneManager	*mSceneMgr;			//�� �Ŵ��� �޾ƿ���
	Ogre::Entity		*mMap[9];			//������ �� ������ 9��
	Ogre::SceneNode		*mMapPieceNode[9];	//���� �� ��嵵 9��
	Ogre::SceneNode		*mMapNode;			//�� 9���� ����带 �Ѱ��ϴ� �����

public:
	CMap();
	~CMap();

	//functions
	bool Initialize(Ogre::SceneManager *sceneMgr);
};