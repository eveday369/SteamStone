/*

	맵을 로딩하는 클래스

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

	Ogre::SceneManager	*mSceneMgr;			//씬 매니저 받아오기
	Ogre::Entity		*mMap[9];			//광장은 맵 조각이 9개
	Ogre::SceneNode		*mMapPieceNode[9];	//따라서 씬 노드도 9개
	Ogre::SceneNode		*mMapNode;			//위 9개의 씬노드를 총괄하는 씬노드

public:
	CMap();
	~CMap();

	//functions
	bool Initialize(Ogre::SceneManager *sceneMgr);
};