/*

	  ī�޶� ���� Ŭ����

*/
#include "BaseApplication.h"

#pragma once

class CCamera
{
protected:
	//values
	Ogre::Camera		*mCamera;			//ī�޶�
	Ogre::SceneNode		*mCameraNode;		//ī�޶��� �����
	Ogre::SceneManager	*mSceneMgr;
	Ogre::SceneNode*	mTargetNode;		//���� ��� ���.

	Ogre::Real			mSpeed;
	Ogre::Vector3		mSightRevision;		//ī�޶� ���� ��ġ
	Ogre::Vector3		mCurrentPosition;	//�ֱ� ��ġ
	Ogre::Vector3		mCurrentLook;

public:
	CCamera(Ogre::SceneManager *sceneMgr, Ogre::Camera *cam);	//�����, Ʈ��ŷ�� �� ���
	~CCamera();

	//functions
	void					Init(Ogre::SceneNode* TargetNode);
	void					moveCamera();		//ī�޶� �̵�
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