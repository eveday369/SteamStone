#include "stdafx.h"
#include "camera.h"

CCamera::CCamera(Ogre::SceneManager *sceneMgr, Ogre::Camera *cam)
{
	mCamera = cam;
	mSceneMgr = sceneMgr;
	mSpeed = 10.0f;

	mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mCameraNode->attachObject(mCamera);

	mCameraNode->setFixedYawAxis (true);
	mTargetNode = NULL;
}

CCamera::~CCamera()
{
	//mCamera = NULL;
	//mSceneMgr = NULL;

	mTargetNode = NULL;
}

void CCamera::Init(Ogre::SceneNode* TargetNode)
{
	mTargetNode = TargetNode;
	SetInitPostion(TargetNode->_getDerivedPosition());
}

void CCamera::moveCamera()
{
	//Ogre::Vector3 goalPos = mTargetNode->getPosition() + mSightRevision;
	//mCameraNode->setPosition(goalPos);
	//mCamera->lookAt(goalPos);
	if(mTargetNode == NULL)
		return;

	mCameraNode->setPosition(mTargetNode->getPosition().x, mTargetNode->getPosition().y+5.0f, mTargetNode->getPosition().z-20.0);
}

void CCamera::moveCamera(Ogre::Real deltaTime)
{
	if(mTargetNode == NULL)
		return;

	Ogre::Vector3 goalPos = mTargetNode->_getDerivedPosition() + mSightRevision;
	Ogre::Vector3 wantSpeed = goalPos - mCurrentPosition;
	
	wantSpeed *= mSpeed;

	
	mCameraNode->translate(wantSpeed * deltaTime);

	/*if(mCameraNode->getPosition().x <= -50.0f)
	{
		mCameraNode->setPosition(Ogre::Vector3(mTargetNode->getPosition().x-(mTargetNode->getPosition().x+50.0f), 0, 0));
		mCamera->lookAt(Ogre::Vector3(mTargetNode->getPosition().x-(mTargetNode->getPosition().x+50.0f), mTargetNode->getPosition().y, mTargetNode->getPosition().z));
	}
	else if(mCameraNode->getPosition().x >= 50.0f)
	{
		mCameraNode->setPosition(Ogre::Vector3(mTargetNode->getPosition().x-(mTargetNode->getPosition().x-50.0f), 0, 0));
		mCamera->lookAt(Ogre::Vector3(mTargetNode->getPosition().x-(mTargetNode->getPosition().x-50.0f), mTargetNode->getPosition().y, mTargetNode->getPosition().z));
	}
	else
		mCamera->lookAt(mTargetNode->getPosition());*/

	mCamera->lookAt(mTargetNode->_getDerivedPosition());
	mCurrentPosition = mCameraNode->_getDerivedPosition();
}

//임시 함수 나중에 지우자
void CCamera::moveCamera(Ogre::Vector3 vector)
{
	mCameraNode->translate(vector);
}