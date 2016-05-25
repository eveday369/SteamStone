#include "stdafx.h"
#include "Character.h"

#define VELOCITY_DEFAULT	70.0f
#define GRAVITY				200.0f
#define RUNSPEED			30

CCharacter::CCharacter(Ogre::String Name, Ogre::SceneManager *sceneMgr, int JobNum)
{
	mName = Name;
	mSceneMgr = sceneMgr;

	//��ƼƼ ����
	switch(JobNum)
	{
	case JOB_WALKER:
		mChar = mSceneMgr->createEntity(mName, "walkerObject02.mesh");
		mChar->setMaterialName("Walker/Skin2");
		break;
	case JOB_GUNNER:
		mChar = mSceneMgr->createEntity(mName, "gunnerBox05.mesh");
		mChar->setMaterialName("Gunner/Skin2");
		break;
	case JOB_PSYKER:
		mChar = mSceneMgr->createEntity(mName, "psykerBox03.mesh");
		mChar->setMaterialName("Psyker/Skin2");
		break;
	}
	//mChar->setCastShadows(true);

	mDirection = Ogre::Vector3(0,0,0);
	mCurrentSpeed = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	mCurrentPosition = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	mCurrentTime = 0;

	m_nMaxHP	= 0;
	m_nHP		= 0;
	m_nMaxMP	= 0;
	m_nMP		= 0;

	m_fSkill1Cool		= 0.0f;
	m_fSkill2Cool		= 0.0f;
	m_fSkill3Cool		= 0.0f;
	m_fSkill1NowCool	= 0.0f;
	m_fSkill2NowCool	= 0.0f;
	m_fSkill3NowCool	= 0.0f;

	MotionNumber = 0;

	//����� ����
	mTargetNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mTargetNode->attachObject(mChar);

	//��ų �ʱ�ȭ
	for(int i=0 ; i<3 ; i++)
	{
		mSkill[i] = false;
	}
	mGuard = false;
	bInput = true;
	bEffect = true;
	mHit = false;
	mTimer = 0;
}

CCharacter::~CCharacter()
{
	ReleaseAll();
}

void CCharacter::ReleaseAll()
{
	//TextIDNode->detachAllObjects();
	//mSceneMgr->destroyMovableObject(TextID);
	//mSceneMgr->destroySceneNode(TextIDNode);
	//TextIDNode = NULL;

	mTargetNode->detachAllObjects();
	mSceneMgr->destroyEntity(mName);
	mSceneMgr->destroySceneNode(mTargetNode);
	//mSceneMgr->clearScene();
	mTargetNode = NULL;
}

/*================================================================================
                             Initialize
================================================================================*/
void CCharacter::initialize()
{
	//mTargetNode->setPosition(0.0f, 25.0f, 0.0f);		//��� ��ġ �ʱ�ȭ
	mTargetNode->setScale(2, 2, 2);

	//ĳ������ ������ �ʱ�ȭ
	mDirection = Ogre::Vector3::ZERO;			//ĳ���� ���� �ʱ�ȭ
	mState = IDLE;							//ĳ���� ���� IDLE�� ����
	mMove = false;
	mGround = true;
	mLanding = false;
//	mJump = false;
//	mAttack = false;

	initTextID();

//	ENERGYBAR_INFO	energyBarInfo;
	m_cBillboard = new Energy(mSceneMgr);
	m_cBillboard->LoadHp(mTargetNode, getMaxHP(), mJob);

	//�ִϸ��̼ǵ��� �ε巴�� �������ش�
	anim = new CAnimation;
	anim->setupAnimation(mChar);
	anim->setBaseAnimation(mState);

	//����Ʈ ����
	effect = new CEffect(mSceneMgr, mName, mJob);
}

/*================================================================================
                             Update
================================================================================*/

void CCharacter::ResetCharacter()
{
	mState = IDLE;							//ĳ���� ���� IDLE�� ����
}

void CCharacter::update(const Ogre::FrameEvent &evt)
{
	//std::cout<<mState<<std::endl;


	m_cBillboard->Update(getHP(), getMaxHP());

	//�������� �ƴҶ� �̵� �����ϴ�
	//if(mState != ATTACK && mState != DEAD)
	move(evt.timeSinceLastFrame);

	//����
	if( mState == IDLE && m_nHP <= 0 )
	{
		mState = DEAD;
		mTimer = 0;
	}

	if(mHit == true)
	{
		//std::cout<<"�¾Ҵ�"<<std::endl;
		//mState = HIT;
		//mSkill[0] = false;
		//mSkill[1] = false;
		//mSkill[2] = false;
		//MotionNumber = 0;
		//mTimer = 0;
		effect->Play(mSceneMgr, mTargetNode, Effect::Hit1, mDirection);
		mHit = false;
	}
	//std::cout<<mState<<std::endl;

	//����Ʈ ������Ʈ
	effect->Update(mTargetNode, evt);
	//effect->Update(evt);
}

void CCharacter::initTextID()
{
	switch(mJob)
	{
		case JOB_WALKER:
			TextID = new Ogre::MovableText(mName+"_ID", mName,"HoHoFont");
			TextID->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_CENTER);
			TextIDNode = mTargetNode->createChildSceneNode(mName+"_node", Ogre::Vector3(0, 5.8, 0));
			TextIDNode->attachObject(TextID);
			break;
		case JOB_PSYKER:
			TextID = new Ogre::MovableText(mName+"_ID", mName,"HoHoFont");
			TextID->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_CENTER);
			TextIDNode = mTargetNode->createChildSceneNode(mName+"_node", Ogre::Vector3(0, 6.8, 0));
			TextIDNode->attachObject(TextID);
			break;
		case JOB_GUNNER:
			TextID = new Ogre::MovableText(mName+"_ID", mName,"HoHoFont");
			TextID->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_CENTER);
			TextIDNode = mTargetNode->createChildSceneNode(mName+"_node", Ogre::Vector3(0, 6.0, 0));
			TextIDNode->attachObject(TextID);
			break;
	}

}
/*================================================================================
                             Jump
================================================================================*/
void CCharacter::jump(Ogre::Real deltaTime)
{
	//translate�� ���� y���� ��� ���ذ���
	//�ӷ¿� �߷°��� ��� ���� y���� ������Ű�ٰ� 
	//0�� �Ǵ� ���� ���߿��� ���߰�, �� �̻����� �Ѿ�� �Ǹ� �����Ѵ�
	//mPeak = false;
	//if(mVerticalVelocity[0] >= 0)
	//{
	//	//mTargetNode->translate(0, mVerticalVelocity[0]*(deltaTime*2), 0, Ogre::Node::TS_LOCAL);
	//	mVerticalVelocity[0] -= GRAVITY * deltaTime;
	//}

	//ĳ���Ͱ� ������ ���������� üũ�Ͽ� ���̸� �߻�
	//if(mVerticalVelocity[0] <= 0)
	//{
	//	mPeak = true;
	//	//mJump = false;
	//}
	//
	////ĳ���� ������� y�� ��ġ���� ���� ��ġ�� ������ ��� ������ �����
	//Ogre::Vector3 pos = mTargetNode->getPosition();
	//if(mPeak && mapCollision(Ogre::Vector3(0, -1, 0), 2))
	//{
	//	mJump = false;
	//	mTargetNode->setPosition(pos);

	//	anim->stopAnimation(mState);
	//	mState = IDLE;
	//	anim->setBaseAnimation(mState, true);
	//	mPeak = false;
	//	mVerticalVelocity[0] = VELOCITY_DEFAULT;
	//	mDirection.y = 0;
	//	
	//}
}

/*================================================================================
                               Move
================================================================================*/
void CCharacter::move(Ogre::Real deltaTime)
{
	//Ű�� �����ִ��� �ȴ����ִ��� üũ�ؼ� translate�� ���� ĳ���� �̵�
	//timeSinceLastFrame�� ���� �����ӿ��� ���� �����ӱ��� �Ѿ�µ� �ɸ��� �ð��� ǥ���ϹǷ�
	//deltaTime���� �����ְ� �Ǹ� ��� �������̵� �̵��ӵ��� �����ϰ� �ȴ�

	//if(!mDirection.isZeroLength() && mState == IDLE)	//�̵�
	//if(mHit == false)
	//{
	if(mCurrentSpeed.x != 0 && mCurrentSpeed.y == 0 && mState == IDLE)
	{
		anim->setBaseAnimation(RUN);
		mState = RUN;
	}
	//}
	//if(mDirection.isZeroLength() && mState == RUN)		//IDLE
	if(mCurrentSpeed.x == 0 && mState == RUN)		//IDLE
	{
		//anim->stopAnimation(mState);
		//MotionNumber = 0;
		anim->setBaseAnimation(IDLE);
		mState = IDLE;
		//mCurrentState = IDLE;
	}

	if(mCurrentSpeed.y < -1 && mState != LANDING && mSkill[0] == false && mSkill[1] == false && mSkill[2] == false)
	{
		//anim->stopAnimation(mState);
		mState = LANDING;
		mTimer = 0;
	}
	//0���� �ϸ� ���� �����Ѵ�...��¿�� ���� �����ѹ�
	if((mCurrentSpeed.y > 5 && (mState == IDLE || mState == RUN || mState == LANDING)))
	{
		mState = JUMP;
		SoundMan::getSingletonPtr()->playSound("Jump");
		mTimer = 0;
	}
	
	//��ų���� Ÿ�̸Ӹ� 0���� �ʱ�ȭ��Ų��
	if((mSkill[0] || mSkill[1] || mSkill[2]) && (mState == IDLE || mState == RUN || mState == ATTACK || mState == ATTACK2 || mState == ATTACK3))
	{
		mTimer = 0;
	}
	//�������϶� Ÿ�̸Ӹ� 0���� �ʱ�ȭ
	//if(mState == ATTACK3 || mState == IDLE)
	//{
	//	MotionNumber = 0;
	//}
	if(MotionNumber == 1 && (mState == IDLE || mState == RUN))
	{
		mState = ATTACK;
		mTimer = 0;
	}
	if(MotionNumber == 2 && mState == ATTACK)
	{
		mState = ATTACK2;
		mTimer = 0;
	}
	if(MotionNumber == 3 && mState == ATTACK2)
	{
		mState = ATTACK3;
		mTimer = 0;
	}
	//std::cout<<mState<<std::endl;

	mTargetNode->translate(mCurrentSpeed * deltaTime);
	//ĳ���� ȸ����Ű�� �κ�
	//ĳ������ ��ǥ ���� ZERO�� �ʱ�ȭ
	//if(mState != ATTACK)

	SeeToDirection();
}

/*================================================================================
                              Collision
================================================================================*/
bool CCharacter::mapCollision(Ogre::Vector3 RayDirection, int QueryMask)
{
	Ogre::Vector3 charaPos = mTargetNode->getPosition();
	//charaPos.y = 500000;

	Ogre::Ray terrainRay(charaPos, RayDirection);
	//Ogre::Ray floorRay(charaPos, Ogre::Vector3(0, -1, 0));
	Ogre::RaySceneQuery *mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mRaySceneQuery->setQueryMask(QueryMask);
	mRaySceneQuery->setRay(terrainRay);
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();	//���⼭ �浹�˻簡 �̷����
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	for(itr = result.begin() ; itr != result.end() ; ++itr)
	{
		if(itr->movable != NULL && itr->movable->getName().substr(0, 4) == "map_")
		{
			if(QueryMask == 1)
			{
				if(itr->distance <= 3.0f)
					return true;
			}
			if(QueryMask == 2)
			{
				if(itr->distance <= 0.2f)
					return true;
			}
		}	
	}
	return false;
}

/*================================================================================
                              Animation
================================================================================*/
void CCharacter::UpdateAnimation(const Ogre::FrameEvent &evt)
{
	switch(mJob)
	{
	case 0:
		WalkerAnimation(evt.timeSinceLastFrame);
		break;
	case 1:
		GunnerAnimation(evt.timeSinceLastFrame);
		break;
	case 2:
		PsykerAnimation(evt.timeSinceLastFrame);
		break;
	}

	//���� ��Ŀ�� 3�� ��ų�� ����� �ߺ��ǹǷ� �̰ɷ� ������Ѿ� �Ѵ�
	if(mJob == 0 && mState == SKILL3)
		anim->playAnimation(evt.timeSinceLastFrame, ATTACK3);
	else if(mState == DEAD)
	{
		switch(mJob)
		{
		case 0:
			if(anim->GetAnimationState(DOWN)->getTimePosition() <= 0.31)	
				anim->playAnimation(evt.timeSinceLastFrame, DOWN);
			break;
		case 1:
			if(anim->GetAnimationState(DOWN)->getTimePosition() <= 0.44)	
				anim->playAnimation(evt.timeSinceLastFrame, DOWN);
			break;
		case 2:
			if(anim->GetAnimationState(DOWN)->getTimePosition() <= 0.46)
				anim->playAnimation(evt.timeSinceLastFrame, DOWN);
			break;
		}
		//anim->playAnimation(evt.timeSinceLastFrame, DOWN);
	}
	else if(mJob == 0 && mState == HIT)
	{
		//if(anim->GetAnimationState(DOWN)->getTimePosition() <= 1.0)
		//std::cout<<"�¾Ҵ�"<<std::endl;
		if(anim->GetAnimationState(DOWN)->getTimePosition() <= 0.05)
			anim->playAnimation(evt.timeSinceLastFrame, DOWN, 0.3);
		else
		{
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	else if(mJob == 1 && mState == HIT)
	{
		if(anim->GetAnimationState(DOWN)->getTimePosition() <= 0.05)
			anim->playAnimation(evt.timeSinceLastFrame, DOWN, 0.3);
		else
		{
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	else if(mJob == 2 && mState == HIT)
	{
		if(anim->GetAnimationState(DOWN)->getTimePosition() <= 0.05)
			anim->playAnimation(evt.timeSinceLastFrame, DOWN, 0.3);
		else
		{
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	else if(mState == DOWN)
	{
		anim->playAnimation(evt.timeSinceLastFrame, DOWN, 0.5);
	}
	else
		anim->playAnimation(evt.timeSinceLastFrame, mState);
}

/*================================================================================
                               SeeToDirection
================================================================================*/
void CCharacter::SeeToDirection()
{
	Ogre::Vector3 mGoalDirection = Ogre::Vector3::ZERO;

	//��� ���� ���� ���⺤�͸� ���ϰ�, ��������ȭ ��Ų��
	mGoalDirection += mDirection.x;
	mGoalDirection.y = 0;
	mGoalDirection.z = 0;
	mGoalDirection.normalise();


	Ogre::Quaternion toGoal = mTargetNode->getOrientation().zAxis().getRotationTo(mGoalDirection);
	Ogre::Real yawToGoal = toGoal.getYaw().valueDegrees();

	mTargetNode->yaw(Ogre::Degree(yawToGoal));
}

/*================================================================================
                               Job Animation
================================================================================*/
void CCharacter::WalkerAnimation(Ogre::Real Timer)
{
	mTimer += Timer;
	//����
	if(mState == DEAD)
	{
		anim->setBaseAnimation(DOWN);
	}
	//���� 1Ÿ
	//if((mState == IDLE || mState == RUN) && bInput)
	if(mState == ATTACK)
	{
		anim->setBaseAnimation(ATTACK);
		bInput = false;

		//����Ʈ ���
		//effect->Play(mSceneMgr, mTargetNode, Effect::Hit1);
		if(anim->GetAnimationState(ATTACK)->getTimePosition() >= 0.1)
		bInput = true;

		//if(anim->GetAnimationState(ATTACK)->getTimePosition() >= 0.42)
		if(mTimer >= anim->GetAnimationState(ATTACK)->getLength())
		{
			MotionNumber = 0;
			//mCurrentState = ATTACK;
			anim->setBaseAnimation(IDLE);
		//	SoundMan::getSingletonPtr()->stopSound("walker0");
			mState = IDLE;
		}
	}
	//���� 2Ÿ
	if(mState == ATTACK2)
	{
		anim->setBaseAnimation(ATTACK2);
		bInput = false;

		if(anim->GetAnimationState(ATTACK2)->getTimePosition() >= 0.1)
		bInput = true;

		//if(anim->GetAnimationState(ATTACK2)->getTimePosition() >= 0.42)
		if(mTimer >= anim->GetAnimationState(ATTACK2)->getLength())
		{
			MotionNumber = 0;
			anim->setBaseAnimation(IDLE);
		//	SoundMan::getSingletonPtr()->stopSound("walker0");
		//	SoundMan::getSingletonPtr()->stopSound("gunner333");
			mState = IDLE;
		}
	}
	//���� 3Ÿ
	if(mState == ATTACK3)
	{
		anim->setBaseAnimation(ATTACK3);
		bInput = false;

		if(anim->GetAnimationState(ATTACK3)->getTimePosition() >= 0.69)
		bInput = true;


		//if(anim->GetAnimationState(ATTACK3)->getTimePosition() >= 0.65)
		if(mTimer >= anim->GetAnimationState(ATTACK3)->getLength())
		{
			MotionNumber = 0;
			anim->setBaseAnimation(IDLE);
		//	SoundMan::getSingletonPtr()->stopSound("walker0");
			mState = IDLE;
			//MotionNumber = 1;
		}
	}
	//��ų1��
	if(mSkill[0] == true)											
	{	
		if((mState == IDLE || mState == RUN || mState == ATTACK || mState == ATTACK2 || mState == ATTACK3) && bInput)
		{
			anim->setBaseAnimation(SKILL1);
			mState = SKILL1;
			bInput = false;

			//����Ʈ ���
			effect->Play(mSceneMgr, mTargetNode, Effect::WalkerSkill2, mDirection);
		}

		//if(anim->GetAnimationState(SKILL1)->getTimePosition() >= 0.97)
		if(mTimer >= anim->GetAnimationState(SKILL1)->getLength())
		{
			MotionNumber = 0;
			mSkill[0] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	//��ų2��
	if(mSkill[1] == true)											
	{																
		if((mState == IDLE || mState == RUN || mState == ATTACK || mState == ATTACK2 || mState == ATTACK3) && bInput)
		{
			mState = SKILL2;
			anim->setBaseAnimation(SKILL2);
			bInput = false;

			//����Ʈ ���
			effect->Play(mSceneMgr, mTargetNode, Effect::WalkerSkill1, mDirection);
		}
		//if(anim->GetAnimationState(SKILL2)->getTimePosition() >= anim->GetAnimationState(SKILL2)->getLength()-0.01)
		if(mTimer >= anim->GetAnimationState(SKILL2)->getLength())
		{
			MotionNumber = 0;
			mSkill[1] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	//��ų3��
	if(mSkill[2] == true)				//������ ĵ�� �ϰ� ����ϱ� ���� ��ƾ...�̾��µ� �ȵȴ�
	{
		if((mState == IDLE || mState == RUN || mState == ATTACK || mState == ATTACK2 || mState == ATTACK3) && bInput)
		{
			//if(mState == ATTACK3)
				//anim->stopAnimation(ATTACK3);
			mState = SKILL3;
			anim->setBaseAnimation(ATTACK3);		//�ϴ� ��Ŀ�� 3�ܰ��� ������� ü����
			bInput = false;
		}

		if(anim->GetAnimationState(ATTACK3)->getTimePosition() >= 0.4 && bEffect) 
		{
			//����Ʈ ���
			effect->Play(mSceneMgr, mTargetNode, Effect::WalkerExplosion, mDirection);
			SoundMan::getSingletonPtr()->playSound("walker3");
			bEffect = false;
		}
		else if(mTimer >= anim->GetAnimationState(ATTACK3)->getLength()) 
		{
			MotionNumber = 0;
			mSkill[2] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
			bEffect = true;
		}
	}
	if(mGuard == true)
	{
		if(mState == IDLE || mState == RUN)
		{
			mState = GUARD;
			anim->setBaseAnimation(GUARD);
		}
		if(anim->GetAnimationState(GUARD)->getTimePosition() >= 0.23)
		{
			//anim->GetAnimationState(GUARD)->setTimePosition(0.23);
		}
	}
	if(mState == HIT)
	{
		// �ǰ� ����Ʈ ���
		//effect->Play(mSceneMgr, mTargetNode, Effect::Hit1);
		//mState = HIT;
		anim->setBaseAnimation(DOWN);


	/*	if(mTimer >= 0.1)
=======
		if(mTimer >= 0.1)
>>>>>>> .r286
		{
<<<<<<< .mine
			std::cout<<"�¾Ҵ�"<<std::endl;
=======
			mHit = false;
>>>>>>> .r286
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			mHit = false;
		}*/
	}
	if(mState == JUMP)
	{
		anim->setBaseAnimation(JUMP);
		//if(anim->GetAnimationState(JUMP)->getTimePosition() >= anim->GetAnimationState(JUMP)->getLength()-0.02)
		
		//if(mTimer >= anim->GetAnimationState(JUMP)->getLength())
		if(anim->GetAnimationState(JUMP)->getTimePosition() >= 0.22)
		{
			anim->GetAnimationState(JUMP)->setTimePosition(0.22);
			//std::cout<<anim->GetAnimationState(JUMP)->getTimePosition()<<std::endl;
			//mCurrentState = JUMP;
		}
	}
	if(mState == LANDING)
	{
		anim->setBaseAnimation(LANDING);
		if(mGround == false)
		{
			if(anim->GetAnimationState(LANDING)->getTimePosition() >= 0.12)
				anim->GetAnimationState(LANDING)->setTimePosition(0.12);
		}
		else
		{
			//if(anim->GetAnimationState(LANDING)->getTimePosition() >= anim->GetAnimationState(LANDING)->getLength()-0.03)
			//mState = IDLE;
			if(mTimer >= anim->GetAnimationState(LANDING)->getLength())
			{
				//mCurrentState = LANDING;
				mState = IDLE;
				anim->setBaseAnimation(IDLE);
			}
		}
	}
}

void CCharacter::PsykerAnimation(Ogre::Real Timer)
{
	mTimer += Timer;
	//����
	if(mState == DEAD)
	{
		anim->setBaseAnimation(DOWN);
	}
	//����
	if(mState == ATTACK)
	{
		anim->setBaseAnimation(ATTACK);
		bInput = false;

		if(anim->GetAnimationState(ATTACK)->getTimePosition() >= 0.1)
			bInput = true;

		//if(anim->GetAnimationState(ATTACK)->getTimePosition() >= anim->GetAnimationState(ATTACK)->getLength()-0.01)
		if(mTimer >= anim->GetAnimationState(ATTACK)->getLength())
		{

			MotionNumber = 0;
			anim->setBaseAnimation(IDLE);
			mState = IDLE;
		}
	}
	//��ų1��
	if(mSkill[0] == true)											
	{																
		if((mState == IDLE || mState == RUN || mState == ATTACK) && bInput)
		{
			anim->setBaseAnimation(SKILL1);
			mState = SKILL1;
			bInput = false;
		}
		
		if(anim->GetAnimationState(SKILL1)->getTimePosition() >= 0.33 && bEffect)
		{
			// ���� ����Ʈ ���
			effect->Play(mSceneMgr, mTargetNode, Effect::PsykerSkill1, mDirection);
			bEffect = false;
		}
		else if(mTimer >= anim->GetAnimationState(SKILL1)->getLength())
		{
			MotionNumber = 0;
			mSkill[0] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
			bEffect = true;
		}
	}
	//��ų2��
	if(mSkill[1] == true)											
	{																
		if((mState == IDLE || mState == RUN || mState == ATTACK) && bInput)
		{
			mState = SKILL2;
			anim->setBaseAnimation(SKILL2);
			bInput = false;

		}
		//if(anim->GetAnimationState(SKILL2)->getTimePosition() >= 1.32)
		if(mTimer >= anim->GetAnimationState(SKILL2)->getLength())
		{
			MotionNumber = 0;
			mSkill[1] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	//��ų3��
	if(mSkill[2] == true)				
	{
		if((mState == IDLE || mState == RUN || mState == ATTACK) && bInput)
		{ 
			mState = SKILL3;
			anim->setBaseAnimation(SKILL3);
			bInput = false;
		}
		
		if(anim->GetAnimationState(SKILL3)->getTimePosition() >= 1.0 && bEffect) 
		{
			// ���� ����Ʈ ���
			effect->Play(mSceneMgr, mTargetNode, Effect::PsykerSkill2, mDirection);
			bEffect = false;
		}
		else if(mTimer >= anim->GetAnimationState(SKILL3)->getLength()) 
		{
			MotionNumber = 0;
			mSkill[2] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
			bEffect = true;
		}
	}
	if(mState == HIT)
	{
		// �ǰ� ����Ʈ ���
		//effect->Play(mSceneMgr, mTargetNode, Effect::Hit1);
		anim->setBaseAnimation(DOWN);
		//mState = HIT;
		/*
		if(mTimer >= 0.1)
		{
			mHit = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
		}*/
	}
	if(mState == JUMP)
	{
		anim->setBaseAnimation(JUMP);
		//if(mTimer >= anim->GetAnimationState(JUMP)->getLength())
		if(anim->GetAnimationState(JUMP)->getTimePosition() >= 0.52)
		{
			anim->GetAnimationState(JUMP)->setTimePosition(0.52);
		}
	}
	if(mState == LANDING)
	{
		anim->setBaseAnimation(LANDING);
		if(mGround == false)
		{
			if(anim->GetAnimationState(LANDING)->getTimePosition() >= 0.22)
				anim->GetAnimationState(LANDING)->setTimePosition(0.22);
		}
		else
		{
			if(mTimer >= anim->GetAnimationState(LANDING)->getLength())
			{
				//anim->stopAnimation(mState);
				mState = IDLE;
				anim->setBaseAnimation(IDLE);
			}
		}
	}
}

void CCharacter::GunnerAnimation(Ogre::Real Timer)
{
	mTimer += Timer;
	//����
	if(mState == DEAD)
	{
		anim->setBaseAnimation(DOWN);
	}
	//����
	if(mState == ATTACK)
	{
			//std::cout<<"dd"<<std::endl;
		anim->setBaseAnimation(ATTACK);
		bInput = false;

		if(anim->GetAnimationState(ATTACK)->getTimePosition() >= 0.1)
			bInput = true;

		//mState = ATTACK; 
		//if(mTimer >= anim->GetAnimationState(ATTACK)->getLength())
		if(mTimer >= 0.55)
		{
			//std::cout<<"�ι�����?"<<std::endl;
			MotionNumber = 0;
			anim->setBaseAnimation(IDLE);
			mState = IDLE;
		}
	}
	//��ź
	if(mState == ATTACK2)
	{
		anim->setBaseAnimation(ATTACK2);
		bInput = false;

		if(anim->GetAnimationState(ATTACK)->getTimePosition() >= 0.1)
			bInput = true;
		
		if(anim->GetAnimationState(ATTACK2)->getTimePosition() >= 0.72)
		{
			MotionNumber = 0;
			anim->setBaseAnimation(IDLE);
			mState = IDLE;
		}
	}
	//��ų1��
	if(mSkill[0] == true)											
	{		
		if((mState == IDLE || mState == RUN || mState == ATTACK || mState == ATTACK2 || mState == ATTACK3) && bInput)
		{	
			mState = SKILL1;
			anim->setBaseAnimation(SKILL1);
			bInput = false;
		}
		//if(anim->GetAnimationState(SKILL1)->getTimePosition() >= 0.79)
		if(mTimer >= anim->GetAnimationState(SKILL1)->getLength())
		{
			MotionNumber = 0;
			mSkill[0] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	//��ų2��
	if(mSkill[1] == true)											
	{																
		if((mState == IDLE || mState == RUN) && bInput)
		{
			mState = SKILL2;
			anim->setBaseAnimation(SKILL2);
			bInput = false;
		}
		//if(anim->GetAnimationState(SKILL2)->getTimePosition() >= 1.32)
		if(mTimer >= anim->GetAnimationState(SKILL2)->getLength())
		{
			MotionNumber = 0;
			mSkill[1] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
		}
	}
	//��ų3��
	if(mSkill[2] == true)				
	{
		if((mState == IDLE || mState == RUN) && bInput)
		{
			mState = SKILL3;
			anim->setBaseAnimation(SKILL3);
			bInput = false;
		}

		if(anim->GetAnimationState(SKILL3)->getTimePosition() >= 0.66 && bEffect) 
		{
			// ����Ʈ ���
			effect->Play(mSceneMgr, mTargetNode, Effect::Shoot, mDirection);
			SoundMan::getSingletonPtr()->playSound("gunner333");
			bEffect = false;
		}
		else if(mTimer >= anim->GetAnimationState(SKILL3)->getLength())
		{
			MotionNumber = 0;
			mSkill[2] = false;
			mState = IDLE;
			anim->setBaseAnimation(IDLE);
			bInput = true;
			bEffect = true;
		}
	}
	if(mState == HIT)
	{
		// �ǰ� ����Ʈ ���
		//effect->Play(mSceneMgr, mTargetNode, Effect::Hit1);

		anim->setBaseAnimation(DOWN);
		//mState = HIT;
		//
		//if(mTimer >= 0.2)
		////if(mTimer >= anim->GetAnimationState(SKILL1)->getLength())
		//{
		//	mHit = false;
		//	mState = IDLE;
		//	anim->setBaseAnimation(IDLE);
		//}
	}

	if(mState == JUMP)
	{
		anim->setBaseAnimation(JUMP);
		//if(anim->GetAnimationState(JUMP)->getTimePosition() >= anim->GetAnimationState(JUMP)->getLength()-0.01)
		//if(mTimer >= anim->GetAnimationState(JUMP)->getLength())
		if(anim->GetAnimationState(JUMP)->getTimePosition() >= 0.29)
		{
			anim->GetAnimationState(JUMP)->setTimePosition(0.29);
		}
	}
	if(mState == LANDING)
	{
		anim->setBaseAnimation(LANDING);
		if(mGround == false)
		{
			if(anim->GetAnimationState(LANDING)->getTimePosition() >= 0.11)
				anim->GetAnimationState(LANDING)->setTimePosition(0.11);
		}
		else
		{
			//if(anim->GetAnimationState(LANDING)->getTimePosition() >= 0.20)
			if(mTimer >= anim->GetAnimationState(LANDING)->getLength())
			{
				mState = IDLE;
				anim->setBaseAnimation(IDLE);
			}
		}
	}
}

