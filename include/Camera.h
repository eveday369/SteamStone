/*

	  카메라 관련 클래스

*/
#include "BaseApplication.h"

#pragma once

class CCamera
{
protected:
	//values
	Ogre::Camera		*mCamera;			//카메라
	Ogre::SceneNode		*mCameraNode;		//카메라의 씬노드
	Ogre::SceneManager	*mSceneMgr;
	Ogre::SceneNode*	mTargetNode;		//따라갈 대상 노드.

	Ogre::Real			mSpeed;
	Ogre::Vector3		mSightRevision;		//카메라가 보는 위치
	Ogre::Vector3		mCurrentPosition;	//최근 위치
	Ogre::Vector3		mCurrentLook;

public:
	CCamera(Ogre::SceneManager *sceneMgr, Ogre::Camera *cam);	//씬노드, 트래킹할 씬 노드
	~CCamera();

	//functions
	void					Init(Ogre::SceneNode* TargetNode);
	void					moveCamera();		//카메라 이동
	void					moveCamera(Ogre::Real deltaTime);
	void					moveCamera(Ogre::Vector3 vector);

	inline Ogre::Vector3	GetSightRevision()								{ return mSightRevision; }
	inline void				SetSightRevision(Ogre::Vector3 sight)			{ mSightRevision = sight; }
	inline void				SetSightRevision(float sX, float sY, float sZ)	{ mSightRevision.x = sX; mSightRevision.y = sY; mSightRevision.z = sZ; }
	inline void				SetSightRevisionReset()							{ mSightRevision = Ogre::Vector3(0.0f, 0.0f, 0.01f); }
	inline void				SetInitPostion(Ogre::Vector3 pos)				{ mCurrentPosition = pos + mSightRevision; mCurrentLook = pos; }
	inline Ogre::Viewport*	GetViewport()	{return mCamera->getViewport();}
private:
};