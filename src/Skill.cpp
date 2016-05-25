#include "stdafx.h"
#include "Skill.h"

CSkill::CSkill()
{
	mSkillActivate = true;
	//timer = new Ogre::Timer;
	//timer->reset();
	//timer_count = 0;

	mSkillNumber	= 0;
	mDamage			= 0;
	mCooltime		= 0;
	mDistance		= 0;
	mAttackframe	= 0;

	//for(int i=0 ; i<3; i++)
	//	mSkill[i] = false;
}

CSkill::~CSkill()
{
	//delete timer;
}

bool CSkill::WalkerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(mDistance >= 0)
	{
		//TargetNode->translate(Ogre::Vector3(1, 0, 0));
		mDistance -= 1.0;
		return false;
	}

	return true;
}

bool CSkill::WalkerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(Dir.x == 1)
		TargetNode->translate(10, 0, 0);
	if(Dir.x == -1)
		TargetNode->translate(-10, 0, 0);

		return true;
}

bool CSkill::WalkerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(Dir.x == 1)
		TargetNode->translate(10, 0, 0);
	if(Dir.x == -1)
		TargetNode->translate(-10, 0, 0);

	return true;
}

bool CSkill::GunnerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	/*if(mSkillActivate)
	{
		timer_count = timer->getMillisecondsCPU();
		while(timer_count<=2000)
		{
			mSkillActivate = false;
			if(Dir.x == 1)
				TargetNode->translate(10, 0, 0);
			if(Dir.x == -1)
				TargetNode->translate(-10, 0, 0);
		}

		return true;
	}*/
	return false;
}

bool CSkill::GunnerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(mSkillActivate)
	{
		if(Dir.x == 1)
			TargetNode->translate(10, 0, 0);
		if(Dir.x == -1)
			TargetNode->translate(-10, 0, 0);

		return true;
	}
	return false;
}

bool CSkill::GunnerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(mSkillActivate)
	{
		if(Dir.x == 1)
			TargetNode->translate(10, 0, 0);
		if(Dir.x == -1)
			TargetNode->translate(-10, 0, 0);

		return true;
	}
	return false;
}

bool CSkill::PsykerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(mSkillActivate)
	{
		if(Dir.x == 1)
			TargetNode->translate(10, 0, 0);
		if(Dir.x == -1)
			TargetNode->translate(-10, 0, 0);

		return true;
	}
	return false;
}

bool CSkill::PsykerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(mSkillActivate)
	{
		if(Dir.x == 1)
			TargetNode->translate(10, 0, 0);
		if(Dir.x == -1)
			TargetNode->translate(-10, 0, 0);

		return true;
	}
	return false;
}

bool CSkill::PsykerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir)
{
	if(mSkillActivate)
	{
		if(Dir.x == 1)
			TargetNode->translate(10, 0, 0);
		if(Dir.x == -1)
			TargetNode->translate(-10, 0, 0);

		return true;
	}
	return false;
}

//bool CSkill::Run(int SkillNum)
//{
//	/*if(mSkillActivate)
//	{
//		if(mSkillNum[SkillNum-1])
//		{
//			WalkerSkill_1();
//		}
//	}*/
//	return true;
//}