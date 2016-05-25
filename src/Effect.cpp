#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect(Ogre::SceneManager* mSceneMgr, Ogre::String Name, int job)
{
	mEffect[0] = mSceneMgr->createEntity(Name + "Hit1", "Hit1Plane001.mesh");
	mEffect[1] = mSceneMgr->createEntity(Name + "Hit2", "Hit2Plane001.mesh");
	mEffect[2] = mSceneMgr->createEntity(Name + "WalkerSkill1", "WalkerSkill1Plane001.mesh");
	mEffect[3] = mSceneMgr->createEntity(Name + "WalkerSkill2", "WalkerSkill2Plane001.mesh");
	mEffect[4] = mSceneMgr->createEntity(Name + "WalkerExplosion", "WalkerExplosionPlane001.mesh");
	mEffect[5] = mSceneMgr->createEntity(Name + "PsykerSkill1", "PsykerSkill1Plane001.mesh");
	mEffect[6] = mSceneMgr->createEntity(Name + "PsykerSkill2", "PsykerSkill2Plane001.mesh");
	mEffect[7] = mSceneMgr->createEntity(Name + "PsykerExplosion", "PsykerExplosionPlane001.mesh");
	mEffect[8] = mSceneMgr->createEntity(Name + "Shoot", "ShootPlane001.mesh");

	for(int i = 0; i < MaxEffect; i++)
	{
		mNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mNode[i]->attachObject(mEffect[i]);
		mEffect[i]->setVisible(true);
		mEffect[i]->setCastShadows(false);
		mNode[i]->setVisible(false);
		mAnimationState[i] = mEffect[i]->getAnimationState("effect");
		mAnimationState[i]->setLoop(false);
		mAnimationState[i]->setEnabled(false);
		mAnimationState[i]->setWeight(0.5);
		mAnimationState[i]->setTimePosition(0);
	}
	mReverse = false;
	mJob = job;

	JobCorrection();
}


CEffect::~CEffect(void)
{
}

// 애니메이션 업데이트
bool CEffect::Update(Ogre::SceneNode* node, const Ogre::FrameEvent& evt)
{
	for(int i = 0; i < MaxEffect; i++)
	{
		mNode[i]->setPosition(node->getPosition());

		if(mAnimationState[i]->hasEnded())
		{
			mNode[i]->setVisible(false);
			mAnimationState[i]->setTimePosition(0);
			mAnimationState[i]->setEnabled(false);
		}
		else if(mAnimationState[i]->getEnabled())
		{
			AlignEffectPosition(mNode[i], i);
			if(i == 5)
				mAnimationState[i]->addTime(evt.timeSinceLastFrame * 5);
			else
				mAnimationState[i]->addTime(evt.timeSinceLastFrame);
		}
	}

	return true;
}

// 이펙트 재생 : SceneManager와 이펙트 타입 지정
bool CEffect::Play(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* node, int mType, Ogre::Vector3 dir)
{
	mDir = dir;

	if(mType == Effect::PsykerSkill1 && dir.x == -1)
	{
		if(!mReverse)
		{
			mNode[mType]->yaw(Ogre::Radian(0));
		}
		else
		{
			mNode[mType]->yaw(Ogre::Radian(3.14f));
			mReverse = false;
		}
	}
	else if(mType == Effect::PsykerSkill1 && dir.x == 1)
	{
		if(!mReverse)
		{
			mNode[mType]->yaw(Ogre::Radian(3.14f));
			mReverse = true;
		}
		else
		{
			mNode[mType]->yaw(Ogre::Radian(0));
		}
	}

	mNode[mType]->setScale(5, 5, 5);
	mNode[mType]->setVisible(true);
	mAnimationState[mType]->setEnabled(true);

	return true;
}

// 캐릭터 노드를 기준으로 이펙트별 좌표 세부 조정
bool CEffect::AlignEffectPosition(Ogre::Node* node, int type)
{
	switch(type)
	{
	case Effect::Hit1:
		mNode[type]->setPosition(node->getPosition().x + (3 * mDir.x), node->getPosition().y + 5, node->getPosition().z);
		break;
	case Effect::Hit2:
		mNode[type]->setPosition(node->getPosition().x + (3 * mDir.x), node->getPosition().y + 5, node->getPosition().z);
		break;
	case Effect::WalkerSkill1:
		// 조정 완료
		mNode[type]->setPosition(node->getPosition().x, node->getPosition().y + 5, node->getPosition().z + 10);
		break;
	case Effect::WalkerSkill2:
		// 조정 완료
		mNode[type]->setPosition(node->getPosition().x + (-3 * mDir.x), node->getPosition().y + 7.5, node->getPosition().z);
		break;
	case Effect::WalkerExplosion:
		// 조정 완료
		mNode[type]->setPosition(node->getPosition().x + (10 * mDir.x), node->getPosition().y + 10, node->getPosition().z);
		break;
	case Effect::PsykerSkill1:
		// 조정 완료
		mNode[type]->setPosition(node->getPosition().x + (3 * mDir.x), node->getPosition().y + 6, node->getPosition().z);
		break;
	case Effect::PsykerSkill2:
		// 조정 완료
		mNode[type]->setPosition(node->getPosition().x, node->getPosition().y + 5, node->getPosition().z);
		break;
	case Effect::PsykerExplosion:
		mNode[type]->setPosition(node->getPosition().x + (10 * mDir.x), node->getPosition().y, node->getPosition().z);
		break;
	case Effect::Shoot:
		// 조정 완료
		mNode[type]->setPosition(node->getPosition().x + (9 * mDir.x), node->getPosition().y + 5, node->getPosition().z);
		break;
	}

	return true;
}

Ogre::Vector3 CEffect::JobCorrection()
{
	switch(mJob)
	{
	case 0:
		mCorr = Ogre::Vector3(5, 5, 0);
		break;
	case 1:
		mCorr = Ogre::Vector3(5, 5, 0);
		break;
	case 2:
		mCorr = Ogre::Vector3(5, 5, 0);
		break;
	}

	return mCorr;
}
